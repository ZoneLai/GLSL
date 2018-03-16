#pragma once
#pragma warning(disable: 4996)
#ifndef __AFX_H__
//#define _T(x) x
#if defined _DEBUG
#include <stdarg.h> 
#include <stdio.h>	/* vsprintf */
#include <crtdbg.h>
#include <windows.h>
#define DPRINTF_BUF_SZ 1024
#define ASSERT(expr)\
do{\
if (!(expr)&&(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)))\
{__asm { int 3 };}\
}while(0)

static __inline void TRACE(char *fmt, ...)
{
	va_list args;
	char buf[DPRINTF_BUF_SZ];
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	OutputDebugString(buf);
	// FILE *fp = fopen("d:\\test.txt","a");
	// fwrite(buf,strlen(buf),1,fp);
	// fclose(fp);
}
#else
static __inline void TRACE(char *fmt, ...) {}
#define ASSERT(expr) if (expr) {NULL;}
#endif
#endif