//#include <iostream>
//#include <fstream>
//#include <SDL2/SDL.h>
//
//const int WIDTH = 640;
//const int HEIGHT = 480;
//const int FPS = 30;
//
//SDL_Window* gWindow = nullptr;
//SDL_Renderer* gRenderer = nullptr;
//SDL_Texture* gTexture = nullptr;
//
//bool initSDL()
//{
//    if (SDL_Init(SDL_INIT_VIDEO) < 0)
//    {
//        std::cout << "SDL ��ʼ��ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    gWindow = SDL_CreateWindow("YUV Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
//    if (!gWindow)
//    {
//        std::cout << "��������ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//    if (!gRenderer)
//    {
//        std::cout << "������Ⱦ��ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
//    if (!gTexture)
//    {
//        std::cout << "��������ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    return true;
//}
//
//void playYUV(const char* filename)
//{
//    FILE* yuvVideo = fopen(filename, "rb");
//    if (!yuvVideo)
//    {
//        std::cout << "���ļ�ʧ�ܣ�" << filename << std::endl;
//        return;
//    }
//    fseek(yuvVideo, 0, SEEK_END);
//    long fileSize = ftell(yuvVideo);
//    rewind(yuvVideo);
//
//    uint32_t y_frame_len = WIDTH * HEIGHT;
//    uint32_t u_frame_len = (WIDTH / 2) * (HEIGHT / 2);
//    uint32_t v_frame_len = (WIDTH / 2) * (HEIGHT / 2);
//    uint32_t yuv_frame_len = y_frame_len + u_frame_len + v_frame_len;
//    uint8_t* yuvBuffer = new uint8_t[yuv_frame_len];
//    size_t video_buff_len = 0;
//    bool loop = true;
//
//    while (loop)
//    {
//        video_buff_len = fread(yuvBuffer, 1, yuv_frame_len, yuvVideo);
//        if (video_buff_len <= 0)
//        {
//            break;
//        }
//
//        SDL_UpdateTexture(gTexture, NULL, yuvBuffer, WIDTH);
//
//        SDL_Rect rect;
//        rect.x = 0;
//        rect.y = 0;
//        rect.w = WIDTH;
//        rect.h = HEIGHT;
//
//        SDL_RenderClear(gRenderer);
//        SDL_RenderCopy(gRenderer, gTexture, NULL, &rect);
//        SDL_RenderPresent(gRenderer);
//        SDL_Delay(40);
//
//        // ����ļ��Ƿ��Ѿ���ȡ���
//        if (video_buff_len < yuv_frame_len)
//        {
//            loop = false;
//        }
//
//        SDL_Event event;
//        while (SDL_PollEvent(&event))
//        {
//            if (event.type == SDL_QUIT)
//            {
//                loop = false;
//                break;
//            }
//            if (event.type == SDL_KEYDOWN)
//            {
//                if (event.key.keysym.sym == SDLK_SPACE)
//                {
//                    bool paused = true;
//                    while (paused)
//                    {
//                        while (SDL_PollEvent(&event))
//                        {
//                            if (event.type == SDL_QUIT)
//                            {
//                                paused = false;
//                                loop = false;
//                                break;
//                            }
//                            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
//                            {
//                                paused = false;
//                                break;
//                            }
//                        }
//                    }
//                }
//                else if (event.key.keysym.sym == SDLK_q)
//                {
//                    loop = false;
//                    break;
//                }
//                else if (event.key.keysym.sym == SDLK_r)
//                {
//                    rewind(yuvVideo);
//                }
//            }
//        }
//    }
//
//    delete[] yuvBuffer;
//    fclose(yuvVideo);
//    SDL_DestroyTexture(gTexture);
//    SDL_DestroyRenderer(gRenderer);
//    SDL_DestroyWindow(gWindow);
//    SDL_Quit();}
//
//int main(int argc, char* args[])
//{
//    std::string filename;
//    std::cout << "�������ļ�·��: ";
//    std::getline(std::cin, filename);
//
//    if (!initSDL())
//    {
//        return -1;
//    }
//
//    playYUV(filename.c_str());
//    return 0;
//}