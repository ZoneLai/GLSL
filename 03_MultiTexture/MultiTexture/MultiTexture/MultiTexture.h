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
	GLuint				_mvpMatrixLoc;
	GLuint				_sample2DTextLoc;
	GLuint				_sample2DTextLoc2;
	GLuint				_mixValueLoc;
	GLuint				_vaoId;
	GLuint				_vboBuffers[2];
	glm::mat4			_mvpM;
	glm::mat4			_projectionM;
	glm::mat4			_viewM;
	glm::mat4			_modelM;
	BMPLoader			_mBMPLoader1;
	BMPLoader			_mBMPLoader2;
};

#endif //__MultiTexture_H__