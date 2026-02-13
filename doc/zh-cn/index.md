# XE 立体引擎概览
## 构建
生成的二进制文件分为release和debug部分
### 本项目依赖于以下第三方库
调用库列表
|库名|调用内容|
|:--:|:--|
mimalloc|mimalloc 内部实现的 malloc/new/free/delete/realloc
libspng|读写 png 图片
libjpeg|读写 jpeg 图片
SDL3|开窗口
vulkan-headers| 使用vulkan 渲染系统
rfd|打开文件资源管理器
zstd|压缩并打包程序资产
### Windows
* #### Visual Studio (msbuild) 
    通常情况下 windows 默认构建方式是 MD/MDd (即静态链接C/C++依赖库) ，因此在 windows 下 XE 引擎生成的二进制文件在不刻意更改的情况下为 MD(d)方式。
    ##### 构建步骤:
    - 下载 vcpkg。
    + 根据调用库列表下载对应的第三方库。 注：windows构建默认为MD，再加之引擎调用第三方库众多，通过静态链接能减少二进制文件体积，因此应下载库类型为 :x64-windows-static-md。
    + 启动 visual studio并设置graphics为启动项目，并构建此项目，生成的二进制位置为SDK下。

### Linux
## 文件结构概览
```
xe
├─build
│  ├─build-bin
│  ├─cmake : cmake 构建系统
│  └─windows : msbild 构建系统
├─doc
├─inc : 头文件系统
│  ├─core
│  ├─data_system
│  ├─graphics
│  ├─logic
│  └─tools
├─rust : 第三方rust库
├─SDK
├─src
│  └─... 同inc
└─test
```
## 代码
### core 部分
### data_system 部分