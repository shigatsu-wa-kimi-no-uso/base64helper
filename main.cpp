/* main.cpp (applying Base64Helper)
// Completed on 2022/11/20
// by kiminouso,Hohai University
*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include"Base64Helper.h"
#define BUFFER_SIZE 1048576

char buf[BUFFER_SIZE];

int main(int argc,char** argv)
{
	Base64Helper helper;
	const char* cmdhelp = "-help";
	const char* cmdtobase64 = "-tobase64";
	const char* cmdtobin = "-tobin";

	const char* tobase64usage = "-tobase64 [sourcefile] [outputfile]";
	const char* tobinusage = "-tobin [sourcefile] [outputfile]";
	
	if (argc < 4)
	{
		printf("Usage:\n\n");
		printf("%-40s Display this summary\n",cmdhelp);
		printf("%-40s Encode source file [sourcefile] into base64 text and output to specified file [outputfile].\n", tobase64usage);
		printf("%-40s Decode source file [sourcefile] into binary file and output to specified file [outputfile].\n", tobinusage);
		printf("Any source file should be under 1048576 bytes (1MB).\n");
		return 1;
	}
	FILE* fin, * fout;
	DWORD srcSize;
	int outputlen;
	BYTE* output;

	if (strcmp(argv[1], cmdtobase64) == 0)
	{
		fin = fopen(argv[2], "rb");
		if (fin == NULL)
		{
			fprintf(stderr, "open file failed.");
			return 1;
		}
		fseek(fin, 0, SEEK_END);
		srcSize = ftell(fin);
		if (srcSize > BUFFER_SIZE)
		{
			fprintf(stderr, "Any source file should be under 1048576 bytes (1MB).\n");
			return 1;
		}
		fseek(fin, 0, SEEK_SET);
		if (1 != fread(buf, srcSize, 1, fin))
		{
			printf("read file failed.");
			return 1;
		}
		printf("source file size:%d bytes\n", srcSize);
		output=helper.encode((BYTE*)buf, srcSize, &outputlen);
		if (output == NULL)
		{
			fprintf(stderr, "encode file failed.");
			return 1;
		}
		fout = fopen(argv[3], "w");
		if (fout == NULL)
		{
			fprintf(stderr, "write file failed.");
			return 1;
		}
		if (1 != fwrite(output, outputlen, 1, fout))
		{
			fprintf(stderr, "write file failed.");
			return 1;
		}
		printf("output size:%d bytes\n", outputlen);
	}
	else if (strcmp(argv[1], cmdtobin) == 0)
	{
		fin = fopen(argv[2], "rb");
		if (fin == NULL)
		{
			fprintf(stderr, "open file failed.");
			return 1;
		}
		fseek(fin, 0, SEEK_END);
		srcSize = ftell(fin);
		if (srcSize > BUFFER_SIZE)
		{
			fprintf(stderr, "Any source file should be under the maximum size 1048576 bytes (1MB).\n");
			return 1;
		}
		fseek(fin, 0, SEEK_SET);
		if (1 != fread(buf, srcSize, 1, fin))
		{
			printf("read file failed.");
			return 1;
		}
		printf("source file size:%d bytes\n", srcSize);
		output = helper.decode((BYTE*)buf, srcSize, &outputlen);
		if (output == NULL)
		{
			fprintf(stderr, "decode file failed.");
			return 1;
		}
		fout = fopen(argv[3], "wb");
		if (fout == NULL)
		{
			fprintf(stderr, "write file failed.");
			return 1;
		}
		if (1 != fwrite(output, outputlen, 1, fout))
		{
			fprintf(stderr, "write file failed.");
			return 1;
		}
		printf("output size:%d bytes\n", outputlen);
	}

	return 0;
}
