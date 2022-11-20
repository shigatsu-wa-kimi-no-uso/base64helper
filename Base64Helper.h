#pragma once
/* Base64Helper.h 
// Accomplished on 2022/11/20 
// by kiminouso,Hohai University
*/

#ifndef _BASE64HELPER_H_
#define _BASE64HELPER_H_
typedef unsigned char BYTE;
typedef unsigned long DWORD;

class Base64Helper
{
    const char index2char[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="; //ĩβ�������'='��ʾ���Ŀ�ȱ

    const BYTE char2index[256] = {
         /*0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15 */
         /*                                                 /n             /r           */
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127,  64, 127, 127,  64, 127, 127,//1
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,//2
         /*                                                      +                    / */
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,  62, 127, 127, 127,  63,//3
         /*0    1    2    3    4    5    6    7    8    9                   =           */
          52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 127, 127, 127,  65, 127, 127,//4
         /*     A    B    C    D    E    F    G    H    I    J    K    L    M    N    O */
         127,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,//5
         /*P    Q    R    S    T    U    V    W    X    Y    Z                          */
          15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 127, 127, 127, 127, 127,//6
         /*     a    b    c    d    e    f    g    h    i    j    k    l    m    n    o */
         127,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,//7
         /*p    q    r    s    t    u    v    w    x    y    z                          */
          41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 127, 127, 127, 127, 127,//8
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,//9
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,//10
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,//11
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,//12
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,//13
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,//14
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,//15
         127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127//16
    };

    union datablock
    {
        BYTE b[4];
        DWORD dword;
    }block;

public:

    BYTE* decode(BYTE* base64code,int len,int* outlen);

    BYTE* encode(BYTE* binaryData,int len,int* outlen);


};

#endif