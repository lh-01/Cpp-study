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
//// ������Ƶ����
//const int WIDTH = 640;
//const int HEIGHT = 480;
//const int FPS = 80;
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
//    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, codecContext->width, codecContext->height);
//
//    bool isPlaying = true;
//    bool isPaused = false;
//    bool shouldRestart = false;
//
//    // ��ȡ��Ƶ֡
//    AVPacket* packet = av_packet_alloc();
//    AVFrame* frame = av_frame_alloc();
//    int frameFinished;
//
//    int frameDelay = 1000 / FPS;  // ֡���(ms)
//
//    while (av_read_frame(formatContext, packet) >= 0) {
//        // ����Ƿ�Ϊ��Ƶ��
//        if (packet->stream_index == videoStreamIndex) {
//            // ������Ƶ֡
//            avcodec_send_packet(codecContext, packet);
//            frameFinished = avcodec_receive_frame(codecContext, frame);
//
//            // ���֡�Ƿ����ɹ�
//            if (frameFinished == 0) {
//                // ����swscontext
//                SwsContext* swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
//                                                        codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,
//                                                        SWS_BILINEAR, nullptr, nullptr, nullptr);
//
//                // ����RGB֡����
//                AVFrame* rgbFrame = av_frame_alloc();
//                int rgbBufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
//                uint8_t* rgbBuffer = static_cast<uint8_t*>(av_malloc(rgbBufferSize));
//
//                // ����RGB֡���ݵĻ�����
//                av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbBuffer,
//                                     AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
//
//                // ����Ⱦǰ������ɫ�ռ�ת��
//                sws_scale(swsContext, frame->data, frame->linesize, 0, codecContext->height, rgbFrame->data, rgbFrame->linesize);
//
//                // ��ȾRGB����
//                SDL_UpdateTexture(texture, nullptr, rgbFrame->data[0], sizeof(uint8_t) * 3 * codecContext->width);
//                SDL_RenderClear(renderer);
//                SDL_RenderCopy(renderer, texture, nullptr, nullptr);
//                SDL_RenderPresent(renderer);
//                SDL_Delay(frameDelay);
//
//                // ������Դ
//                av_frame_free(&rgbFrame);
//                av_freep(&rgbBuffer);
//                sws_freeContext(swsContext);
//            }
//        }
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
//                        int64_t currentTime = av_rescale_q(avio_tell(formatContext->pb), AV_TIME_BASE_Q,
//                                                           formatContext->streams[videoStreamIndex]->time_base);
//                        av_seek_frame(formatContext, videoStreamIndex,currentTime +30 * AV_TIME_BASE, AVSEEK_FLAG_ANY);
//                    }else if(event.key.keysym.sym == SDLK_LEFT){
//                        std::cout << "����30s" << std::endl;
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
//        av_packet_unref(packet);
//    }
//
//    // ������Դ
//    av_frame_free(&frame);
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
//    const char* filename = "E:/byTask/source/input1.mp4";
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
//    // �ͷ���Դ
//    avformat_close_input(&formatContext);
//    avcodec_free_context(&videoCodecContext);
//
//    return 0;
//}