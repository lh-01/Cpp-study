cmake_minimum_required(VERSION 3.26)
project(byTask)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
# 声明一个变量 SDL_DIR 为 SDL 安装路径
set(SDL_DIR "E:/SDL2-2.28.2/x86_64-w64-mingw32")
# 设置 OpenAL 目录路径
set(OPENAL_DIR "E:/OpenAL")

include_directories(E:/ffmpeg-6.0-full_build-shared/include
        E:/SDL2-2.28.2/x86_64-w64-mingw32/include
        E:/OpenAL/include)
link_directories(E:/ffmpeg-6.0-full_build-shared/lib
        E:/SDL2-2.28.2/x86_64-w64-mingw32/lib
        E:/OpenAL/libs/Win64)

# 指定使用MinGW的链接器
set(CMAKE_CXX_LINK_EXECUTABLE "E:/mingw/mingw64/bin/g++.exe <FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES> -g")

add_executable(byTask
        task1.cpp
        task2.cpp
        task2_client.cpp
        task2_server.cpp
        task3.cpp
        task3.1.a.cpp
        task3.1.b_yuv.cpp
        task3.1.b_rgb.cpp
        audio_pcm.cpp
        video_audio.cpp
        video_rgb.cpp
        video_yuv.cpp
)
target_compile_options(byTask PRIVATE -Wno-error)

target_include_directories(byTask PRIVATE include)

target_link_libraries(byTask PRIVATE
        mingw32
        SDL2main
        SDL2
        avcodec avfilter avformat avutil swresample swscale
        OpenAL32
)

if (WIN32)
    target_link_libraries(byTask PRIVATE ws2_32)
endif()