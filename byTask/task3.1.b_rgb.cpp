//#include <iostream>
//#include <minwindef.h>
//#include <condition_variable>
//#include <mutex>
//
//extern "C" {
//#include <SDL2/SDL.h>
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libavutil/imgutils.h>
//}
//
//// 定义视频参数
//const int WIDTH = 640;
//const int HEIGHT = 480;
//const int FPS = 80;
//
//// SDL窗口和渲染器
//SDL_Window* gWindow = nullptr;
//SDL_Renderer* gRenderer = nullptr;
//SDL_Texture* gTexture = nullptr;
//
//int64_t videoCurrentTime = 0;
//
//// 初始化SDL
//void initializeSDL(const AVCodecContext* codecCtx) {
//    SDL_Init(SDL_INIT_VIDEO);
//
//    gWindow = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//                               WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
//    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
//    gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
//                                 codecCtx->width, codecCtx->height);
//}
//
//// 渲染RGB数据
//void renderRGB(uint8_t* rgbData, const AVCodecContext* codecCtx) {
//    SDL_UpdateTexture(gTexture, nullptr, rgbData, sizeof(uint8_t) * 3 * codecCtx->width);
//    SDL_RenderClear(gRenderer);
//    SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);
//    SDL_RenderPresent(gRenderer);
//}
//
//// 清理SDL资源
//void cleanup() {
//    SDL_DestroyTexture(gTexture);
//    SDL_DestroyRenderer(gRenderer);
//    SDL_DestroyWindow(gWindow);
//    SDL_Quit();
//}
//
//// 播放视频
//void playVideo(const std::string& videoFilePath) {
//    // 初始化FFmpeg
//    avformat_network_init();
//
//    // 打开视频文件
//    AVFormatContext* formatCtx = avformat_alloc_context();
//    if (avformat_open_input(&formatCtx, videoFilePath.c_str(), nullptr, nullptr) != 0) {
//        std::cerr << "无法打开视频文件" << std::endl;
//        return;
//    }
//
//    // 检索视频流信息
//    if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
//        std::cerr << "无法检索流信息" << std::endl;
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    // 查找视频流
//    int videoStreamIndex = -1;
//    for (int i = 0; i < formatCtx->nb_streams; i++) {
//        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
//            videoStreamIndex = i;
//            break;
//        }
//    }
//
//    if (videoStreamIndex == -1) {
//        std::cerr << "无法找到视频流" << std::endl;
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    // 获取编解码器参数
//    AVCodecParameters* codecParams = formatCtx->streams[videoStreamIndex]->codecpar;
//
//    // 查找解码器
//    AVCodec* codec = const_cast<AVCodec *>(avcodec_find_decoder(codecParams->codec_id));
//    if (!codec) {
//        std::cout << "找不到解码器" << std::endl;
//        return;
//    }
//
//    // 获取视频解码器上下文
//    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
//    if (codecCtx == nullptr) {
//        std::cerr << "无法分配解码器上下文" << std::endl;
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    if (avcodec_parameters_to_context(codecCtx, formatCtx->streams[videoStreamIndex]->codecpar) < 0) {
//        std::cerr << "无法复制解码器参数" << std::endl;
//        avcodec_free_context(&codecCtx);
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    // 打开解码器
//    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
//        std::cerr << "无法打开解码器" << std::endl;
//        avcodec_free_context(&codecCtx);
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    // 输出视频格式信息
//    std::cout << "视频格式: " << av_get_pix_fmt_name(codecCtx->pix_fmt) << std::endl;
//    std::cout << "宽度: " << codecCtx->width << std::endl;
//    std::cout << "高度: " << codecCtx->height << std::endl;
//    std::cout << "比特率: " << codecParams->bit_rate << " bps" << std::endl;
//
//    // 初始化SDL
//    initializeSDL(codecCtx);
//
//    // 读取视频帧
//    AVPacket* packet = av_packet_alloc();
//    AVFrame* frame = av_frame_alloc();
//    int frameFinished;
//
//    bool isPlaying = true;
//    bool isPaused = false;
//    bool shouldRestart = false;
//    int frameDelay = 1000 / FPS;  // 帧间隔(ms)
//
//    while (av_read_frame(formatCtx, packet) >= 0) {
//        // 检查是否为视频流
//        if (packet->stream_index == videoStreamIndex) {
//            AVStream* videoStream = formatCtx->streams[videoStreamIndex];
//            // 获取视频流的时间基准和样本率
//            AVRational timeBase = videoStream->time_base;
//            // 获取视频包的时间戳pts 当前时间
//            videoCurrentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
//            // 解码视频帧
//            avcodec_send_packet(codecCtx, packet);
//            frameFinished = avcodec_receive_frame(codecCtx, frame);
//
//            // 检查帧是否解码成功
//            if (frameFinished == 0) {
//                // 创建swscontext
//                SwsContext* swsContext = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
//                                                        codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24,
//                                                        SWS_BILINEAR, nullptr, nullptr, nullptr);
//
//                // 创建RGB帧数据
//                AVFrame* rgbFrame = av_frame_alloc();
//                int rgbBufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);
//                uint8_t* rgbBuffer = static_cast<uint8_t*>(av_malloc(rgbBufferSize));
//
//                // 分配RGB帧数据的缓冲区
//                av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbBuffer,
//                                     AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);
//
//                // 在渲染前进行颜色空间转换
//                sws_scale(swsContext, frame->data, frame->linesize, 0, codecCtx->height, rgbFrame->data, rgbFrame->linesize);
//
//                // 渲染RGB数据
//                renderRGB(rgbFrame->data[0], codecCtx);
//
//                // 清理资源
//                av_frame_free(&rgbFrame);
//                av_freep(&rgbBuffer);
//                sws_freeContext(swsContext);
//            }
//        }
//
//        SDL_Delay(frameDelay);
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
//                        AVStream* videoStream = formatCtx->streams[videoStreamIndex];
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
//                        int seekResult = av_seek_frame(formatCtx, videoStreamIndex, timestamp, seekFlags);
//
//                        if (seekResult < 0) {
//                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                            av_strerror(seekResult, errbuf, sizeof(errbuf));
//                            std::cout << "错误：" << errbuf << std::endl;
//                        }
//                    }else if(event.key.keysym.sym == SDLK_LEFT){
//                        std::cout << "后退30s" << std::endl;
//                        AVStream* videoStream = formatCtx->streams[videoStreamIndex];
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
//                        int seekResult = av_seek_frame(formatCtx, videoStreamIndex, timestamp, seekFlags);
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
//            SDL_RenderClear(gRenderer);
//            av_seek_frame(formatCtx, videoStreamIndex, 0, AVSEEK_FLAG_FRAME);
//            shouldRestart = false;
//            isPaused = false;
//        }
//
//        av_packet_unref(packet);
//    }
//
//    // 清理资源
//    av_frame_free(&frame);
//    av_packet_free(&packet);
//    avcodec_free_context(&codecCtx);
//    avformat_close_input(&formatCtx);
//    cleanup();
//}
//
//
//int main(int argc, char* args[]) {
//    std::string videoFilePath;
//    std::cout << "请输入要播放的mp4视频文件路径: ";
//    std::getline(std::cin, videoFilePath);
//    playVideo(videoFilePath);
//    return 0;
//}