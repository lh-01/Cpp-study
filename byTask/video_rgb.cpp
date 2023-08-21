//#include <iostream>
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
//    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, codecContext->width, codecContext->height);
//
//    bool isPlaying = true;
//    bool isPaused = false;
//    bool shouldRestart = false;
//
//    // 读取视频帧
//    AVPacket* packet = av_packet_alloc();
//    AVFrame* frame = av_frame_alloc();
//    int frameFinished;
//
//    int frameDelay = 1000 / FPS;  // 帧间隔(ms)
//
//    while (av_read_frame(formatContext, packet) >= 0) {
//        // 检查是否为视频流
//        if (packet->stream_index == videoStreamIndex) {
//            // 解码视频帧
//            avcodec_send_packet(codecContext, packet);
//            frameFinished = avcodec_receive_frame(codecContext, frame);
//
//            // 检查帧是否解码成功
//            if (frameFinished == 0) {
//                // 创建swscontext
//                SwsContext* swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
//                                                        codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,
//                                                        SWS_BILINEAR, nullptr, nullptr, nullptr);
//
//                // 创建RGB帧数据
//                AVFrame* rgbFrame = av_frame_alloc();
//                int rgbBufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
//                uint8_t* rgbBuffer = static_cast<uint8_t*>(av_malloc(rgbBufferSize));
//
//                // 分配RGB帧数据的缓冲区
//                av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbBuffer,
//                                     AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
//
//                // 在渲染前进行颜色空间转换
//                sws_scale(swsContext, frame->data, frame->linesize, 0, codecContext->height, rgbFrame->data, rgbFrame->linesize);
//
//                // 渲染RGB数据
//                SDL_UpdateTexture(texture, nullptr, rgbFrame->data[0], sizeof(uint8_t) * 3 * codecContext->width);
//                SDL_RenderClear(renderer);
//                SDL_RenderCopy(renderer, texture, nullptr, nullptr);
//                SDL_RenderPresent(renderer);
//                SDL_Delay(frameDelay);
//
//                // 清理资源
//                av_frame_free(&rgbFrame);
//                av_freep(&rgbBuffer);
//                sws_freeContext(swsContext);
//            }
//        }
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
//                        int64_t currentTime = av_rescale_q(avio_tell(formatContext->pb), AV_TIME_BASE_Q,
//                                                           formatContext->streams[videoStreamIndex]->time_base);
//                        av_seek_frame(formatContext, videoStreamIndex,currentTime +30 * AV_TIME_BASE, AVSEEK_FLAG_ANY);
//                    }else if(event.key.keysym.sym == SDLK_LEFT){
//                        std::cout << "后退30s" << std::endl;
//                        int64_t currentTime = av_rescale_q(avio_tell(formatContext->pb), AV_TIME_BASE_Q,
//                                                           formatContext->streams[videoStreamIndex]->time_base);
//                        av_seek_frame(formatContext, videoStreamIndex,currentTime -30 * AV_TIME_BASE, AVSEEK_FLAG_ANY);
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
//        av_packet_unref(packet);
//    }
//
//    // 清理资源
//    av_frame_free(&frame);
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
//    const char* filename = "E:/byTask/source/input1.mp4";
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
//    // 释放资源
//    avformat_close_input(&formatContext);
//    avcodec_free_context(&videoCodecContext);
//
//    return 0;
//}