# base64编码解码器

#### 介绍
base64编码解码器
https://zhuanlan.zhihu.com/p/585153904
#### 文件说明

1.  Base64Helper.h base64解码编码器所用的类Base64Helper的声明
2.  Base64Helper.cpp base64解码编码器所用的类Base64Helper的实现
3.  main.cpp   应用类Base64Helper的命令行程序
4.  base64helper.exe  以上文件编译出的程序

base64helper.exe 用法：

base64helper  -tobase64 [sourcefile] [outputfile]  将源文件编码成base64
base64helper  -tobin [sourcefile] [outputfile]  将源文件解码成二进制文件

正确性个人测试暂未发现问题，可以自己下载使用下。

Base64Helper类用法
需包含Base64Helper.h并将Base64Helper.cpp添加至编译文件中，
或者直接包含内联函数版Base64Helper.h

定义一个Base64Helper类的对象即可使用 
成员函数用法:

BYTE* decode(BYTE* base64code, int len, int* outlen)

其中 typedef unsigned char BYTE

返回一个解码的unsigned char数组的首指针，解码失败返回NULL
参数：
BYTE* base64code [输入参数]含有base64字符的要解码的内容，可以包含换行符，不得包含任何非base64字符的其他字符
int len [输入参数] 参数 base64code 的长度
int* outlen [输出参数]输出返回值unsigned char数组的数组的大小

BYTE* encode(BYTE* binaryData, int len, int* outlen)

返回一个编码成base64的unsigned char数组的首指针，编码失败返回NULL
参数：
BYTE* binaryData [输入参数]要编码的数据
int len [输入参数]参数 binaryData 的长度
int* outlen [输出参数]输出返回值unsigned char数组的数组的大小


Completed on 2022/11/20 by kiminouso,Hohai University
