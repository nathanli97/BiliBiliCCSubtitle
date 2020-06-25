# BiliBiliCCSubtitle:一个用于下载B站CC字幕及转换的工具

### 运行环境要求：
> Windows 10，X86——64
### 编译依赖：
> libcurl 和 jsoncpp
>
## 使用说明
你可以在[这里](https://github.com/nathanli97/BiliBiliCCSubtitle/releases/download/1.0.0/BilibiliCCSubtitle-1.0.0-x86_64.zip)下载到最新版本程序。或者克隆整个库进行编译。

第一步：下载CC字幕。
> ccdown [-o outputfile] url

这里的URL指的是带有CC字幕的BiliBili播放页地址。比如说这个转送门：
[https://www.bilibili.com/video/BV1JE411N7UD](https://www.bilibili.com/video/BV1JE411N7UD)

outputfile指的是下载后保存的文件名。默认情况下，文件名需要以.json结尾(当然，如果你没有以json结尾的话程序会帮你补一个json后缀上去)。

一般情况下，下载的字幕文件名随着字幕语言的不同而不同。比如说，如果你想要下载的CC字幕提供了中文和英文两种语言可选，那么当你执行了下面的指令：

> ccdown -o XXX字幕.json http://bilibili/video/BVXXXXXXXXX

之后，程序所在的文件夹会多出两个文件：XXX字幕.zh-CN.json和XXX字幕.en-US.json。

第二步：转换CC字幕

由于B站的字幕文件使用JSON格式存储的，所以你可以用下面这条指令将其转换为普通字幕文件：

cctosrt -o XXX字幕-中文.srt XXX字幕.zh-CN.json

之后，你的程序文件夹就应该出现了一个XXX字幕-中文.srt字幕文件，把这个字幕文件加载到你喜爱的播放软件吧！
