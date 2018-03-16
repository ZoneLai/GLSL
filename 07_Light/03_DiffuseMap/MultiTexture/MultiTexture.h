#ifndef __MultiTexture_H__
#define __MultiTexture_H__
#include "GLFrame.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
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
	GLuint				_pLampProgramHandle;
	GLuint				_vaoId;
	GLuint				_vaoLampId;
	GLuint				_vboBuffers;
	BMPLoader			_diffuseTexture;
	BMPLoader			_specularTexture;
};

#endif //__MultiTexture_H__