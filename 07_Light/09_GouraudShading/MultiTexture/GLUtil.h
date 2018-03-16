//========================================================
/**
*  @file  GLUtil.h
*
*  项目描述： 万视VR Windows版
*  文件描述:  GLSL编译模块
*  适用平台： Windows7/10
*
*  作者：     LAI ZHONG AN
*  电子邮件:  zhonganlai@gmail.com
*  创建日期： 20017-07-16
*  修改日期： 20017-07-16
*
*/
//========================================================
#ifndef __GLUTIL_H__
#define __GLUTIL_H__
#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif
	extern void CheckOpenGL(char* location);
	extern void CheckFrameBufferStatus();
	extern void CheckGlError(const char* op);
	extern void CheckLocation(int location, char* label);

	extern bool CompileShaderFromString(const char* strShaderSource, GLint hShaderHandle);
	extern bool LinkShaderProgram(unsigned int hShaderProgram);
	extern bool CompileShaderProgram(const char* strVertexShader, const char* strFragmentShader, GLuint* pShaderProgramHandle);
#ifdef __cplusplus
}
#endif

#endif //__GLUTIL_H__