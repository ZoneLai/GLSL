#ifndef __MultiTexture_H__
#define __MultiTexture_H__
#include "GLFrame.h"
#include "DDSFile.h"
#include "CBMPLoader.h"

class MultiTexture : GLApplication
{
public:
	bool	Init();							/**< OpenGL�ĳ�ʼ�� */
	void	Uninit();						/**< OpenGL��ж�� */
	void	Update(DWORD milliseconds);		/**< ִ��OpenGL����ĸ��� */
	void	Draw();							/**< ����OpenGL���� */

private:
	friend class GLApplication;				/**< ����Ϊ����һ����Ԫ�࣬�����������������ʵ����������GL_Application * GL_Application::Create(const char * class_name) */
	MultiTexture(const char * class_name);	/**< ���캯�� */
	
	float				_mAngle;
	GLuint				_pShaderProgramHandle;
	GLuint				_skyBoxProgramHandle;
	GLuint				_vaoId[2];
	GLuint				_vboBuffers[2];
	DDSFile				_mDDSFile;
	BMPLoader			_mCbue;
};

#endif //__MultiTexture_H__