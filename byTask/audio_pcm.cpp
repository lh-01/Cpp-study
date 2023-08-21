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
//    // 初始化FFmpeg
//    avformat_network_init();
//
//    const char* filename = "E:/byTask/source/output.pcm";
//    std::cout << "文件路径: " << filename << std::endl;
//
//
//    // 创建PCM输入格式
//    AVInputFormat* inputFormat = const_cast<AVInputFormat *>(av_find_input_format("s16le"));
//    if (!inputFormat) {
//        std::cout << "无法找到PCM输入格式" << std::endl;
//        return -1;
//    }
//    std::cout << "找到PCM输入格式 " << std::endl;
//
//    AVFormatContext* formatContext = avformat_alloc_context();
//    if (avformat_open_input(&formatContext, filename, inputFormat, NULL) != 0) {
//        std::cout << "无法打开音频文件" << std::endl;
//        return -1;
//    }
//    std::cout << "打开音频文件成功 " << std::endl;
//
//    // 获取音频流信息
//    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
//        std::cout << "无法获取音频流信息" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // 查找音频流索引
//    int audioStreamIndex = -1;
//    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
//        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
//            audioStreamIndex = i;
//            break;
//        }
//    }
//
//    if (audioStreamIndex == -1) {
//        std::cout << "找不到音频流" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // 获取音频解码器参数
//    AVCodecParameters* codecParameters = formatContext->streams[audioStreamIndex]->codecpar;
//
//    // 查找音频解码器
//    AVCodec* codec = const_cast<AVCodec *>(avcodec_find_decoder(codecParameters->codec_id));
//    if (codec == nullptr) {
//        std::cout << "找不到音频解码器" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // 创建解码器上下文
//    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
//    if (codecContext == nullptr) {
//        std::cout << "无法创建解码器上下文" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // 设置解码器参数
//    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
//        std::cout << "无法设置解码器参数" << std::endl;
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // 打开解码器
//    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
//        std::cout << "无法打开解码器" << std::endl;
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//    std::cout << "打开解码器成功" << std::endl;
//
//    // 初始化OpenAL
//    ALCdevice* device = alcOpenDevice(nullptr);
//    if (device == nullptr) {
//        std::cout << "无法打开音频设备" << std::endl;
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    ALCcontext* context = alcCreateContext(device, nullptr);
//    if (context == nullptr) {
//        std::cout << "无法创建音频上下文" << std::endl;
//        alcCloseDevice(device);
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//    std::cout << "创建音频上下文成功" << std::endl;
//    ALint deviceSampleRate = 0;
//    alcGetIntegerv(device, ALC_FREQUENCY, sizeof(ALint), &deviceSampleRate);
//    std::cout << "设备的默认采样率为：" << deviceSampleRate << " Hz" << std::endl;
//
//
//    if (alcMakeContextCurrent(context) == ALC_FALSE) {
//        std::cout << "无法设置音频上下文" << std::endl;
//        alcDestroyContext(context);
//        alcCloseDevice(device);
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // 创建音频源对象
//    ALuint source;
//    alGenSources(1, &source);
//
//
//    // 创建音频缓冲区对象
//    ALuint buffers[NUM_BUFFERS];
//    alGenBuffers(NUM_BUFFERS, buffers);
//
//    // 设置音频数据的格式
//    ALenum format;
//    if (codecParameters->channels == 1) {
//        format = AL_FORMAT_MONO16;
//    } else if (codecParameters->channels == 2) {
//        format = AL_FORMAT_STEREO16;
//    } else {
//        std::cout << "不支持的音频通道数" << std::endl;
//        alcMakeContextCurrent(nullptr);
//        alcDestroyContext(context);
//        alcCloseDevice(device);
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // 读取并播放音频数据
//    std::cout << "读取并播放音频数据" << std::endl;
//    AVPacket* packet = av_packet_alloc();
//    while (av_read_frame(formatContext, packet) >= 0) {
//        if (packet->stream_index == audioStreamIndex) {
//            // 将音频数据包发送给解码器
//            int sendResult = avcodec_send_packet(codecContext, packet);
//            if (sendResult < 0) {
//                char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                av_strerror(sendResult, errbuf, AV_ERROR_MAX_STRING_SIZE);
//                std::cout << "无法发送音频数据包到解码器: " << errbuf << std::endl;
//                av_packet_unref(packet);
//                break;
//            }
//            std::cout << "发送音频数据包到解码器成功" << std::endl;
//            while (true) {
//                // 接收解码后的音频数据包
//                AVFrame* frame = av_frame_alloc();
//                int receiveResult = avcodec_receive_frame(codecContext, frame);
//                if (receiveResult == AVERROR(EAGAIN)) {
//                    // 解码器需要更多输入数据
//                    av_frame_free(&frame);
//                    break;
//                } else if (receiveResult == AVERROR_EOF) {
//                    // 解码器已经处理完所有数据
//                    av_frame_free(&frame);
//                    break;
//                } else if (receiveResult < 0) {
//                    char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                    av_strerror(receiveResult, errbuf, AV_ERROR_MAX_STRING_SIZE);
//                    std::cout << "无法从解码器接收数据: " << errbuf << std::endl;
//                    av_packet_unref(packet);
//                    break;
//                }
//                std::cout << "从解码器接收数据成功" << std::endl;
//                // 检查音频数据格式
//                if (frame->format != AV_SAMPLE_FMT_S16) {
//                    std::cout << "音频数据格式不支持。请将解码后的音频数据转换为AV_SAMPLE_FMT_S16格式。" << std::endl;
//                    av_frame_free(&frame);
//                    continue;
//                }
//
//
//                // 播放解码后的音频数据
//                // 将音频数据填充到缓冲区
//                ALuint buffer;
//                alGenBuffers(1, &buffer);
//                alBufferData(buffer, AL_FORMAT_STEREO16, frame->data[0], frame->linesize[0], frame->sample_rate);
//
//                // 将缓冲区添加到音频源的缓冲区队列
//                alSourceQueueBuffers(source, 1, &buffer);
//
//                // 检查音频播放状态
//                ALint state;
//                alGetSourcei(source, AL_SOURCE_STATE, &state);
//                if (state != AL_PLAYING) {
//                    // 开始播放音频
//                    alSourcePlay(source);
//                }
//
//                // 检查缓冲区队列状态
//                ALint queuedBuffers;
//                alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);
//
//                if (queuedBuffers >= NUM_BUFFERS) {
//                    // 缓冲区队列已满，等待缓冲区处理
//                    while (queuedBuffers >= NUM_BUFFERS) {
//                        // 判断缓冲区队列已处理的缓冲区数量
//                        ALint processedBuffers;
//                        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processedBuffers);
//
//                        // 如果有已处理的缓冲区，从缓冲区队列中移除并释放它们
//                        if (processedBuffers > 0) {
//                            ALuint unqueuedBuffers[processedBuffers];
//                            alSourceUnqueueBuffers(source, processedBuffers, unqueuedBuffers);
//                            alDeleteBuffers(processedBuffers, unqueuedBuffers);
//                        }
//
//                        // 检查缓冲区队列状态
//                        alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);
//                    }
//                }
//
////                // 检查缓冲区队列剩余的缓冲区数量
////                if (queuedBuffers < 3) {
////                    // 从解码器中接收更多音频数据
////                    continue;
////                }
//
////                // 判断缓冲区队列已处理的缓冲区数量
////                ALint processedBuffers;
////                alGetSourcei(source, AL_BUFFERS_PROCESSED, &processedBuffers);
////
////                // 如果有已处理的缓冲区，从缓冲区队列中移除并释放它们
////                if (processedBuffers > 0) {
////                    ALuint unqueuedBuffers[processedBuffers];
////                    alSourceUnqueueBuffers(source, processedBuffers, unqueuedBuffers);
////                    alDeleteBuffers(processedBuffers, unqueuedBuffers);
////                }
//
//                // 释放音频帧
//                av_frame_free(&frame);
//            }
//        }
//    }
//
//    // 等待音频播放完毕
//    ALint state;
//    do {
//        alGetSourcei(source, AL_SOURCE_STATE, &state);
//    } while (state == AL_PLAYING);
//
//    std::cout<< "播放完成" <<std::endl;
//
//    // 停止播放音频
//    alSourceStop(source);
//
//    // 清空缓冲区队列
//    ALint queuedBuffers;
//    alGetSourcei(source, AL_BUFFERS_QUEUED, &queuedBuffers);
//    if (queuedBuffers > 0) {
//        alSourceUnqueueBuffers(source, std::min(queuedBuffers, NUM_BUFFERS), buffers);
//    }
//    // 释放资源
//    av_packet_free(&packet);
//    // 释放OpenAL资源
//    alDeleteSources(1, &source);
//    alDeleteBuffers(NUM_BUFFERS, buffers);
//    // 关闭OpenAL上下文和设备
//    alcMakeContextCurrent(nullptr);
//    alcDestroyContext(context);
//    alcCloseDevice(device);
//    // 关闭解码器
//    avcodec_close(codecContext);
//    // 释放解码器上下文和音频文件上下文
//    avcodec_free_context(&codecContext);
//    avformat_close_input(&formatContext);
//
//    return 0;
//}