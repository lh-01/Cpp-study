# Cpp-study

## Taks1 简易计算器

## 作者: 刘昊

### 运行结果截屏
- ![task1](https://github.com/lh-01/Cpp-study/assets/81234347/4f3a98c1-3b74-4e4a-8a05-8e53aa164df5)


### 简单说明
- 代码使用了两个栈，一个用于保存操作数，一个用于保存运算符。通过迭代扫描输入的表达式，按照运算符的优先级进行运算，最终得到结果。
- precedence 函数用于判断运算符的优先级，返回一个整数值。值越大表示优先级越高。
- applyOperation 函数根据给定的运算符和操作数，返回运算结果。
- evaluateExpression 函数用于计算给定表达式的值。在遍历表达式时，它将数字压入操作数栈中，将运算符压入运算符栈中，并根据运算符的优先级进行计算。
- main 函数负责不断接收用户输入的表达式，调用 evaluateExpression 函数计算结果并输出。 

### 花费时间
-  4小时

## Taks2 简易文字聊天程序

## 作者: 刘昊

### 运行结果截屏
#### 服务器端
- ![服务器](https://github.com/lh-01/Cpp-study/assets/81234347/3e6ac5f1-204d-4644-87aa-183fe5423003)
#### 客户端1
- ![客户端1_1](https://github.com/lh-01/Cpp-study/assets/81234347/0025e8c8-6d39-464a-b7bd-043a23f5f3b2)
- ![客户端1_2](https://github.com/lh-01/Cpp-study/assets/81234347/dd3609c1-7236-4f3d-932c-afd704e0938f)
#### 客户端2
- ![客户端2_1](https://github.com/lh-01/Cpp-study/assets/81234347/ddc8c8f7-00d5-49c1-8370-3b7ff51e2fba)
- ![客户端2_2](https://github.com/lh-01/Cpp-study/assets/81234347/4fc7d061-5853-46eb-a853-75560a87f7f9)

### 简单说明
 - 该程序分为客户端程序和服务器端程序，在主程序中用户输入不同的选项执行不同的功能。
 - 在客户端程序中，创建了Client类，主要包括连接服务器、登录、注销、发送消息和接收消息等功能。在Client类中包括构造函数Client()用于初始化Winsock和创建客户端套接字，connectToServer()方法用于连接指定的服务器，login()方法用于登录服务器，logout()方法用于注销登录，sendMessage()方法用于向指定用户发送消息，receiveMessage()方法用于接收消息。
 - 在服务器端程序中，创建了Server类，主要包括启动服务器、处理客户端请求、处理登录、注销和发送消息等功能。在Server类中包括构造函数Server()用于初始化Winsock和创建服务器端套接字，start()函数中用于启动服务器，绑定指定端口并开始监听，等待客户端的连接请求。一旦接受到客户端的连接请求，就创建一个新的套接字与客户端进行通信，处理客户端的登录、注销和发送消息请求，login()方法用于处理登录请求，logout()方法用于处理注销请求，sendMessage()用于将消息发送到指定客户端，getSocketByUsername()方法用于根据用户名获取套接字。

### 花费时间
-  4d
  
## Taks3.3 基于 ffmpeg 的音视频播放器

## 作者: 刘昊

### 运行结果录屏
- 录屏链接

### 简单说明
- 对自己的程序进行简单说明

### 花费时间
- 7d
  
## Taks3.1.a 基于SDL的yuv图像播放器

## 作者: 刘昊

### 运行结果录屏
- 录屏链接

### 简单说明
- 代码中的initSDL函数用于初始化SDL库、创建窗口、渲染器和纹理，确保SDL环境正确设置。
playYUV函数用于播放YUV视频文件。首先打开指定的YUV视频文件，然后循环读取视频帧数据并进行渲染。每次循环，它调用SDL_UpdateTexture更新纹理，然后通过SDL_RenderCopy将纹理复制到渲染器上并调用SDL_RenderPresent将渲染内容显示在窗口上。
在循环过程中，程序会监听SDL事件，当用户按下空格键时暂停视频播放，再次按下空格键继续播放。按下’q’键退出播放，按下’r’键重新回到视频开头。当用户点击窗口关闭按钮时，程序也会退出。
代码通过使用SDL库提供的函数和事件处理机制，实现了一个简单的YUV视频播放器，它能够显示YUV格式的视频文件，并且支持暂停、继续、退出和重新播放等功能。

### 花费时间
- 2d
  
## Taks3.1.b 基于SDL的mp4视频播放器

## 作者: 刘昊

### 运行结果录屏
- 录屏链接

### 简单说明
 #### 方式1：task3.1.b_yuv
- 代码中使用FFmpeg库解码和SDL库显示视频，将输入的mp4视频文件解码为原始视频帧，并通过SDL库将帧渲染到窗口上。
 程序中首先初始化SDL和FFmpeg库，并获取要播放的mp4视频文件路径，并打开该文件。然后，程序根据视频流的类型找到视频流并获得视频编解码器。接下来，程序将视频帧解码并调用SDL函数进行渲染。
 在videoPlayer函数中，程序循环读取视频帧的数据，并对帧进行解码、转换和渲染。它通过调用av_read_frame函数读取一个音频或视频包，然后检查包的流索引是否与视频流索引匹配。如果匹配，就解码视频包，并通过SDL函数渲染视频帧。循环中，包的数据通过avcodec_send_packet函数发送给解码器，并通过avcodec_receive_frame函数接收解码后的帧。然后，将帧数据更新到SDL纹理中，然后将纹理复制到渲染器上，并调用SDL_RenderPresent函数显示渲染内容。通过SDL_Delay函数，控制帧率。在循环过程中，程序会监听SDL事件，当用户按下空格键时暂停视频播放，再次按下空格键继续播放。按下’q’键退出播放，按下’r’键重新回到视频开头，按下右键视频前进30s,按下左键视频后退30s。当用户点击窗口关闭按钮时，程序也会退出。最后，程序在结束之前进行内存释放和清理工作，并关闭SDL和FFmpeg库。
 #### 方式2：task3.1.b_rgb
- 代码中使用FFmpeg库解码和SDL库显示视频，将输入的mp4视频文件解码为原始视频帧，并通过SDL库将帧渲染到窗口上。
initializeSDL()函数通过调用SDL库的相关函数来初始化SDL，并创建窗口、渲染器和纹理用于显示视频内容。
renderRGB()函数将RGB数据渲染到SDL的纹理上，并通过渲染器进行显示。
cleanup()函数用于清理SDL相关资源，包括销毁纹理、渲染器和窗口，并调用SDL库的清理函数。
playVideo()函数是主要的视频播放函数。它首先初始化FFmpeg库，然后打开并检索视频文件的信息。接着查找视频流，并获取其解码器参数。接下来，根据解码器参数查找相应的解码器，并创建解码器上下文。然后打开解码器。输出视频的格式信息，包括像素格式、宽度、高度和比特率。调用initializeSDL()函数初始化SDL。进入循环，不断读取视频帧。如果帧是视频帧，则解码并进行颜色空间转换，然后调用renderRGB()函数渲染和显示RGB数据。在循环中使用SDL_Delay()函数来控制帧率，使视频播放流畅。在循环过程中，程序会监听SDL事件，当用户按下空格键时暂停视频播放，再次按下空格键继续播放。按下’q’键退出播放，按下’r’键重新回到视频开头，按下右键视频前进30s,按下左键视频后退30s。当用户点击窗口关闭按钮时，程序也会退出。在循环结束后，释放相关资源，包括帧和数据包的内存，以及解码器上下文等。

### 花费时间
- 2d
  
