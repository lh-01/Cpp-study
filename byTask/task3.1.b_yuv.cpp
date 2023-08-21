//#include <iostream>
//#include <condition_variable>
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
//// 定义视频参数
//const int WIDTH = 640;
//const int HEIGHT = 480;
//const int FPS = 80;
//
//int64_t videoCurrentTime = 0;
//
//// 视频播放
//void videoPlayer(AVFormatContext* formatContext, AVCodecContext* codecContext, int videoStreamIndex) {
//    // 初始化SDL
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
//        std::cout << "无法初始化SDL" << std::endl;
//    }
//
//    // 创建窗口和渲染器
//    SDL_Window* window = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, codecContext->width, codecContext->height);
//
//    SDL_Rect srcRect;
//    srcRect.x = 0;
//    srcRect.y = 0;
//    srcRect.w = codecContext->width;
//    srcRect.h = codecContext->height;
//
//    SDL_Rect dstRect; // 目标矩形，用于渲染视频帧到窗口中
//    dstRect.x = 0;
//    dstRect.y = 0;
//    dstRect.w = WIDTH;
//    dstRect.h = HEIGHT;
//
//    // 计算缩放比例
//    float scaleX = static_cast<float>(dstRect.w) / srcRect.w;
//    float scaleY = static_cast<float>(dstRect.h) / srcRect.h;
//
//    bool isPlaying = true;
//    bool isPaused = false;
//    bool shouldRestart = false;
//
//    // 读取视频帧
//    AVPacket* packet = av_packet_alloc();
//    while (av_read_frame(formatContext, packet) >= 0) {
//        // 检查是否为视频流
//        if (packet->stream_index == videoStreamIndex) {
//            AVStream* videoStream = formatContext->streams[videoStreamIndex];
//            // 获取音频流的时间基准和样本率
//            AVRational timeBase = videoStream->time_base;
//            // 获取音频包的时间戳pts 当前时间
//            videoCurrentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
//            // 发送视频数据包到解码器
//            int sendResult = avcodec_send_packet(codecContext, packet);
//            if (sendResult < 0) {
//                std::cout << "无法发送视频数据包到解码器" << std::endl;
//                break;
//            }
//
//            // 接收解码后的视频数据帧
//            AVFrame* frame = av_frame_alloc();
//            while (avcodec_receive_frame(codecContext, frame) == 0) {
//                // 更新目标矩形的尺寸
//                dstRect.w = static_cast<int>(srcRect.w * scaleX);
//                dstRect.h = static_cast<int>(srcRect.h * scaleY);
//
//                // 渲染视频帧
//                SDL_UpdateYUVTexture(texture, &srcRect, frame->data[0], frame->linesize[0],
//                                     frame->data[1], frame->linesize[1],
//                                     frame->data[2], frame->linesize[2]);
//                SDL_RenderClear(renderer);
//                SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
//                SDL_RenderPresent(renderer);
//                SDL_Delay(40); // 控制帧率
//            }
//            av_frame_free(&frame);
//        }
//        // 定义条件变量和互斥锁
//        std::condition_variable cv;
//        std::mutex mtx;
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
//                        bool paused = true;
//                        while (paused)
//                        {
//                            while (SDL_PollEvent(&event))
//                            {
//                                if (event.type == SDL_QUIT)
//                                {
//                                    paused = false;
//                                    break;
//                                }
//                                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
//                                {
//                                    paused = false;
//                                    break;
//                                }
//                            }
//                        }
//                    } else if (event.key.keysym.sym == SDLK_r) {
//                        std::cout << "按下r" << std::endl;
//                        shouldRestart = true;
//                    } else if (event.key.keysym.sym == SDLK_q) {
//                        std::cout << "按下q" << std::endl;
//                        isPlaying = false;
//                    }else if(event.key.keysym.sym == SDLK_RIGHT){
//                        std::cout << "前进30s" << std::endl;
//                        AVStream* videoStream = formatContext->streams[videoStreamIndex];
//                        // 获取视频流的时间基准和帧率
//                        AVRational timeBase = videoStream->time_base;
//
//                        int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        int64_t targetTime = videoCurrentTime + span;
//
//                        int64_t maxTime = videoStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        // std::cout << "视频总时间(微秒)：" << maxTime <<std::endl;
//
//                        if (targetTime >= maxTime) {
//                            targetTime = maxTime;
//                        }
//
//                        int seekFlags = AVSEEK_FLAG_BACKWARD;
//                        int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase);
//                        int seekResult = av_seek_frame(formatContext, videoStreamIndex, timestamp, seekFlags);
//
//                        if (seekResult < 0) {
//                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                            av_strerror(seekResult, errbuf, sizeof(errbuf));
//                            std::cout << "错误：" << errbuf << std::endl;
//                        }
//                    }else if(event.key.keysym.sym == SDLK_LEFT){
//                        std::cout << "后退30s" << std::endl;
//                        AVStream* videoStream = formatContext->streams[videoStreamIndex];
//                        // 获取视频流的时间基准和帧率
//                        AVRational timeBase = videoStream->time_base;
//                        int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        int64_t targetTime = videoCurrentTime - span;
//
//                        int64_t minTime = 0;
//
//                        if (targetTime <= minTime) {
//                            targetTime = minTime;
//                        }
//
//                        int seekFlags = AVSEEK_FLAG_BACKWARD;
//                        int64_t timestamp = av_rescale_q(targetTime, AV_TIME_BASE_Q, timeBase);
//                        int seekResult = av_seek_frame(formatContext, videoStreamIndex, timestamp, seekFlags);
//
//                        if (seekResult < 0) {
//                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                            av_strerror(seekResult, errbuf, sizeof(errbuf));
//                            std::cout << "错误：" << errbuf << std::endl;
//                        }
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
//            SDL_RenderClear(renderer);
//            av_seek_frame(formatContext, videoStreamIndex, 0, AVSEEK_FLAG_FRAME);
//            shouldRestart = false;
//            isPaused = false;
//        }
////        if (isPaused) {
////            // 暂停
////            std::cout << "暂停播放" << std::endl;
////            SDL_Delay(1000);
//////            std::unique_lock<std::mutex> lock(mtx);
//////            cv.wait(lock, [&] { return !isPaused; }); // 等待唤醒信号，直到isPaused为false
////            continue;
////        }
//    }
//
//    // 清理资源
//    av_packet_free(&packet);
//    SDL_DestroyTexture(texture);
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//}
//
//int main(int argc, char* args[]) {
//    // 初始化FFmpeg
//    avformat_network_init();
//    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
//
//    std::string filename;
//    std::cout << "请输入要播放的mp4视频文件路径: ";
//    std::getline(std::cin, filename);
//
//    // 打开视频文件
//    AVFormatContext* formatContext = avformat_alloc_context();
//    if (avformat_open_input(&formatContext, filename.c_str(), NULL, NULL) != 0) {
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
//    int videoStreamIndex = -1;
//    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
//        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex < 0) {
//            videoStreamIndex = i;
//        }
//    }
//
//    // 获取视频编解码器参数
//    AVCodecParameters* videoCodecParams = formatContext->streams[videoStreamIndex]->codecpar;
//
//    // 获取视频解码器
//    AVCodec* videoCodec = const_cast<AVCodec *>(avcodec_find_decoder(videoCodecParams->codec_id));
//    if (videoCodec == NULL) {
//        std::cout << "找不到视频解码器" << std::endl;
//        return -1;
//    }
//
//    // 初始化视频解码器
//    // 获取视频解码器上下文
//    AVCodecContext* videoCodecContext = avcodec_alloc_context3(videoCodec);
//    // 设置解码器参数
//    avcodec_parameters_to_context(videoCodecContext, videoCodecParams);
//    if (avcodec_open2(videoCodecContext, videoCodec, NULL) != 0) {
//        std::cout << "无法打开视频解码器" << std::endl;
//        return -1;
//    }
//
//    // 输出视频格式信息
//    std::cout << "视频格式: " << av_get_pix_fmt_name(videoCodecContext->pix_fmt) << std::endl;
//    std::cout << "宽度: " << videoCodecContext->width << std::endl;
//    std::cout << "高度: " << videoCodecContext->height << std::endl;
//    std::cout << "比特率: " << videoCodecParams->bit_rate << " bps" << std::endl;
//
//    videoPlayer(formatContext, videoCodecContext, videoStreamIndex);
//
//
//    // 释放资源
//    avformat_close_input(&formatContext);
//    avformat_free_context(formatContext);
//    avcodec_free_context(&videoCodecContext);
//
//    return 0;
//}