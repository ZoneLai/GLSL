//========================================================
/**
*  @file  GLUtil.h
*
*  ��Ŀ������ ����VR Windows��
*  �ļ�����:  GLSL����ģ��
*  ����ƽ̨�� Windows7/10
*
*  ���ߣ�     LAI ZHONG AN
*  �����ʼ�:  zhonganlai@gmail.com
*  �������ڣ� 20017-07-16
*  �޸����ڣ� 20017-07-16
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