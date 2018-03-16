#ifndef __STDAFX_H__
#define __STDAFX_H__
#include <windows.h>
#include <stdio.h>
#include <math.h>                         
#include <time.h>
#include <atlimage.h>
#include <io.h>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#include "Debug.h"

#ifdef _WIN64
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glut64.lib")
#else
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glut32.lib")
#endif // _WIN64

#define NS_SP_BEGIN                     namespace simope {
#define NS_SP_END                       }
#define USING_NS_SP                     using namespace simope
#define STRINGIFY(A)					#A
#define PANO_APICALL					__declspec(dllexport)
#define __EXTERN_C						extern "C"
#define PANO_APIENTRY					__stdcall

#pragma warning(disable: 4316)
#pragma warning(disable: 4996)

#endif