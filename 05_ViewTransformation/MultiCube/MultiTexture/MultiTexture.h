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
	glm::vec3			getEyePosCircle();
	glm::vec3			getEyePosSphere();
	
	float				_mAngle;
	float				_mAngle2;
	GLuint				_pShaderProgramHandle;
	GLuint				_pAxisProgramHandle;
	GLuint				_vaoId;
	GLuint				_vboBuffers;
	BMPLoader			_mBMPLoader1;
	const glm::vec3		_cubePostitions[7] = {
		glm::vec3(0.0f, 0.0f, 1.2f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.2f, 1.2f, 0.0f),
		glm::vec3(-1.2f, 1.2f, 0.0f),
		glm::vec3(-1.2f, -1.5f, 0.0f),
		glm::vec3(1.2f, -1.5f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.2f),
	};
	bool				_isCircleEyePos;
};

#endif //__MultiTexture_H__