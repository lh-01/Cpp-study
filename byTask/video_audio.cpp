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
//// 音频处理线程
//void audioPlayer(AVFormatContext* formatContext, AVCodecContext* codecContext, int audioStreamIndex,AVCodecParameters* audioCodecParameters) {
//    //  初始化SDL
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
//        std::cout << "无法初始化SDL" << std::endl;
//    }
//
//    // 创建窗口和渲染器
//    SDL_Window* window = SDL_CreateWindow("Audio Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
//
//    // 初始化OpenAL
//    ALCdevice* device = alcOpenDevice(nullptr);
//    if (device == nullptr) {
//        std::cout << "无法打开音频设备" << std::endl;
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
//    }
//
//    ALCcontext* context = alcCreateContext(device, nullptr);
//    if (context == nullptr) {
//        std::cout << "无法创建音频上下文" << std::endl;
//        alcCloseDevice(device);
//        avcodec_close(codecContext);
//        avcodec_free_context(&codecContext);
//        avformat_close_input(&formatContext);
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
//    }
//
//    // 创建音频源对象
//    ALuint source;
//    alGenSources(1, &source);
//
//    alSourcef(source, AL_PITCH, 1.0f);
//    alSourcef(source, AL_GAIN, 1.0f);
//    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
//    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
//    alSourcei(source, AL_LOOPING, AL_FALSE);
//
//    // 创建音频缓冲区对象
//    ALuint buffers[NUM_BUFFERS];
//    alGenBuffers(NUM_BUFFERS, buffers);
//
//    bool isPlaying = true;
//    bool isPaused = false;
//    bool shouldRestart = false;
//
//    // 读取并播放音频数据
//    std::cout << "读取并播放音频数据" << std::endl;
//    AVPacket* packet = av_packet_alloc();
//    while (av_read_frame(formatContext, packet) >= 0) {
//        if (packet->stream_index == audioStreamIndex) {
//            AVStream* audioStream = formatContext->streams[audioStreamIndex];
//            // 获取音频流的时间基准和样本率
//            AVRational timeBase = audioStream->time_base;
//            // 获取音频包的时间戳pts 当前时间
//            currentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
//            // 将音频数据包发送给解码器
//            int sendResult = avcodec_send_packet(codecContext, packet);
//            if (sendResult < 0) {
//                char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                av_strerror(sendResult, errbuf, AV_ERROR_MAX_STRING_SIZE);
//                // std::cout << "无法发送音频数据包到解码器: " << errbuf << std::endl;
//                av_packet_unref(packet);
//                break;
//            }
//            // std::cout << "发送音频数据包到解码器成功" << std::endl;
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
//                // std::cout << "从解码器接收数据成功" << std::endl;
//                // 创建目标音频帧
//                AVFrame* targetFrame = av_frame_alloc();
//                targetFrame->format = AV_SAMPLE_FMT_S16;
//                targetFrame->channels = 2; // 双声道
//                targetFrame->channel_layout = av_get_default_channel_layout(2); // 双声道的channel_layout
//                targetFrame->sample_rate = 44100;
//                targetFrame->nb_samples = frame->nb_samples;
//                av_frame_get_buffer(targetFrame, 0);
//
//                // 创建音频转换上下文
//                SwrContext* swrContext = swr_alloc_set_opts(nullptr,
//                                                            targetFrame->channel_layout,
//                                                            AV_SAMPLE_FMT_S16,
//                                                            targetFrame->sample_rate,
//                                                            frame->channel_layout,
//                                                            static_cast<AVSampleFormat>(frame->format),
//                                                            frame->sample_rate,
//                                                            0,
//                                                            nullptr);
//                // 初始化音频转换上下文
//                swr_init(swrContext);
//                // 进行音频数据格式转换
//                swr_convert_frame(swrContext, targetFrame, frame);
//                // 释放资源
//                swr_free(&swrContext);
//
//                // 播放音频数据
//                // 将音频数据填充到缓冲区
//                ALuint buffer;
//                alGenBuffers(1, &buffer);
//                alBufferData(buffer, AL_FORMAT_STEREO16, targetFrame->data[0], targetFrame->linesize[0], targetFrame->sample_rate);
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
//                // 释放音频帧
//                av_frame_free(&frame);
//                av_frame_free(&targetFrame);
//            }
//        }
//
//        // 处理键盘事件
//        SDL_Event event;
//        while (SDL_PollEvent(&event)) {
//            switch (event.type) {
//                case SDL_QUIT:
//                    isPlaying = false;
//                    break;
//                case SDL_KEYDOWN:
//                    if (event.key.keysym.sym == SDLK_SPACE) {
//                        std::cout << "按下空格键" << std::endl;
//                        isPaused = !isPaused;
//                        if (!isPaused) {
//                            std::cout << "继续播放" << std::endl;
//                        }
//                    } else if (event.key.keysym.sym == SDLK_r) {
//                        std::cout << "按下r" << std::endl;
//                        shouldRestart = true;
//                    } else if (event.key.keysym.sym == SDLK_q) {
//                        std::cout << "按下q" << std::endl;
//                        isPlaying = false;
//                    }else if (event.key.keysym.sym == SDLK_RIGHT) {
//                        AVStream* audioStream = formatContext->streams[audioStreamIndex];
//                        // 获取音频流的时间基准和样本率
//                        AVRational timeBase = audioStream->time_base;
//                        // 获取音频包的时间戳pts 当前时间
//                        // int64_t currentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
//                        // std::cout << "currentTime: " << currentTime << " (" << timeBase.num << "/" << timeBase.den << ")" << std::endl;
//                        std::cout << "当前时间1：" << currentTime <<std::endl;
//                        int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        std::cout << "跳转时间1：" << span <<std::endl;
//                        int64_t targetTime = currentTime + (30 * AV_TIME_BASE);  // 增加30秒的时间跨度
//                        std::cout << "目标时间1：" << targetTime <<std::endl;
//                        int64_t maxTime = audioStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        std::cout << "总时间1：" << maxTime <<std::endl;
//
//                        if (targetTime >= maxTime) {
//                            targetTime = maxTime;
//                        }
//
//                        int seekFlags = AVSEEK_FLAG_BACKWARD;  // 使用AVSEEK_FLAG_BACKWARD标志，确保向前跳转
//                        int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase); // 将timestamp的单位转换为音频流的时间基准
//                        int seekResult = av_seek_frame(formatContext, audioStreamIndex, timestamp, seekFlags);
//
//                        if (seekResult < 0) {
//                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                            av_strerror(seekResult, errbuf, sizeof(errbuf));
//                            std::cout << "错误：" << errbuf << std::endl;
//                        } else {
//                            // 更新播放相关的变量
//                            alSourceStop(source);  // 停止当前音频
//                            alSourceRewind(source);  // 重置音频播放位置
//                            currentTime = targetTime;  // 更新当前时间
//                            alSourcePlay(source);  // 播放音频
//                        }
//
//                        int64_t currentTimeSec = currentTime / AV_TIME_BASE; // 将currentTime的单位转换为秒
//                        int64_t spanSec = span / AV_TIME_BASE; // 将span的单位转换为秒
//                        int64_t targetTimeSec = targetTime / AV_TIME_BASE; // 目标时间的秒数
//
//                        int64_t maxTimeSec = maxTime / AV_TIME_BASE; // 将maxTime的单位转换为秒
//
//                        std::cout << "当前时间：" << currentTimeSec<< "秒" << std::endl;
//                        std::cout << "跳转时间：" << spanSec << "秒" << std::endl;
//                        std::cout << "目标时间：" << targetTimeSec << "秒" << std::endl;
//
//                        std::cout << "总时间：" << maxTimeSec << "秒" << std::endl;
//
//
//                    } else if (event.key.keysym.sym == SDLK_LEFT) {
//                        AVStream* audioStream = formatContext->streams[audioStreamIndex];
//                        AVRational timeBase = audioStream->time_base;
//                        std::cout << "当前时间1：" << currentTime <<std::endl;
//                        int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        std::cout << "跳转时间1：" << span <<std::endl;
//
//                        int64_t targetTime = currentTime - span;  // 减去30秒的时间跨度
//                        std::cout << "目标时间1：" << targetTime <<std::endl;
//
//                        if (targetTime < 0) {
//                            targetTime = 0;  // 不允许跳转到负时间
//                        }
//                        int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase); // 将timestamp的单位转换为音频流的时间基准
//
//                        int seekFlags = AVSEEK_FLAG_BACKWARD;  // 使用AVSEEK_FLAG_BACKWARD标志，确保向前跳转
//
//                        int seekResult = av_seek_frame(formatContext, audioStreamIndex, timestamp, seekFlags);
//
//                        if (seekResult < 0) {
//                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                            av_strerror(seekResult, errbuf, sizeof(errbuf));
//                            std::cout << "错误：" << errbuf << std::endl;
//                        } else {
//                            // 更新播放相关的变量
//                            alSourceStop(source);  // 停止当前音频
//                            alSourceRewind(source);  // 重置音频播放位置
//                            currentTime = targetTime;  // 更新当前时间
//                            alSourcePlay(source);  // 播放音频
//                        }
//
//                        int64_t currentTimeSec = currentTime / AV_TIME_BASE; // 将currentTime的单位转换为秒
//                        int64_t spanSec = span / AV_TIME_BASE; // 将span的单位转换为秒
//                        int64_t targetTimeSec = targetTime / AV_TIME_BASE; // 目标时间的秒数
//
//                        int64_t maxTime = audioStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        int64_t maxTimeSec = maxTime / AV_TIME_BASE; // 将maxTime的单位转换为秒
//
//                        std::cout << "当前时间：" << currentTimeSec<< "秒" << std::endl;
//                        std::cout << "跳转时间：" << spanSec << "秒" << std::endl;
//                        std::cout << "目标时间：" << targetTimeSec << "秒" << std::endl;
//
//                        std::cout << "总时间：" << maxTimeSec << "秒" << std::endl;
//                    }else if(event.key.keysym.sym == SDLK_UP){
//                        std::cout << "增加音量" << std::endl;
//                        // 增加音量
//                        float gain;
//                        alGetSourcef(source, AL_GAIN, &gain);
//                        gain = std::min(gain + 0.1f, 1.0f);
//                        alSourcef(source, AL_GAIN, gain);
//                    }else if(event.key.keysym.sym == SDLK_DOWN){
//                        std::cout << "减小音量" << std::endl;
//                        // 减小音量
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
//            std::cout << "退出播放" << std::endl;
//            break;
//        }
//
//        if (shouldRestart) {
//            // 重新播放
//            std::cout << "重新播放" << std::endl;
//            av_seek_frame(formatContext, audioStreamIndex, 0, AVSEEK_FLAG_FRAME);
//            shouldRestart = false;
//            isPaused = false;
//        }
//        if (isPaused) {
//            // 暂停
//            std::cout << "暂停播放" << std::endl;
//            continue;
//        }
//
//    }
//
//
//    // av_packet_free(&packet);
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
//
//}
//
//int main(int argc, char* args[]) {
//    // 初始化FFmpeg
//    avformat_network_init();
//    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
//
//    const char* filename = "E:/byTask/source/input.mp4";
//
//    // 打开视频文件
//    AVFormatContext* formatContext = avformat_alloc_context();
//    if (avformat_open_input(&formatContext, filename, NULL, NULL) != 0) {
//        std::cout << "无法打开视频文件" << std::endl;
//        return -1;
//    }
//
//    std::cout << "打开视频文件成功 " << std::endl;
//
//    // 检索视频流信息
//    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
//        std::cerr << "无法检索流信息" << std::endl;
//        avformat_close_input(&formatContext);
//        return -1;
//    }
//
//    // 查找音频和视频流
//    int audioStreamIndex = -1;
//    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
//        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex < 0) {
//            audioStreamIndex = i;
//        }
//    }
//
//    // 检查音频和视频流是否存在
//    if (audioStreamIndex < 0 ) {
//        std::cout << "音频流未找到" << std::endl;
//        return -1;
//    }
//
//    std::cout << "音频流索引: " << audioStreamIndex << std::endl;
//
//    // 获取音频解码器参数
//    AVCodecParameters* audioCodecParameters = formatContext->streams[audioStreamIndex]->codecpar;
//    // 获取音频解码器
//    AVCodec* audioCodec = const_cast<AVCodec *>(avcodec_find_decoder(audioCodecParameters->codec_id));
//    if (audioCodec == NULL) {
//        std::cout << "找不到音频解码器" << std::endl;
//        return -1;
//    }
//    // 初始化音频解码器
//    // 创建解码器上下文
//    AVCodecContext* audioCodecContext = avcodec_alloc_context3(audioCodec);
//    // 设置解码器参数
//    avcodec_parameters_to_context(audioCodecContext, audioCodecParameters);
//    // 打开解码器
//    if (avcodec_open2(audioCodecContext, audioCodec, NULL) != 0) {
//        std::cout << "无法打开音频解码器" << std::endl;
//        return -1;
//    }
//    std::cout << "打开音频解码器成功" << std::endl;
//    // 获取音频解码器参数
//    int sampleRate = audioCodecContext->sample_rate;
//    int channels = audioCodecContext->channels;
//    AVSampleFormat sampleFormat = audioCodecContext->sample_fmt;
//
//    // 输出音频格式信息
//    std::cout << "采样率: " << sampleRate << " Hz" << std::endl;
//    std::cout << "通道数: " << channels << std::endl;
//    std::cout << "音频格式: " << av_get_sample_fmt_name(sampleFormat) << std::endl;
//
//
//    AVStream* audioStream = formatContext->streams[audioStreamIndex];
//    AVRational timeBase = audioStream->time_base;
//    std::cout << timeBase.num <<std::endl;
//    std::cout << timeBase.den <<std::endl;
//    if (timeBase.num == 1 && timeBase.den == AV_TIME_BASE) {
//        std::cout << "时间单位为秒" <<std::endl;
//    }
//
//    // 音频处理
//    audioPlayer(formatContext, audioCodecContext, audioStreamIndex, audioCodecParameters);
//
//    // 释放资源
//    avformat_close_input(&formatContext);
//    avcodec_free_context(&audioCodecContext);
//
//    return 0;
//}