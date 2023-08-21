#include <iostream>
#include <thread>
#include <condition_variable>

extern "C" {
#include <SDL2/SDL.h>
#include <al.h>
#include <alc.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#define NUM_BUFFERS 3
// ������Ƶ����
const int WIDTH = 640;
const int HEIGHT = 480;

// ��Ƶ��ʱ���
int64_t audioPts = 0;
// ��Ƶ֡ʱ���
int64_t videoPts = 0;
// ��ֵ
int64_t ptsDifference = 0;

std::mutex mtx;
std::condition_variable cv;
bool isAudioReady = false;
bool isVideoReady = false;
bool isPlaybackFinished = false;

// �������ں���Ⱦ��
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;

bool isPlaying = true;
bool isPaused = false;
bool shouldVideoRestart = false;
bool shouldAudioRestart = false;
bool isVideoRight = false;
bool isVideoLeft = false;
bool isAudioRight = false;
bool isAudioLeft = false;
bool isUp = false;
bool isDown = false;

int64_t audioCurrentTime = 0;
int64_t videoCurrentTime = 0;

// ��Ƶ�����߳�
void audioThread(AVFormatContext* formatContext, AVCodecContext* codecContext, int audioStreamIndex,AVCodecParameters* audioCodecParameters) {
    // ��ʼ��OpenAL
    ALCdevice* device = alcOpenDevice(nullptr);
    if (device == nullptr) {
        std::cout << "�޷�����Ƶ�豸" << std::endl;
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
    }

    ALCcontext* context = alcCreateContext(device, nullptr);
    if (context == nullptr) {
        std::cout << "�޷�������Ƶ������" << std::endl;
        alcCloseDevice(device);
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
    }
    std::cout << "������Ƶ�����ĳɹ�" << std::endl;
    ALint deviceSampleRate = 0;
    alcGetIntegerv(device, ALC_FREQUENCY, sizeof(ALint), &deviceSampleRate);
    std::cout << "�豸��Ĭ�ϲ�����Ϊ��" << deviceSampleRate << " Hz" << std::endl;


    if (alcMakeContextCurrent(context) == ALC_FALSE) {
        std::cout << "�޷�������Ƶ������" << std::endl;
        alcDestroyContext(context);
        alcCloseDevice(device);
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
    }

    // ������ƵԴ����
    ALuint source;
    alGenSources(1, &source);

    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_LOOPING, AL_FALSE);

    // ������Ƶ����������
    ALuint buffers[NUM_BUFFERS];
    alGenBuffers(NUM_BUFFERS, buffers);

    // ��ȡ��������Ƶ����
    std::cout << "��ȡ��������Ƶ����" << std::endl;
    AVPacket* packet = av_packet_alloc();
    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == audioStreamIndex) {
            AVStream* audioStream = formatContext->streams[audioStreamIndex];
            // ��ȡ��Ƶ����ʱ���׼��������
            AVRational timeBase = audioStream->time_base;
            // ��ȡ��Ƶ����ʱ���pts ��ǰʱ��
            audioCurrentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;

            audioPts = packet->pts;
            // ����Ƶ���ݰ����͸�������
            int sendResult = avcodec_send_packet(codecContext, packet);
            if (sendResult < 0) {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(sendResult, errbuf, AV_ERROR_MAX_STRING_SIZE);
                std::cout << "�޷�������Ƶ���ݰ���������: " << errbuf << std::endl;
                av_packet_unref(packet);
                break;
            }
            // std::cout << "������Ƶ���ݰ����������ɹ�" << std::endl;
            while (true) {
                // ���ս�������Ƶ���ݰ�
                AVFrame* frame = av_frame_alloc();
                int receiveResult = avcodec_receive_frame(codecContext, frame);
                if (receiveResult == AVERROR(EAGAIN)) {
                    // ��������Ҫ������������
                    av_frame_free(&frame);
                    break;
                } else if (receiveResult == AVERROR_EOF) {
                    // �������Ѿ���������������
                    av_frame_free(&frame);
                    break;
                } else if (receiveResult < 0) {
                    char errbuf[AV_ERROR_MAX_STRING_SIZE];
                    av_strerror(receiveResult, errbuf, AV_ERROR_MAX_STRING_SIZE);
                    std::cout << "�޷��ӽ�������������: " << errbuf << std::endl;
                    av_packet_unref(packet);
                    break;
                }
                // ֪ͨ���߳���Ƶ����׼����
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    isAudioReady = true;
                }
                cv.notify_all();

                // �ȴ����̵߳Ĳ����ź�
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [] { return isPlaybackFinished; });
                }
                // std::cout << "�ӽ������������ݳɹ�" << std::endl;
                // ����Ŀ����Ƶ֡
                AVFrame* targetFrame = av_frame_alloc();
                targetFrame->format = AV_SAMPLE_FMT_S16;
                targetFrame->channels = 2; // ˫����
                targetFrame->channel_layout = av_get_default_channel_layout(2); // ˫������channel_layout
                targetFrame->sample_rate = 44100;
                targetFrame->nb_samples = frame->nb_samples;
                av_frame_get_buffer(targetFrame, 0);

                // ������Ƶת��������
                SwrContext* swrContext = swr_alloc_set_opts(nullptr,
                                                            targetFrame->channel_layout,
                                                            AV_SAMPLE_FMT_S16,
                                                            targetFrame->sample_rate,
                                                            frame->channel_layout,
                                                            static_cast<AVSampleFormat>(frame->format),
                                                            frame->sample_rate,
                                                            0,
                                                            nullptr);
                // ��ʼ����Ƶת��������
                swr_init(swrContext);
                // ������Ƶ���ݸ�ʽת��
                swr_convert_frame(swrContext, targetFrame, frame);
                // �ͷ���Դ
                swr_free(&swrContext);

                // ������Ƶ����
                // ����Ƶ������䵽������
                ALuint buffer;
                alGenBuffers(1, &buffer);
                alBufferData(buffer, AL_FORMAT_STEREO16, targetFrame->data[0], targetFrame->linesize[0], targetFrame->sample_rate);

                // �����������ӵ���ƵԴ�Ļ���������
                alSourceQueueBuffers(source, 1, &buffer);

                // �����Ƶ����״̬
                ALint state;
                alGetSourcei(source, AL_SOURCE_STATE, &state);
                if (state != AL_PLAYING) {
                    // ��ʼ������Ƶ
                    alSourcePlay(source);
                }

                // ��黺��������״̬
                ALint queuedBuffers;
                alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);

                if (queuedBuffers >= NUM_BUFFERS) {
                    // �����������������ȴ�����������
                    while (queuedBuffers >= NUM_BUFFERS) {
                        // �жϻ����������Ѵ����Ļ���������
                        ALint processedBuffers;
                        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processedBuffers);

                        // ������Ѵ����Ļ��������ӻ������������Ƴ����ͷ�����
                        if (processedBuffers > 0) {
                            ALuint unqueuedBuffers[processedBuffers];
                            alSourceUnqueueBuffers(source, processedBuffers, unqueuedBuffers);
                            alDeleteBuffers(processedBuffers, unqueuedBuffers);
                        }

                        // ��黺��������״̬
                        alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);
                    }
                }

                // �ͷ���Ƶ֡
                av_frame_free(&frame);
                av_frame_free(&targetFrame);
            }
        }

        while(isPaused){
            // std::cout << "��ͣ" << std::endl;
        }

        if (!isPlaying) {
            std::cout << "�˳�����" << std::endl;
            break;
        }

        if (shouldAudioRestart) {
            // ���²���
            av_seek_frame(formatContext, audioStreamIndex, 0, AVSEEK_FLAG_FRAME);
            shouldAudioRestart = false;
            isPaused = false;
        }
        if(isAudioRight){
            std::cout << "ǰ��30s" <<std::endl;
            AVStream* audioStream = formatContext->streams[audioStreamIndex];
            // ��ȡ��Ƶ����ʱ���׼��������
            AVRational timeBase = audioStream->time_base;
            // ��ȡ��Ƶ����ʱ���pts ��ǰʱ��
            // int64_t currentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
            std::cout << "currentTime: " << audioCurrentTime << " (" << timeBase.num << "/" << timeBase.den << ")" << std::endl;
            std::cout << "��ǰʱ��(΢��)��" << audioCurrentTime <<std::endl;
            int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
            std::cout << "��תʱ��(΢��)��" << span <<std::endl;
            int64_t targetTime = audioCurrentTime + (30 * AV_TIME_BASE);  // ����30���ʱ����
            std::cout << "Ŀ��ʱ��(΢��)��" << targetTime <<std::endl;
            int64_t maxTime = audioStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
            std::cout << "��ʱ��(΢��)��" << maxTime <<std::endl;

            if (targetTime >= maxTime) {
                targetTime = maxTime;
            }

            int seekFlags = AVSEEK_FLAG_BACKWARD;  // ʹ��AVSEEK_FLAG_BACKWARD��־��ȷ����ǰ��ת
            int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase); // ��timestamp�ĵ�λת��Ϊ��Ƶ����ʱ���׼
            int seekResult = av_seek_frame(formatContext, audioStreamIndex, timestamp, seekFlags);

            if (seekResult < 0) {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(seekResult, errbuf, sizeof(errbuf));
                std::cout << "����" << errbuf << std::endl;
            } else {
                // ���²�����صı���
                alSourceStop(source);  // ֹͣ��ǰ��Ƶ
                alSourceRewind(source);  // ������Ƶ����λ��
                audioCurrentTime = targetTime;  // ���µ�ǰʱ��
                alSourcePlay(source);  // ������Ƶ
            }

//            int64_t currentTimeSec = currentTime / AV_TIME_BASE; // ��currentTime�ĵ�λת��Ϊ��
//            int64_t spanSec = span / AV_TIME_BASE; // ��span�ĵ�λת��Ϊ��
//            int64_t targetTimeSec = targetTime / AV_TIME_BASE; // Ŀ��ʱ�������
//            int64_t maxTimeSec = maxTime / AV_TIME_BASE; // ��maxTime�ĵ�λת��Ϊ��
//
//            std::cout << "��ǰʱ�䣺" << currentTimeSec<< "��" << std::endl;
//            std::cout << "��תʱ�䣺" << spanSec << "��" << std::endl;
//            std::cout << "Ŀ��ʱ�䣺" << targetTimeSec << "��" << std::endl;
//            std::cout << "��ʱ�䣺" << maxTimeSec << "��" << std::endl;
            isAudioRight = false;
        }
        if(isAudioLeft){
            std::cout << "����30s" <<std::endl;
            AVStream* audioStream = formatContext->streams[audioStreamIndex];
            AVRational timeBase = audioStream->time_base;
            std::cout << "��ǰʱ��(΢��)��" << audioCurrentTime <<std::endl;
            int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
            std::cout << "��תʱ��(΢��)��" << span <<std::endl;
            int64_t targetTime = audioCurrentTime - span;  // ��ȥ30���ʱ����
            std::cout << "Ŀ��ʱ��(΢��)��" << targetTime <<std::endl;
            int64_t maxTime = audioStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
            std::cout << "��ʱ��(΢��)��" << maxTime <<std::endl;

            if (targetTime < 0) {
                targetTime = 0;  // ��������ת����ʱ��
            }
            int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase); // ��timestamp�ĵ�λת��Ϊ��Ƶ����ʱ���׼

            int seekFlags = AVSEEK_FLAG_BACKWARD;  // ʹ��AVSEEK_FLAG_BACKWARD��־��ȷ����ǰ��ת

            int seekResult = av_seek_frame(formatContext, audioStreamIndex, timestamp, seekFlags);

            if (seekResult < 0) {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(seekResult, errbuf, sizeof(errbuf));
                std::cout << "����" << errbuf << std::endl;
            } else {
                // ���²�����صı���
                alSourceStop(source);  // ֹͣ��ǰ��Ƶ
                alSourceRewind(source);  // ������Ƶ����λ��
                audioCurrentTime = targetTime;  // ���µ�ǰʱ��
                alSourcePlay(source);  // ������Ƶ
            }

//            int64_t currentTimeSec = currentTime / AV_TIME_BASE; // ��currentTime�ĵ�λת��Ϊ��
//            int64_t spanSec = span / AV_TIME_BASE; // ��span�ĵ�λת��Ϊ��
//            int64_t targetTimeSec = targetTime / AV_TIME_BASE; // Ŀ��ʱ�������
//            int64_t maxTimeSec = maxTime / AV_TIME_BASE; // ��maxTime�ĵ�λת��Ϊ��
//
//            std::cout << "��ǰʱ�䣺" << currentTimeSec<< "��" << std::endl;
//            std::cout << "��תʱ�䣺" << spanSec << "��" << std::endl;
//            std::cout << "Ŀ��ʱ�䣺" << targetTimeSec << "��" << std::endl;
//            std::cout << "��ʱ�䣺" << maxTimeSec << "��" << std::endl;
            isAudioLeft = false;
        }
        if(isUp){
            // ��������
            float gain;
            alGetSourcef(source, AL_GAIN, &gain);
            gain = std::min(gain + 0.1f, 1.0f);
            alSourcef(source, AL_GAIN, gain);
            isUp = false;
        }
        if(isDown){
            // ��С����
            float gain;
            alGetSourcef(source, AL_GAIN, &gain);
            gain = std::max(gain - 0.1f, 0.0f);
            alSourcef(source, AL_GAIN, gain);
            isDown = false;
        }

    }

    // �ȴ���Ƶ�������
    ALint state;
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);

    std::cout<< "�������" <<std::endl;

    // ֹͣ������Ƶ
    alSourceStop(source);

    // ��ջ���������
    ALint queuedBuffers;
    alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);
    if (queuedBuffers > 0) {
        alSourceUnqueueBuffers(source, std::min(queuedBuffers, NUM_BUFFERS), buffers);
    }
    // �ͷ���Դ
    av_packet_free(&packet);
    // �ͷ�OpenAL��Դ
    alDeleteSources(1, &source);
    alDeleteBuffers(NUM_BUFFERS, buffers);
    // �ر�OpenAL�����ĺ��豸
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

// ��Ƶ�����߳�
void videoThread(AVFormatContext* formatContext, AVCodecContext* codecContext, int videoStreamIndex) {
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = codecContext->width;
    srcRect.h = codecContext->height;

    SDL_Rect dstRect; // Ŀ����Σ�������Ⱦ��Ƶ֡��������
    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = WIDTH;
    dstRect.h = HEIGHT;

    // �������ű���
    float scaleX = static_cast<float>(dstRect.w) / srcRect.w;
    float scaleY = static_cast<float>(dstRect.h) / srcRect.h;

    // ��ȡ��Ƶ֡
    AVPacket* packet = av_packet_alloc();
    while (av_read_frame(formatContext, packet) >= 0) {
        // ����Ƿ�Ϊ��Ƶ��
        if (packet->stream_index == videoStreamIndex) {
            AVStream* videoStream = formatContext->streams[videoStreamIndex];
            // ��ȡ��Ƶ����ʱ���׼��������
            AVRational timeBase = videoStream->time_base;
            // ��ȡ��Ƶ����ʱ���pts ��ǰʱ��
            videoCurrentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
            // ������Ƶ���ݰ���������
            int sendResult = avcodec_send_packet(codecContext, packet);
            if (sendResult < 0) {
                std::cout << "�޷�������Ƶ���ݰ���������" << std::endl;
                break;
            }

            // ���ս�������Ƶ����֡
            AVFrame* frame = av_frame_alloc();
            while (avcodec_receive_frame(codecContext, frame) == 0) {
                // ֪ͨ���߳���Ƶ����׼����
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    isVideoReady = true;
                }
                cv.notify_all();

                // �ȴ����̵߳Ĳ����ź�
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [] { return isPlaybackFinished; });
                }
                videoPts = packet->pts;
                // ������Ƶ������Ƶ֡��ʱ�����ֵ
                ptsDifference = videoPts - audioPts;
                // std::cout << "ʱ��" << 1.0 * ptsDifference / AV_TIME_BASE << "��" << std::endl;
                int delayTime = static_cast<int>(ptsDifference * av_q2d(timeBase));
                // std::cout << delayTime <<std::endl;
                // SDL_Delay(delayTime);

                // ����Ŀ����εĳߴ�
                dstRect.w = static_cast<int>(srcRect.w * scaleX);
                dstRect.h = static_cast<int>(srcRect.h * scaleY);

                // ��Ⱦ��Ƶ֡
                SDL_UpdateYUVTexture(texture, &srcRect, frame->data[0], frame->linesize[0],
                                     frame->data[1], frame->linesize[1],
                                     frame->data[2], frame->linesize[2]);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
                SDL_RenderPresent(renderer);
                SDL_Delay(22); // ����֡��
            }
            av_frame_free(&frame);
        }

        while(isPaused){
            // std::cout << "��ͣ" << std::endl;
        }
        if (!isPlaying) {
            std::cout << "�˳�����" << std::endl;
            break;
        }

        if (shouldVideoRestart) {
            // ���²���
            std::cout << "���²���" << std::endl;
            SDL_RenderClear(renderer);
            av_seek_frame(formatContext, videoStreamIndex, 0, AVSEEK_FLAG_FRAME);
            shouldVideoRestart = false;
            isPaused = false;
        }
        if(isVideoRight){
            std::cout << "ǰ��30s" << std::endl;
            AVStream* videoStream = formatContext->streams[videoStreamIndex];
            // ��ȡ��Ƶ����ʱ���׼��֡��
            AVRational timeBase = videoStream->time_base;
            // int64_t currentTime = av_rescale_q(avio_tell(formatContext->pb), AV_TIME_BASE_Q, timeBase);

            int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
            int64_t targetTime = videoCurrentTime + span;

            int64_t maxTime = videoStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
            // std::cout << "��Ƶ��ʱ��(΢��)��" << maxTime <<std::endl;

            if (targetTime >= maxTime) {
                targetTime = maxTime;
            }

            int seekFlags = AVSEEK_FLAG_BACKWARD;
            int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase);
            int seekResult = av_seek_frame(formatContext, videoStreamIndex, timestamp, seekFlags);

            if (seekResult < 0) {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(seekResult, errbuf, sizeof(errbuf));
                std::cout << "����" << errbuf << std::endl;
            }
            isVideoRight = false;
        }
        if(isVideoLeft){
            std::cout << "����30s" << std::endl;
            AVStream* videoStream = formatContext->streams[videoStreamIndex];
            // ��ȡ��Ƶ����ʱ���׼��֡��
            AVRational timeBase = videoStream->time_base;
            int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
            int64_t targetTime = videoCurrentTime - span;

            int64_t minTime = 0;

            if (targetTime <= minTime) {
                targetTime = minTime;
            }

            int seekFlags = AVSEEK_FLAG_BACKWARD;
            int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase);
            int seekResult = av_seek_frame(formatContext, videoStreamIndex, timestamp, seekFlags);

            if (seekResult < 0) {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(seekResult, errbuf, sizeof(errbuf));
                std::cout << "����" << errbuf << std::endl;
            }
            isVideoLeft = false;
        }
    }

    // ������Դ
    av_packet_free(&packet);
}

int main(int argc, char* args[]) {
    // ��ʼ��FFmpeg
    avformat_network_init();
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);

    std::string filename;
    std::cout << "�������ļ�·��: ";
    std::getline(std::cin, filename);

    // ����Ƶ�ļ�
    AVFormatContext* audioFormatContext = avformat_alloc_context();
    if (avformat_open_input(&audioFormatContext, filename.c_str(), NULL, NULL) != 0) {
        std::cout << "�޷�����Ƶ�ļ�" << std::endl;
        return -1;
    }

    // ������Ƶ����Ϣ
    if (avformat_find_stream_info(audioFormatContext, nullptr) < 0) {
        std::cerr << "�޷���������Ϣ" << std::endl;
        avformat_close_input(&audioFormatContext);
        return -1;
    }

    // ������Ƶ��
    int audioStreamIndex = -1;
    for (unsigned int i = 0; i < audioFormatContext->nb_streams; i++) {
        if (audioFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex < 0) {
            audioStreamIndex = i;
        }
    }

    // �����Ƶ���Ƿ����
    if (audioStreamIndex < 0) {
        std::cout << "��Ƶ��δ�ҵ�" << std::endl;
        return -1;
    }
    std::cout << "��Ƶ������: " << audioStreamIndex << std::endl;

    // ��ȡ��Ƶ����������
    AVCodecParameters* audioCodecParameters = audioFormatContext->streams[audioStreamIndex]->codecpar;
    // ��ȡ��Ƶ������
    AVCodec* audioCodec = const_cast<AVCodec *>(avcodec_find_decoder(audioCodecParameters->codec_id));
    if (audioCodec == NULL) {
        std::cout << "�Ҳ�����Ƶ������" << std::endl;
        return -1;
    }
    // ��ʼ����Ƶ������
    // ����������������
    AVCodecContext* audioCodecContext = avcodec_alloc_context3(audioCodec);
    // ���ý���������
    avcodec_parameters_to_context(audioCodecContext, audioCodecParameters);
    // �򿪽�����
    if (avcodec_open2(audioCodecContext, audioCodec, NULL) != 0) {
        std::cout << "�޷�����Ƶ������" << std::endl;
        return -1;
    }
    std::cout << "����Ƶ�������ɹ�" << std::endl;
    // ��ȡ��Ƶ����������
    int sampleRate = audioCodecContext->sample_rate;
    int channels = audioCodecContext->channels;
    AVSampleFormat sampleFormat = audioCodecContext->sample_fmt;

    // �����Ƶ��ʽ��Ϣ
    std::cout << "������: " << sampleRate << " Hz" << std::endl;
    std::cout << "ͨ����: " << channels << std::endl;
    std::cout << "��Ƶ��ʽ: " << av_get_sample_fmt_name(sampleFormat) << std::endl;

    // �����Ƶ������
    std::cout << "��Ƶ������: " << audioStreamIndex << std::endl;
    std::cout << "��Ƶ��ʱ��: " << audioFormatContext->streams[audioStreamIndex]->duration << std::endl;
    std::cout << "��Ƶ��������: " << audioCodecParameters->sample_rate << std::endl;
    std::cout << "��Ƶ��ͨ����: " << audioCodecParameters->channels << std::endl;
    std::cout << "��Ƶ����ʽ: " << av_get_pix_fmt_name((AVPixelFormat)audioCodecParameters->format) << std::endl;


    // ����Ƶ�ļ�
    AVFormatContext* videoFormatContext = avformat_alloc_context();
    if (avformat_open_input(&videoFormatContext, filename.c_str(), NULL, NULL) != 0) {
        std::cout << "�޷�����Ƶ�ļ�" << std::endl;
        return -1;
    }

    std::cout << "����Ƶ�ļ��ɹ� " << std::endl;

    // ������Ƶ����Ϣ
    if (avformat_find_stream_info(videoFormatContext, nullptr) < 0) {
        std::cerr << "�޷���������Ϣ" << std::endl;
        avformat_close_input(&videoFormatContext);
        return -1;
    }

    // ������Ƶ��
    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < videoFormatContext->nb_streams; i++) {
        if (videoFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex < 0) {
            videoStreamIndex = i;
        }
    }

    // �����Ƶ����Ƶ���Ƿ����
    if (videoStreamIndex < 0) {
        std::cout << "��Ƶ��δ�ҵ�" << std::endl;
        return -1;
    }
    std::cout << "��Ƶ������: " << videoStreamIndex << std::endl;

    // ��ȡ��Ƶ�����������
    AVCodecParameters* videoCodecParams = videoFormatContext->streams[videoStreamIndex]->codecpar;

    // ��ȡ��Ƶ������
    AVCodec* videoCodec = const_cast<AVCodec *>(avcodec_find_decoder(videoCodecParams->codec_id));
    if (videoCodec == NULL) {
        std::cout << "�Ҳ�����Ƶ������" << std::endl;
        return -1;
    }

    // ��ʼ����Ƶ������
    // ��ȡ��Ƶ������������
    AVCodecContext* videoCodecContext = avcodec_alloc_context3(videoCodec);
    // ���ý���������
    avcodec_parameters_to_context(videoCodecContext, videoCodecParams);
    if (avcodec_open2(videoCodecContext, videoCodec, NULL) != 0) {
        std::cout << "�޷�����Ƶ������" << std::endl;
        return -1;
    }

    // �����Ƶ��ʽ��Ϣ
    std::cout << "��Ƶ��ʽ: " << av_get_pix_fmt_name(videoCodecContext->pix_fmt) << std::endl;
    std::cout << "����: " << videoCodecContext->width << std::endl;
    std::cout << "�߶�: " << videoCodecContext->height << std::endl;
    std::cout << "������: " << videoCodecParams->bit_rate << " bps" << std::endl;

    // �����Ƶ������
    std::cout << "��Ƶ������: " << videoStreamIndex << std::endl;
    std::cout << "��Ƶ��ʱ��: " << videoFormatContext->streams[videoStreamIndex]->duration << std::endl;
    std::cout << "��Ƶ������: " << videoCodecParams->width << std::endl;
    std::cout << "��Ƶ���߶�: " << videoCodecParams->height << std::endl;
    std::cout << "��Ƶ����ʽ: " << av_get_pix_fmt_name((AVPixelFormat)videoCodecParams->format) << std::endl;
    std::cout << "��Ƶ��������: " << videoCodecParams->bit_rate << " bps" << std::endl;


    // ��ʼ��SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "�޷���ʼ��SDL" << std::endl;
    }

    // �������ں���Ⱦ��
    window = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, videoCodecContext->width, videoCodecContext->height);

    // ������Ƶ�����߳�
    std::thread audioProcessingThread(audioThread, audioFormatContext, audioCodecContext, audioStreamIndex, audioCodecParameters);
    // ������Ƶ�����߳�
    std::thread videoProcessingThread(videoThread, videoFormatContext, videoCodecContext, videoStreamIndex);

    // �ȴ���Ƶ����Ƶ�߳�׼����
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return isAudioReady && isVideoReady; });
    }

    // ͬ���źţ���ʼ����
    {
        std::lock_guard<std::mutex> lock(mtx);
        isPlaybackFinished = true;
    }
    cv.notify_all();

    // ��ѭ��
    SDL_Event event;
    while (isPlaying) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isPlaying = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    isPaused = true;
                    while (isPaused) {
                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_QUIT) {
                                isPaused = false;
                                break;
                            }
                            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                                isPaused = false;
                                break;
                            }
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_r) {
                    std::cout << "���²���" << std::endl;
                    shouldVideoRestart = true;
                    shouldAudioRestart = true;
                } else if (event.key.keysym.sym == SDLK_q) {
                    isPlaying = false;
                }else if (event.key.keysym.sym == SDLK_RIGHT) {
                    std::cout << "ǰ��30s" << std::endl;
                    isVideoRight = true;
                    isAudioRight = true;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    std::cout << "����30s" << std::endl;
                    isVideoLeft = true;
                    isAudioLeft = true;
                }else if(event.key.keysym.sym == SDLK_UP){
                    std::cout << "��������" << std::endl;
                    isUp = true;
                }else if(event.key.keysym.sym == SDLK_DOWN){
                    std::cout << "��С����" << std::endl;
                    isDown = true;
                }
            }
        }
    }


    // �ȴ���Ƶ����Ƶ�����߳̽���
    audioProcessingThread.join();
    videoProcessingThread.join();

    // �ͷ���Դ
    avformat_close_input(&audioFormatContext);
    avformat_close_input(&videoFormatContext);
    avformat_free_context(audioFormatContext);
    avformat_free_context(videoFormatContext);
    avcodec_free_context(&audioCodecContext);
    avcodec_free_context(&videoCodecContext);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}