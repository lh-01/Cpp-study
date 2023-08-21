//#include <iostream>
//#include <thread>
//
//extern "C" {
//#include <SDL2/SDL.h>
//#include <al.h>
//#include <alc.h>
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libavutil/avutil.h>
//#include <libswresample/swresample.h>
//#include <libswscale/swscale.h>
//#include <libavutil/imgutils.h>
//}
//
//#define NUM_BUFFERS 3
//const int WIDTH = 640;
//const int HEIGHT = 480;
//
//int64_t currentTime = 0;
//
//// ��Ƶ�����߳�
//void audioPlayer(AVFormatContext* formatContext, AVCodecContext* codecContext, int audioStreamIndex,AVCodecParameters* audioCodecParameters) {
//    //  ��ʼ��SDL
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
//        std::cout << "�޷���ʼ��SDL" << std::endl;
//    }
//
//    // �������ں���Ⱦ��
//    SDL_Window* window = SDL_CreateWindow("Audio Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
//
//    // ��ʼ��OpenAL
//    ALCdevice* device = alcOpenDevice(nullptr);
//    if (device == nullptr) {
//        std::cout << "�޷�����Ƶ�豸" << std::endl;
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//    }
//
//    ALCcontext* context = alcCreateContext(device, nullptr);
//    if (context == nullptr) {
//        std::cout << "�޷�������Ƶ������" << std::endl;
//        alcCloseDevice(device);
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//    }
//    std::cout << "������Ƶ�����ĳɹ�" << std::endl;
//    ALint deviceSampleRate = 0;
//    alcGetIntegerv(device, ALC_FREQUENCY, sizeof(ALint), &deviceSampleRate);
//    std::cout << "�豸��Ĭ�ϲ�����Ϊ��" << deviceSampleRate << " Hz" << std::endl;
//
//
//    if (alcMakeContextCurrent(context) == ALC_FALSE) {
//        std::cout << "�޷�������Ƶ������" << std::endl;
//        alcDestroyContext(context);
//        alcCloseDevice(device);
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//    }
//
//    // ������ƵԴ����
//    ALuint source;
//    alGenSources(1, &source);
//
//    alSourcef(source, AL_PITCH, 1.0f);
//    alSourcef(source, AL_GAIN, 1.0f);
//    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
//    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
//    alSourcei(source, AL_LOOPING, AL_FALSE);
//
//    // ������Ƶ����������
//    ALuint buffers[NUM_BUFFERS];
//    alGenBuffers(NUM_BUFFERS, buffers);
//
//    bool isPlaying = true;
//    bool isPaused = false;
//    bool shouldRestart = false;
//
//    // ��ȡ��������Ƶ����
//    std::cout << "��ȡ��������Ƶ����" << std::endl;
//    AVPacket* packet = av_packet_alloc();
//    while (av_read_frame(formatContext, packet) >= 0) {
//        if (packet->stream_index == audioStreamIndex) {
//            AVStream* audioStream = formatContext->streams[audioStreamIndex];
//            // ��ȡ��Ƶ����ʱ���׼��������
//            AVRational timeBase = audioStream->time_base;
//            // ��ȡ��Ƶ����ʱ���pts ��ǰʱ��
//            currentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
//            // ����Ƶ���ݰ����͸�������
//            int sendResult = avcodec_send_packet(codecContext, packet);
//            if (sendResult < 0) {
//                char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                av_strerror(sendResult, errbuf, AV_ERROR_MAX_STRING_SIZE);
//                // std::cout << "�޷�������Ƶ���ݰ���������: " << errbuf << std::endl;
//                av_packet_unref(packet);
//                break;
//            }
//            // std::cout << "������Ƶ���ݰ����������ɹ�" << std::endl;
//            while (true) {
//                // ���ս�������Ƶ���ݰ�
//                AVFrame* frame = av_frame_alloc();
//                int receiveResult = avcodec_receive_frame(codecContext, frame);
//                if (receiveResult == AVERROR(EAGAIN)) {
//                    // ��������Ҫ������������
//                    av_frame_free(&frame);
//                    break;
//                } else if (receiveResult == AVERROR_EOF) {
//                    // �������Ѿ���������������
//                    av_frame_free(&frame);
//                    break;
//                } else if (receiveResult < 0) {
//                    char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                    av_strerror(receiveResult, errbuf, AV_ERROR_MAX_STRING_SIZE);
//                    std::cout << "�޷��ӽ�������������: " << errbuf << std::endl;
//                    av_packet_unref(packet);
//                    break;
//                }
//                // std::cout << "�ӽ������������ݳɹ�" << std::endl;
//                // ����Ŀ����Ƶ֡
//                AVFrame* targetFrame = av_frame_alloc();
//                targetFrame->format = AV_SAMPLE_FMT_S16;
//                targetFrame->channels = 2; // ˫����
//                targetFrame->channel_layout = av_get_default_channel_layout(2); // ˫������channel_layout
//                targetFrame->sample_rate = 44100;
//                targetFrame->nb_samples = frame->nb_samples;
//                av_frame_get_buffer(targetFrame, 0);
//
//                // ������Ƶת��������
//                SwrContext* swrContext = swr_alloc_set_opts(nullptr,
//                                                            targetFrame->channel_layout,
//                                                            AV_SAMPLE_FMT_S16,
//                                                            targetFrame->sample_rate,
//                                                            frame->channel_layout,
//                                                            static_cast<AVSampleFormat>(frame->format),
//                                                            frame->sample_rate,
//                                                            0,
//                                                            nullptr);
//                // ��ʼ����Ƶת��������
//                swr_init(swrContext);
//                // ������Ƶ���ݸ�ʽת��
//                swr_convert_frame(swrContext, targetFrame, frame);
//                // �ͷ���Դ
//                swr_free(&swrContext);
//
//                // ������Ƶ����
//                // ����Ƶ������䵽������
//                ALuint buffer;
//                alGenBuffers(1, &buffer);
//                alBufferData(buffer, AL_FORMAT_STEREO16, targetFrame->data[0], targetFrame->linesize[0], targetFrame->sample_rate);
//
//                // ����������ӵ���ƵԴ�Ļ���������
//                alSourceQueueBuffers(source, 1, &buffer);
//
//                // �����Ƶ����״̬
//                ALint state;
//                alGetSourcei(source, AL_SOURCE_STATE, &state);
//                if (state != AL_PLAYING) {
//                    // ��ʼ������Ƶ
//                    alSourcePlay(source);
//                }
//
//                // ��黺��������״̬
//                ALint queuedBuffers;
//                alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);
//
//                if (queuedBuffers >= NUM_BUFFERS) {
//                    // �����������������ȴ�����������
//                    while (queuedBuffers >= NUM_BUFFERS) {
//                        // �жϻ����������Ѵ���Ļ���������
//                        ALint processedBuffers;
//                        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processedBuffers);
//
//                        // ������Ѵ���Ļ��������ӻ������������Ƴ����ͷ�����
//                        if (processedBuffers > 0) {
//                            ALuint unqueuedBuffers[processedBuffers];
//                            alSourceUnqueueBuffers(source, processedBuffers, unqueuedBuffers);
//                            alDeleteBuffers(processedBuffers, unqueuedBuffers);
//                        }
//
//                        // ��黺��������״̬
//                        alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);
//                    }
//                }
//
//                // �ͷ���Ƶ֡
//                av_frame_free(&frame);
//                av_frame_free(&targetFrame);
//            }
//        }
//
//        // ��������¼�
//        SDL_Event event;
//        while (SDL_PollEvent(&event)) {
//            switch (event.type) {
//                case SDL_QUIT:
//                    isPlaying = false;
//                    break;
//                case SDL_KEYDOWN:
//                    if (event.key.keysym.sym == SDLK_SPACE) {
//                        std::cout << "���¿ո��" << std::endl;
//                        isPaused = !isPaused;
//                        if (!isPaused) {
//                            std::cout << "��������" << std::endl;
//                        }
//                    } else if (event.key.keysym.sym == SDLK_r) {
//                        std::cout << "����r" << std::endl;
//                        shouldRestart = true;
//                    } else if (event.key.keysym.sym == SDLK_q) {
//                        std::cout << "����q" << std::endl;
//                        isPlaying = false;
//                    }else if (event.key.keysym.sym == SDLK_RIGHT) {
//                        AVStream* audioStream = formatContext->streams[audioStreamIndex];
//                        // ��ȡ��Ƶ����ʱ���׼��������
//                        AVRational timeBase = audioStream->time_base;
//                        // ��ȡ��Ƶ����ʱ���pts ��ǰʱ��
//                        // int64_t currentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
//                        // std::cout << "currentTime: " << currentTime << " (" << timeBase.num << "/" << timeBase.den << ")" << std::endl;
//                        std::cout << "��ǰʱ��1��" << currentTime <<std::endl;
//                        int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        std::cout << "��תʱ��1��" << span <<std::endl;
//                        int64_t targetTime = currentTime + (30 * AV_TIME_BASE);  // ����30���ʱ����
//                        std::cout << "Ŀ��ʱ��1��" << targetTime <<std::endl;
//                        int64_t maxTime = audioStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        std::cout << "��ʱ��1��" << maxTime <<std::endl;
//
//                        if (targetTime >= maxTime) {
//                            targetTime = maxTime;
//                        }
//
//                        int seekFlags = AVSEEK_FLAG_BACKWARD;  // ʹ��AVSEEK_FLAG_BACKWARD��־��ȷ����ǰ��ת
//                        int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase); // ��timestamp�ĵ�λת��Ϊ��Ƶ����ʱ���׼
//                        int seekResult = av_seek_frame(formatContext, audioStreamIndex, timestamp, seekFlags);
//
//                        if (seekResult < 0) {
//                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                            av_strerror(seekResult, errbuf, sizeof(errbuf));
//                            std::cout << "����" << errbuf << std::endl;
//                        } else {
//                            // ���²�����صı���
//                            alSourceStop(source);  // ֹͣ��ǰ��Ƶ
//                            alSourceRewind(source);  // ������Ƶ����λ��
//                            currentTime = targetTime;  // ���µ�ǰʱ��
//                            alSourcePlay(source);  // ������Ƶ
//                        }
//
//                        int64_t currentTimeSec = currentTime / AV_TIME_BASE; // ��currentTime�ĵ�λת��Ϊ��
//                        int64_t spanSec = span / AV_TIME_BASE; // ��span�ĵ�λת��Ϊ��
//                        int64_t targetTimeSec = targetTime / AV_TIME_BASE; // Ŀ��ʱ�������
//
//                        int64_t maxTimeSec = maxTime / AV_TIME_BASE; // ��maxTime�ĵ�λת��Ϊ��
//
//                        std::cout << "��ǰʱ�䣺" << currentTimeSec<< "��" << std::endl;
//                        std::cout << "��תʱ�䣺" << spanSec << "��" << std::endl;
//                        std::cout << "Ŀ��ʱ�䣺" << targetTimeSec << "��" << std::endl;
//
//                        std::cout << "��ʱ�䣺" << maxTimeSec << "��" << std::endl;
//
//
//                    } else if (event.key.keysym.sym == SDLK_LEFT) {
//                        AVStream* audioStream = formatContext->streams[audioStreamIndex];
//                        AVRational timeBase = audioStream->time_base;
//                        std::cout << "��ǰʱ��1��" << currentTime <<std::endl;
//                        int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        std::cout << "��תʱ��1��" << span <<std::endl;
//
//                        int64_t targetTime = currentTime - span;  // ��ȥ30���ʱ����
//                        std::cout << "Ŀ��ʱ��1��" << targetTime <<std::endl;
//
//                        if (targetTime < 0) {
//                            targetTime = 0;  // ��������ת����ʱ��
//                        }
//                        int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase); // ��timestamp�ĵ�λת��Ϊ��Ƶ����ʱ���׼
//
//                        int seekFlags = AVSEEK_FLAG_BACKWARD;  // ʹ��AVSEEK_FLAG_BACKWARD��־��ȷ����ǰ��ת
//
//                        int seekResult = av_seek_frame(formatContext, audioStreamIndex, timestamp, seekFlags);
//
//                        if (seekResult < 0) {
//                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                            av_strerror(seekResult, errbuf, sizeof(errbuf));
//                            std::cout << "����" << errbuf << std::endl;
//                        } else {
//                            // ���²�����صı���
//                            alSourceStop(source);  // ֹͣ��ǰ��Ƶ
//                            alSourceRewind(source);  // ������Ƶ����λ��
//                            currentTime = targetTime;  // ���µ�ǰʱ��
//                            alSourcePlay(source);  // ������Ƶ
//                        }
//
//                        int64_t currentTimeSec = currentTime / AV_TIME_BASE; // ��currentTime�ĵ�λת��Ϊ��
//                        int64_t spanSec = span / AV_TIME_BASE; // ��span�ĵ�λת��Ϊ��
//                        int64_t targetTimeSec = targetTime / AV_TIME_BASE; // Ŀ��ʱ�������
//
//                        int64_t maxTime = audioStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        int64_t maxTimeSec = maxTime / AV_TIME_BASE; // ��maxTime�ĵ�λת��Ϊ��
//
//                        std::cout << "��ǰʱ�䣺" << currentTimeSec<< "��" << std::endl;
//                        std::cout << "��תʱ�䣺" << spanSec << "��" << std::endl;
//                        std::cout << "Ŀ��ʱ�䣺" << targetTimeSec << "��" << std::endl;
//
//                        std::cout << "��ʱ�䣺" << maxTimeSec << "��" << std::endl;
//                    }else if(event.key.keysym.sym == SDLK_UP){
//                        std::cout << "��������" << std::endl;
//                        // ��������
//                        float gain;
//                        alGetSourcef(source, AL_GAIN, &gain);
//                        gain = std::min(gain + 0.1f, 1.0f);
//                        alSourcef(source, AL_GAIN, gain);
//                    }else if(event.key.keysym.sym == SDLK_DOWN){
//                        std::cout << "��С����" << std::endl;
//                        // ��С����
//                        float gain;
//                        alGetSourcef(source, AL_GAIN, &gain);
//                        gain = std::max(gain - 0.1f, 0.0f);
//                        alSourcef(source, AL_GAIN, gain);
//                    }
//
//                    break;
//            }
//        }
//
//        if (!isPlaying) {
//            std::cout << "�˳�����" << std::endl;
//            break;
//        }
//
//        if (shouldRestart) {
//            // ���²���
//            std::cout << "���²���" << std::endl;
//            av_seek_frame(formatContext, audioStreamIndex, 0, AVSEEK_FLAG_FRAME);
//            shouldRestart = false;
//            isPaused = false;
//        }
//        if (isPaused) {
//            // ��ͣ
//            std::cout << "��ͣ����" << std::endl;
//            continue;
//        }
//
//    }
//
//
//    // av_packet_free(&packet);
//
//    // �ȴ���Ƶ�������
//    ALint state;
//    do {
//        alGetSourcei(source, AL_SOURCE_STATE, &state);
//    } while (state == AL_PLAYING);
//
//    std::cout<< "�������" <<std::endl;
//
//    // ֹͣ������Ƶ
//    alSourceStop(source);
//
//    // ��ջ���������
//    ALint queuedBuffers;
//    alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);
//    if (queuedBuffers > 0) {
//        alSourceUnqueueBuffers(source, std::min(queuedBuffers, NUM_BUFFERS), buffers);
//    }
//    // �ͷ���Դ
//    av_packet_free(&packet);
//    // �ͷ�OpenAL��Դ
//    alDeleteSources(1, &source);
//    alDeleteBuffers(NUM_BUFFERS, buffers);
//    // �ر�OpenAL�����ĺ��豸
//    alcMakeContextCurrent(nullptr);
//    alcDestroyContext(context);
//    alcCloseDevice(device);
//
//}
//
//int main(int argc, char* args[]) {
//    // ��ʼ��FFmpeg
//    avformat_network_init();
//    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
//
//    const char* filename = "E:/byTask/source/input.mp4";
//
//    // ����Ƶ�ļ�
//    AVFormatContext* formatContext = avformat_alloc_context();
//    if (avformat_open_input(&formatContext, filename, NULL, NULL) != 0) {
//        std::cout << "�޷�����Ƶ�ļ�" << std::endl;
//        return -1;
//    }
//
//    std::cout << "����Ƶ�ļ��ɹ� " << std::endl;
//
//    // ������Ƶ����Ϣ
//    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
//        std::cerr << "�޷���������Ϣ" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // ������Ƶ����Ƶ��
//    int audioStreamIndex = -1;
//    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
//        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex < 0) {
//            audioStreamIndex = i;
//        }
//    }
//
//    // �����Ƶ����Ƶ���Ƿ����
//    if (audioStreamIndex < 0 ) {
//        std::cout << "��Ƶ��δ�ҵ�" << std::endl;
//        return -1;
//    }
//
//    std::cout << "��Ƶ������: " << audioStreamIndex << std::endl;
//
//    // ��ȡ��Ƶ����������
//    AVCodecParameters* audioCodecParameters = formatContext->streams[audioStreamIndex]->codecpar;
//    // ��ȡ��Ƶ������
//    AVCodec* audioCodec = const_cast<AVCodec *>(avcodec_find_decoder(audioCodecParameters->codec_id));
//    if (audioCodec == NULL) {
//        std::cout << "�Ҳ�����Ƶ������" << std::endl;
//        return -1;
//    }
//    // ��ʼ����Ƶ������
//    // ����������������
//    AVCodecContext* audioCodecContext = avcodec_alloc_context3(audioCodec);
//    // ���ý���������
//    avcodec_parameters_to_context(audioCodecContext, audioCodecParameters);
//    // �򿪽�����
//    if (avcodec_open2(audioCodecContext, audioCodec, NULL) != 0) {
//        std::cout << "�޷�����Ƶ������" << std::endl;
//        return -1;
//    }
//    std::cout << "����Ƶ�������ɹ�" << std::endl;
//    // ��ȡ��Ƶ����������
//    int sampleRate = audioCodecContext->sample_rate;
//    int channels = audioCodecContext->channels;
//    AVSampleFormat sampleFormat = audioCodecContext->sample_fmt;
//
//    // �����Ƶ��ʽ��Ϣ
//    std::cout << "������: " << sampleRate << " Hz" << std::endl;
//    std::cout << "ͨ����: " << channels << std::endl;
//    std::cout << "��Ƶ��ʽ: " << av_get_sample_fmt_name(sampleFormat) << std::endl;
//
//
//    AVStream* audioStream = formatContext->streams[audioStreamIndex];
//    AVRational timeBase = audioStream->time_base;
//    std::cout << timeBase.num <<std::endl;
//    std::cout << timeBase.den <<std::endl;
//    if (timeBase.num == 1 && timeBase.den == AV_TIME_BASE) {
//        std::cout << "ʱ�䵥λΪ��" <<std::endl;
//    }
//
//    // ��Ƶ����
//    audioPlayer(formatContext, audioCodecContext, audioStreamIndex, audioCodecParameters);
//
//    // �ͷ���Դ
//    avformat_close_input(&formatContext);
//    avcodec_free_context(&audioCodecContext);
//
//    return 0;
//}