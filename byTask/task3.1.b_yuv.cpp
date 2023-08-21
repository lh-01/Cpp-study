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
//// ������Ƶ����
//const int WIDTH = 640;
//const int HEIGHT = 480;
//const int FPS = 80;
//
//int64_t videoCurrentTime = 0;
//
//// ��Ƶ����
//void videoPlayer(AVFormatContext* formatContext, AVCodecContext* codecContext, int videoStreamIndex) {
//    // ��ʼ��SDL
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
//        std::cout << "�޷���ʼ��SDL" << std::endl;
//    }
//
//    // �������ں���Ⱦ��
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
//    SDL_Rect dstRect; // Ŀ����Σ�������Ⱦ��Ƶ֡��������
//    dstRect.x = 0;
//    dstRect.y = 0;
//    dstRect.w = WIDTH;
//    dstRect.h = HEIGHT;
//
//    // �������ű���
//    float scaleX = static_cast<float>(dstRect.w) / srcRect.w;
//    float scaleY = static_cast<float>(dstRect.h) / srcRect.h;
//
//    bool isPlaying = true;
//    bool isPaused = false;
//    bool shouldRestart = false;
//
//    // ��ȡ��Ƶ֡
//    AVPacket* packet = av_packet_alloc();
//    while (av_read_frame(formatContext, packet) >= 0) {
//        // ����Ƿ�Ϊ��Ƶ��
//        if (packet->stream_index == videoStreamIndex) {
//            AVStream* videoStream = formatContext->streams[videoStreamIndex];
//            // ��ȡ��Ƶ����ʱ���׼��������
//            AVRational timeBase = videoStream->time_base;
//            // ��ȡ��Ƶ����ʱ���pts ��ǰʱ��
//            videoCurrentTime = packet->pts * av_q2d(timeBase) * AV_TIME_BASE;
//            // ������Ƶ���ݰ���������
//            int sendResult = avcodec_send_packet(codecContext, packet);
//            if (sendResult < 0) {
//                std::cout << "�޷�������Ƶ���ݰ���������" << std::endl;
//                break;
//            }
//
//            // ���ս�������Ƶ����֡
//            AVFrame* frame = av_frame_alloc();
//            while (avcodec_receive_frame(codecContext, frame) == 0) {
//                // ����Ŀ����εĳߴ�
//                dstRect.w = static_cast<int>(srcRect.w * scaleX);
//                dstRect.h = static_cast<int>(srcRect.h * scaleY);
//
//                // ��Ⱦ��Ƶ֡
//                SDL_UpdateYUVTexture(texture, &srcRect, frame->data[0], frame->linesize[0],
//                                     frame->data[1], frame->linesize[1],
//                                     frame->data[2], frame->linesize[2]);
//                SDL_RenderClear(renderer);
//                SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
//                SDL_RenderPresent(renderer);
//                SDL_Delay(40); // ����֡��
//            }
//            av_frame_free(&frame);
//        }
//        // �������������ͻ�����
//        std::condition_variable cv;
//        std::mutex mtx;
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
//                        AVStream* videoStream = formatContext->streams[videoStreamIndex];
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
//                        int seekResult = av_seek_frame(formatContext, videoStreamIndex, timestamp, seekFlags);
//
//                        if (seekResult < 0) {
//                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
//                            av_strerror(seekResult, errbuf, sizeof(errbuf));
//                            std::cout << "����" << errbuf << std::endl;
//                        }
//                    }else if(event.key.keysym.sym == SDLK_LEFT){
//                        std::cout << "����30s" << std::endl;
//                        AVStream* videoStream = formatContext->streams[videoStreamIndex];
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
//                        int seekResult = av_seek_frame(formatContext, videoStreamIndex, timestamp, seekFlags);
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
//            SDL_RenderClear(renderer);
//            av_seek_frame(formatContext, videoStreamIndex, 0, AVSEEK_FLAG_FRAME);
//            shouldRestart = false;
//            isPaused = false;
//        }
////        if (isPaused) {
////            // ��ͣ
////            std::cout << "��ͣ����" << std::endl;
////            SDL_Delay(1000);
//////            std::unique_lock<std::mutex> lock(mtx);
//////            cv.wait(lock, [&] { return !isPaused; }); // �ȴ������źţ�ֱ��isPausedΪfalse
////            continue;
////        }
//    }
//
//    // ������Դ
//    av_packet_free(&packet);
//    SDL_DestroyTexture(texture);
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//}
//
//int main(int argc, char* args[]) {
//    // ��ʼ��FFmpeg
//    avformat_network_init();
//    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
//
//    std::string filename;
//    std::cout << "������Ҫ���ŵ�mp4��Ƶ�ļ�·��: ";
//    std::getline(std::cin, filename);
//
//    // ����Ƶ�ļ�
//    AVFormatContext* formatContext = avformat_alloc_context();
//    if (avformat_open_input(&formatContext, filename.c_str(), NULL, NULL) != 0) {
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
//    int videoStreamIndex = -1;
//    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
//        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex < 0) {
//            videoStreamIndex = i;
//        }
//    }
//
//    // ��ȡ��Ƶ�����������
//    AVCodecParameters* videoCodecParams = formatContext->streams[videoStreamIndex]->codecpar;
//
//    // ��ȡ��Ƶ������
//    AVCodec* videoCodec = const_cast<AVCodec *>(avcodec_find_decoder(videoCodecParams->codec_id));
//    if (videoCodec == NULL) {
//        std::cout << "�Ҳ�����Ƶ������" << std::endl;
//        return -1;
//    }
//
//    // ��ʼ����Ƶ������
//    // ��ȡ��Ƶ������������
//    AVCodecContext* videoCodecContext = avcodec_alloc_context3(videoCodec);
//    // ���ý���������
//    avcodec_parameters_to_context(videoCodecContext, videoCodecParams);
//    if (avcodec_open2(videoCodecContext, videoCodec, NULL) != 0) {
//        std::cout << "�޷�����Ƶ������" << std::endl;
//        return -1;
//    }
//
//    // �����Ƶ��ʽ��Ϣ
//    std::cout << "��Ƶ��ʽ: " << av_get_pix_fmt_name(videoCodecContext->pix_fmt) << std::endl;
//    std::cout << "���: " << videoCodecContext->width << std::endl;
//    std::cout << "�߶�: " << videoCodecContext->height << std::endl;
//    std::cout << "������: " << videoCodecParams->bit_rate << " bps" << std::endl;
//
//    videoPlayer(formatContext, videoCodecContext, videoStreamIndex);
//
//
//    // �ͷ���Դ
//    avformat_close_input(&formatContext);
//    avformat_free_context(formatContext);
//    avcodec_free_context(&videoCodecContext);
//
//    return 0;
//}