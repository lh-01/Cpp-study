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
