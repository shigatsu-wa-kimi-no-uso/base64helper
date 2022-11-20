/* Base64Helper.cpp
// Completed on 2022/11/20
// by kiminouso,Hohai University
*/
#include "Base64Helper.h"

BYTE* Base64Helper::decode(BYTE* base64code, int len, int* outlen)
{
	int binLen;
	BYTE* bin;

	block.dword = 0;

	BYTE* const code = new BYTE[len];

	/*检测合法性,过滤换行符并转换为索引码,*/
	int j = 0;
	for (int i = 0; i < len - 2; i++)
	{
		if (char2index[base64code[i]] > 64) return 0;
		if (char2index[base64code[i]] == 64) continue; //换行符在char2index中对应为64
		code[j] = char2index[base64code[i]];
		j++;
	}

	/*对于最后2个字符,'='也是合合法的*/
	for (int i = len - 2; i < len; i++)
	{
		if (char2index[base64code[i]] > 65) return 0; //65为'='在char2index中对应的值
		if (char2index[base64code[i]] == 64) continue;
		code[j] = char2index[base64code[i]];
		j++;
	}

	if (j % 4 != 0) return 0;			//base64码长度一定是4的倍数(去掉换行后)

	int blockCnt = j / 4;	//base64数据块数

	/*先处理最后4个字符*/
	/*情况1: 最后4个字符有2个'='(code==65),说明原始数据在最后缺2字节才能凑足3字节*/
	/*情况2: 最后4个字符有1个'=',说明原始数据在最后缺1字节才能凑足3字节*/
	/*情况3: 最后4个字符中没有'=',说明原始数据长度是3的倍数*/
	if (code[len - 2] < 64 && code[len - 1] < 64)
	{
		binLen = ((len * 6) / 8);
		bin = new BYTE[binLen];
		block.dword |= ((code[len - 4] << 18) |
						(code[len - 3] << 12) |
						(code[len - 2] << 6) |
						(code[len - 1]));
		bin[binLen - 3] = block.b[2];
		bin[binLen - 2] = block.b[1];
		bin[binLen - 1] = block.b[0];
	}
	else if (code[len - 2] == 65 && code[len - 1] == 65) //过滤最后两个字符是'=' '其他'的情况
	{
		binLen = ((len * 6) / 8) - 2;
		bin = new BYTE[binLen];
		block.dword |= ((code[len - 4] << 18) | (code[len - 3] << 12));
		bin[binLen - 1] = block.b[2];
	}
	else if (code[len - 1] == 65)
	{
		binLen = ((len * 6) / 8) - 1;
		bin = new BYTE[binLen];
		block.dword |= ((code[len - 4] << 18) | (code[len - 3] << 12) | (code[len - 2] << 6));
		bin[binLen - 2] = block.b[2];
		bin[binLen - 1] = block.b[1];
	}
	else return 0;			//其他情况为不合法的base64


	/*对除去末尾的4个字符的其他字符解码*/
	int off = 0;
	for (int i = 0; i < len - 4; i += 4)
	{
		block.dword = 0;
		block.dword |= ((code[i] << 18) |
					(code[i + 1] << 12) |
					(code[i + 2] << 6) |
					(code[i + 3]));
		bin[off] = block.b[2];
		bin[off+1] = block.b[1];
		bin[off+2] = block.b[0];
		off += 3;
	}

	delete[] code;
	*outlen = binLen;
	return bin;
}

BYTE* Base64Helper::encode(BYTE* binaryData, int len, int* outlen)
{
	int blockCnt = len / 3;						//base64 3个原始数据为一组,对应4个base64码
	int remainder = len % 3;	//还剩多少字节没有被表示
	if (remainder > 0) blockCnt++;

	datablock* base64block = new datablock[blockCnt];
	
	datablock temp24bit;
	DWORD mask[4];

	mask[0] = 0x00'FC'00'00;			//掩码
	mask[1] = 0x00'03'F0'00;
	mask[2] = 0x00'00'0F'C0;
	mask[3] = 0x00'00'00'3F;
	int off = 0;

	for (int i = 0; i < blockCnt - 1 ; i++)
	{
		base64block[i].dword = 0;
		temp24bit.b[0] = binaryData[off + 2];		//存入一个dword中的低位
		temp24bit.b[1] = binaryData[off + 1];
		temp24bit.b[2] = binaryData[off];  

		base64block[i].b[0] = (temp24bit.dword & mask[0]) >> 18;//过滤出19-24位
		base64block[i].b[1] = (temp24bit.dword & mask[1]) >> 12;//过滤出13-18位
		base64block[i].b[2] = (temp24bit.dword & mask[2]) >> 6; //过滤出7-12位 
		base64block[i].b[3] = (temp24bit.dword & mask[3]);	  //过滤出低6位
		off += 3;
	}

	switch (remainder)
	{
	case 1:
		base64block[blockCnt - 1].dword = 0;
		temp24bit.b[1] = 0;						//排除前面的干扰
		temp24bit.b[2] = binaryData[off];		//从高位存入一个dword
		base64block[blockCnt - 1].b[0] = (temp24bit.dword & mask[0]) >> 18;//过滤出19-24位
		base64block[blockCnt - 1].b[1] = (temp24bit.dword & mask[1]) >> 12;//过滤出13-18位 
		base64block[blockCnt - 1].b[2] = 64;			//写入特殊标记,待译码时翻译成'='
		base64block[blockCnt - 1].b[3] = 64;
		break;
	case 2:
		base64block[blockCnt - 1].dword = 0;
		temp24bit.b[0] = 0;							//排除前面的干扰
		temp24bit.b[1] = binaryData[off + 1];		
		temp24bit.b[2] = binaryData[off];		//从高位存入一个dword
		base64block[blockCnt - 1].b[0] = (temp24bit.dword & mask[0]) >> 18;	//过滤出19-24位
		base64block[blockCnt - 1].b[1] = (temp24bit.dword & mask[1]) >> 12;//过滤出13-18位 
		base64block[blockCnt - 1].b[2] = (temp24bit.dword & mask[2]) >> 6;//过滤出7-12位	
		base64block[blockCnt - 1].b[3] = 64;		//写入特殊标记,待译码时翻译成'='
		break;
	case 0:
		base64block[blockCnt - 1].dword = 0;
		temp24bit.b[0] = binaryData[off + 2];		//存入一个dword中的低位
		temp24bit.b[1] = binaryData[off + 1];
		temp24bit.b[2] = binaryData[off];

		base64block[blockCnt - 1].b[0] = (temp24bit.dword & mask[0]) >> 18;//过滤出19-24位
		base64block[blockCnt - 1].b[1] = (temp24bit.dword & mask[1]) >> 12;//过滤出13-18位
		base64block[blockCnt - 1].b[2] = (temp24bit.dword & mask[2]) >> 6; //过滤出7-12位 
		base64block[blockCnt - 1].b[3] = (temp24bit.dword & mask[3]);	  //过滤出低6位
		break;
	}

	for (int i = 0; i < blockCnt; i++)
	{
		base64block[i].b[0] = index2char[base64block[i].b[0]];
		base64block[i].b[1] = index2char[base64block[i].b[1]];
		base64block[i].b[2] = index2char[base64block[i].b[2]];
		base64block[i].b[3] = index2char[base64block[i].b[3]];
	}

	*outlen = blockCnt * 4;
	return (BYTE*)base64block;
}