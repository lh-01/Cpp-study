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
//// ������Ƶ����
//const int WIDTH = 640;
//const int HEIGHT = 480;
//const int FPS = 80;
//
//// SDL���ں���Ⱦ��
//SDL_Window* gWindow = nullptr;
//SDL_Renderer* gRenderer = nullptr;
//SDL_Texture* gTexture = nullptr;
//
//int64_t videoCurrentTime = 0;
//
//// ��ʼ��SDL
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
//// ��ȾRGB����
//void renderRGB(uint8_t* rgbData, const AVCodecContext* codecCtx) {
//    SDL_UpdateTexture(gTexture, nullptr, rgbData, sizeof(uint8_t) * 3 * codecCtx->width);
//    SDL_RenderClear(gRenderer);
//    SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);
//    SDL_RenderPresent(gRenderer);
//}
//
//// ����SDL��Դ
//void cleanup() {
//    SDL_DestroyTexture(gTexture);
//    SDL_DestroyRenderer(gRenderer);
//    SDL_DestroyWindow(gWindow);
//    SDL_Quit();
//}
//
//// ������Ƶ
//void playVideo(const std::string& videoFilePath) {
//    // ��ʼ��FFmpeg
//    avformat_network_init();
//
//    // ����Ƶ�ļ�
//    AVFormatContext* formatCtx = avformat_alloc_context();
//    if (avformat_open_input(&formatCtx, videoFilePath.c_str(), nullptr, nullptr) != 0) {
//        std::cerr << "�޷�����Ƶ�ļ�" << std::endl;
//        return;
//    }
//
//    // ������Ƶ����Ϣ
//    if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
//        std::cerr << "�޷���������Ϣ" << std::endl;
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    // ������Ƶ��
//    int videoStreamIndex = -1;
//    for (int i = 0; i < formatCtx->nb_streams; i++) {
//        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
//            videoStreamIndex = i;
//            break;
//        }
//    }
//
//    if (videoStreamIndex == -1) {
//        std::cerr << "�޷��ҵ���Ƶ��" << std::endl;
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    // ��ȡ�����������
//    AVCodecParameters* codecParams = formatCtx->streams[videoStreamIndex]->codecpar;
//
//    // ���ҽ�����
//    AVCodec* codec = const_cast<AVCodec *>(avcodec_find_decoder(codecParams->codec_id));
//    if (!codec) {
//        std::cout << "�Ҳ���������" << std::endl;
//        return;
//    }
//
//    // ��ȡ��Ƶ������������
//    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
//    if (codecCtx == nullptr) {
//        std::cerr << "�޷����������������" << std::endl;
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    if (avcodec_parameters_to_context(codecCtx, formatCtx->streams[videoStreamIndex]->codecpar) < 0) {
//        std::cerr << "�޷����ƽ���������" << std::endl;
//        avcodec_free_context(&codecCtx);
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    // �򿪽�����
//    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
//        std::cerr << "�޷��򿪽�����" << std::endl;
//        avcodec_free_context(&codecCtx);
//        avformat_close_input(&formatCtx);
//        return;
//    }
//
//    // �����Ƶ��ʽ��Ϣ
//    std::cout << "��Ƶ��ʽ: " << av_get_pix_fmt_name(codecCtx->pix_fmt) << std::endl;
//    std::cout << "���: " << codecCtx->width << std::endl;
//    std::cout << "�߶�: " << codecCtx->height << std::endl;
//    std::cout << "������: " << codecParams->bit_rate << " bps" << std::endl;
//
//    // ��ʼ��SDL
//    initializeSDL(codecCtx);
//
//    // ��ȡ��Ƶ֡
//    AVPacket* packet = av_packet_alloc();
//    AVFrame* frame = av_frame_alloc();
//    int frameFinished;
//
//    bool isPlaying = true;
//    bool isPaused = false;
//    bool shouldRestart = false;
//    int frameDelay = 1000 / FPS;  // ֡���(ms)
//
//    while (av_read_frame(formatCtx, packet) >= 0) {
//        // ����Ƿ�Ϊ��Ƶ��
//        if (packet->stream_index == videoStreamIndex) {
//            AVStream* videoStream = formatCtx->streams[videoStreamIndex];
//            // ��ȡ��Ƶ����ʱ���׼��������
//            AVRational timeBase = videoStream->time_base;
//            // ��ȡ��Ƶ����ʱ���pts ��ǰʱ��
//            videoCurrentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
//            // ������Ƶ֡
//            avcodec_send_packet(codecCtx, packet);
//            frameFinished = avcodec_receive_frame(codecCtx, frame);
//
//            // ���֡�Ƿ����ɹ�
//            if (frameFinished == 0) {
//                // ����swscontext
//                SwsContext* swsContext = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
//                                                        codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24,
//                                                        SWS_BILINEAR, nullptr, nullptr, nullptr);
//
//                // ����RGB֡����
//                AVFrame* rgbFrame = av_frame_alloc();
//                int rgbBufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);
//                uint8_t* rgbBuffer = static_cast<uint8_t*>(av_malloc(rgbBufferSize));
//
//                // ����RGB֡���ݵĻ�����
//                av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbBuffer,
//                                     AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);
//
//                // ����Ⱦǰ������ɫ�ռ�ת��
//                sws_scale(swsContext, frame->data, frame->linesize, 0, codecCtx->height, rgbFrame->data, rgbFrame->linesize);
//
//                // ��ȾRGB����
//                renderRGB(rgbFrame->data[0], codecCtx);
//
//                // ������Դ
//                av_frame_free(&rgbFrame);
//                av_freep(&rgbBuffer);
//                sws_freeContext(swsContext);
//            }
//        }
//
//        SDL_Delay(frameDelay);
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
//                        std::cout << "����r" << std::endl;
//                        shouldRestart = true;
//                    } else if (event.key.keysym.sym == SDLK_q) {
//                        std::cout << "����q" << std::endl;
//                        isPlaying = false;
//                    }else if(event.key.keysym.sym == SDLK_RIGHT){
//                        std::cout << "ǰ��30s" << std::endl;
//                        AVStream* videoStream = formatCtx->streams[videoStreamIndex];
//                        // ��ȡ��Ƶ����ʱ���׼��֡��
//                        AVRational timeBase = videoStream->time_base;
//
//                        int64_t span = 30 * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        int64_t targetTime = videoCurrentTime + span;
//
//                        int64_t maxTime = videoStream->duration * av_q2d(timeBase) * AV_TIME_BASE_Q.den / AV_TIME_BASE_Q.num;
//                        // std::cout << "��Ƶ��ʱ��(΢��)��" << maxTime <<std::endl;
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
//                            std::cout << "����" << errbuf << std::endl;
//                        }
//                    }else if(event.key.keysym.sym == SDLK_LEFT){
//                        std::cout << "����30s" << std::endl;
//                        AVStream* videoStream = formatCtx->streams[videoStreamIndex];
//                        // ��ȡ��Ƶ����ʱ���׼��֡��
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
//                            std::cout << "����" << errbuf << std::endl;
//                        }
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
//            SDL_RenderClear(gRenderer);
//            av_seek_frame(formatCtx, videoStreamIndex, 0, AVSEEK_FLAG_FRAME);
//            shouldRestart = false;
//            isPaused = false;
//        }
//
//        av_packet_unref(packet);
//    }
//
//    // ������Դ
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
//    std::cout << "������Ҫ���ŵ�mp4��Ƶ�ļ�·��: ";
//    std::getline(std::cin, videoFilePath);
//    playVideo(videoFilePath);
//    return 0;
//}