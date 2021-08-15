# BiliBiliCCSubtitle:一个用于下载B站CC字幕及转换的工具


[![badge](https://img.shields.io/badge/license-Apache-blue.svg)](https://github.com/nathanli97/BilibiliCCSubtitle/blob/master/LICENSE)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://bitbucket.org/lbesson/ansi-colors)
[![Platform](https://img.shields.io/badge/platform-Win32|Win32_64-lightgrey.svg)](#)
[![Version](https://img.shields.io/badge/version-1.1.1-blue.svg)](#)

### Changelog
2021-6-5 修复了多P视频中间有分p没有字幕会导致下载中断的问题

2021-8-15 增加了下载到文件夹的功能；增加了对国际版/东南亚版Bilibili的支持。

### 运行环境要求：
> Windows 10，X86_64 or X86
### 编译依赖：
> vcpkg
>
> libcurl 
>
>  jsoncpp
## 使用说明
你可以在[这里（Win64）](https://github.com/nathanli97/BiliBiliCCSubtitle/releases/latest/download/BiliBiliCCSubtitle-x86_64.zip)或者[这里（Win32）](https://github.com/nathanli97/BiliBiliCCSubtitle/releases/latest/download/BiliBiliCCSubtitle-x86.zip)下载到最新版本程序。或者克隆整个库进行编译。

### 第一步：下载CC字幕。
> ccdown -d url

这里的URL指的是带有CC字幕的BiliBili播放页地址。比如说这个转送门：
[https://www.bilibili.com/video/BV1JE411N7UD](https://www.bilibili.com/video/BV1JE411N7UD)

或者这个：

[https://www.biliintl.com/en/play/1010919/10446796](https://www.biliintl.com/en/play/1010919/10446796)

一般情况下，下载的字幕文件名随着字幕语言的不同而不同。比如说，如果你想要下载的CC字幕提供了中文和英文两种语言可选，那么当你执行了下面的指令：

> ccdown -d http://bilibili.com/video/BVXXXXXXXXX

当然，如果指定的视频有多个分P，你可以在URL指定分P:
> ccdown -d http://bilibili.com/video/BVXXXXXXXXX?p=2

或者，你想下载P2~P5的所有字幕(注意，URL不要在指定分P，因为在URL指定分P会覆盖分P范围选项):
> ccdown -s 2 -e 5 -d http://bilibili.com/video/BVXXXXXXXXX

又或者，你想下载P6及之前的所有分P字幕:
> ccdown -e 6 -d http://bilibili.com/video/BVXXXXXXXXX

又或者，你想下载P1及之后的分P字幕：
> ccdown -s 1 -d http://bilibili.com/video/BVXXXXXXXXX

默认情况下，程序会在程序所在目录新建一个“downloads"的文件夹，此文件夹下会保存下载的字幕文件。

之后，downloads/<BV号>/文件夹下会多出一些文件：AV号(BV号)-Pi.zh-CN.json AV号(BV号)-Pi.en-US.json（Pi代表分P，i=1,2,3,....）（国内版Bilibili）

或者downloads/GLOBAL<视频ID>/文件夹下会多出一些文件：GLOBAL<视频ID>-<语言ID>.json。

### 第二步：转换CC字幕

由于B站的字幕文件使用JSON格式存储的，所以你可以用下面这条指令将其转换为普通字幕文件：

> ccdown -c [-o XXX字幕-中文.srt] XXX字幕.zh-CN.json


之后，你的程序文件夹就应该出现了一个XXX字幕-中文.srt字幕文件，把这个字幕文件加载到你喜爱的播放软件吧！
### 或者。。。在下载的同时自动转换
你可以同时指定-d 和 -c选项以达到下载时自动转换为srt的目的。例如：

> ccdown -c -d -s 1 http://bilibili.com/video/BVXXXXXXXXX

> ccdown -c -d http://bilibili.com/video/BVXXXXXXXXX?p=2

> ccdown -c -e 6 -d http://bilibili.com/video/BVXXXXXXXXX
