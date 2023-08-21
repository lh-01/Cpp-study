//#include <iostream>
//#include <chrono>
//#include <thread>
//#include <cstring>
//#include <al.h>
//#include <alc.h>
//#include <synchapi.h>
//
//
//extern "C" {
//#include <libavformat/avformat.h>
//#include <libavutil/avutil.h>
//#include <libavcodec/avcodec.h>
//#include <libswresample/swresample.h>
//}
//
//#define NUM_BUFFERS 3
//#define BUFFER_SIZE 2048
//
//int main() {
//    // ��ʼ��FFmpeg
//    avformat_network_init();
//
//    const char* filename = "E:/byTask/source/output.pcm";
//    std::cout << "�ļ�·��: " << filename << std::endl;
//
//
//    // ����PCM�����ʽ
//    AVInputFormat* inputFormat = const_cast<AVInputFormat *>(av_find_input_format("s16le"));
//    if (!inputFormat) {
//        std::cout << "�޷��ҵ�PCM�����ʽ" << std::endl;
//        return -1;
//    }
//    std::cout << "�ҵ�PCM�����ʽ " << std::endl;
//
//    AVFormatContext* formatContext = avformat_alloc_context();
//    if (avformat_open_input(&formatContext, filename, inputFormat, NULL) != 0) {
//        std::cout << "�޷�����Ƶ�ļ�" << std::endl;
//        return -1;
//    }
//    std::cout << "����Ƶ�ļ��ɹ� " << std::endl;
//
//    // ��ȡ��Ƶ����Ϣ
//    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
//        std::cout << "�޷���ȡ��Ƶ����Ϣ" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // ������Ƶ������
//    int audioStreamIndex = -1;
//    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
//        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
//            audioStreamIndex = i;
//            break;
//        }
//    }
//
//    if (audioStreamIndex == -1) {
//        std::cout << "�Ҳ�����Ƶ��" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // ��ȡ��Ƶ����������
//    AVCodecParameters* codecParameters = formatContext->streams[audioStreamIndex]->codecpar;
//
//    // ������Ƶ������
//    AVCodec* codec = const_cast<AVCodec *>(avcodec_find_decoder(codecParameters->codec_id));
//    if (codec == nullptr) {
//        std::cout << "�Ҳ�����Ƶ������" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // ����������������
//    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
//    if (codecContext == nullptr) {
//        std::cout << "�޷�����������������" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // ���ý���������
//    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
//        std::cout << "�޷����ý���������" << std::endl;
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // �򿪽�����
//    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
//        std::cout << "�޷��򿪽�����" << std::endl;
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//    std::cout << "�򿪽������ɹ�" << std::endl;
//
//    // ��ʼ��OpenAL
//    ALCdevice* device = alcOpenDevice(nullptr);
//    if (device == nullptr) {
//        std::cout << "�޷�����Ƶ�豸" << std::endl;
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    ALCcontext* context = alcCreateContext(device, nullptr);
//    if (context == nullptr) {
//        std::cout << "�޷�������Ƶ������" << std::endl;
//        alcCloseDevice(device);
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
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
//        return -1;
//    }
//
//    // ������ƵԴ����
//    ALuint source;
//    alGenSources(1, &source);
//
//
//    // ������Ƶ����������
//    ALuint buffers[NUM_BUFFERS];
//    alGenBuffers(NUM_BUFFERS, buffers);
//
//    // ������Ƶ���ݵĸ�ʽ
//    ALenum format;
//    if (codecParameters->channels == 1) {
//        format = AL_FORMAT_MONO16;
//    } else if (codecParameters->channels == 2) {
//        format = AL_FORMAT_STEREO16;
//    } else {
//        std::cout << "��֧�ֵ���Ƶͨ����" << std::endl;
//        alcMakeContextCurrent(nullptr);
//        alcDestroyContext(context);
//        alcCloseDevice(device);
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // ��ȡ��������Ƶ����
//    std::cout << "��ȡ��������Ƶ����" << std::endl;
//    AVPacket* packet = av_packet_alloc();
//    while (av_read_frame(formatContext, packet) >= 0) {
//        if (packet->stream_index == audioStreamIndex) {
//            // ����Ƶ���ݰ����͸�������
//            int sendResult = avcodec_send_packet(codecContext, packet);
//            if (sendResult < 0) {
//                char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                av_strerror(sendResult, errbuf, AV_ERROR_MAX_STRING_SIZE);
//                std::cout << "�޷�������Ƶ���ݰ���������: " << errbuf << std::endl;
//                av_packet_unref(packet);
//                break;
//            }
//            std::cout << "������Ƶ���ݰ����������ɹ�" << std::endl;
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
//                std::cout << "�ӽ������������ݳɹ�" << std::endl;
//                // �����Ƶ���ݸ�ʽ
//                if (frame->format != AV_SAMPLE_FMT_S16) {
//                    std::cout << "��Ƶ���ݸ�ʽ��֧�֡��뽫��������Ƶ����ת��ΪAV_SAMPLE_FMT_S16��ʽ��" << std::endl;
//                    av_frame_free(&frame);
//                    continue;
//                }
//
//
//                // ���Ž�������Ƶ����
//                // ����Ƶ������䵽������
//                ALuint buffer;
//                alGenBuffers(1, &buffer);
//                alBufferData(buffer, AL_FORMAT_STEREO16, frame->data[0], frame->linesize[0], frame->sample_rate);
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
////                // ��黺��������ʣ��Ļ���������
////                if (queuedBuffers < 3) {
////                    // �ӽ������н��ո�����Ƶ����
////                    continue;
////                }
//
////                // �жϻ����������Ѵ���Ļ���������
////                ALint processedBuffers;
////                alGetSourcei(source, AL_BUFFERS_PROCESSED, &processedBuffers);
////
////                // ������Ѵ���Ļ��������ӻ������������Ƴ����ͷ�����
////                if (processedBuffers > 0) {
////                    ALuint unqueuedBuffers[processedBuffers];
////                    alSourceUnqueueBuffers(source, processedBuffers, unqueuedBuffers);
////                    alDeleteBuffers(processedBuffers, unqueuedBuffers);
////                }
//
//                // �ͷ���Ƶ֡
//                av_frame_free(&frame);
//            }
//        }
//    }
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
//    // �رս�����
//    avcodec_close(codecContext);
//    // �ͷŽ����������ĺ���Ƶ�ļ�������
//    avcodec_free_context(&codecContext);
//    avformat_close_input(&formatContext);
//
//    return 0;
//}