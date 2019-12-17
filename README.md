# faceRecognition-jna
基于ncnn的人脸识别算法，使用jna技术在java中调用(Windows版)

1. 编译ncnn：https://github.com/Tencent/ncnn
2. 将ncnn生成的include和lib文件夹复制到项目根路径
3. 修改cmakelist.txt文件，保证ncnn.lib路径正确

> cd <faceRecognition-jna-root-dir>
> mkdir build
> cd build
> cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..
> nmake
