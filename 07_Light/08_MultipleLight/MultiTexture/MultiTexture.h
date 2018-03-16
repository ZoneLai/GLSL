#ifndef __MultiTexture_H__
#define __MultiTexture_H__
#include "GLFrame.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "CBMPLoader.h"
class MultiTexture : GLApplication
{
public:
	bool	Init();							/**< OpenGL的初始化 */
	void	Uninit();						/**< OpenGL的卸载 */
	void	Update(DWORD milliseconds);		/**< 执行OpenGL程序的更新 */
	void	Draw();							/**< 绘制OpenGL场景 */

private:
	friend class GLApplication;				/**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	MultiTexture(const char * class_name);	/**< 构造函数 */
	void				setupLights(glm::vec3* PointLightPositions, int pointLightCnt);
	glm::vec3			getEyePosCircle();
	
	float				_mAngle;
	GLuint				_pShaderProgramHandle;
	GLuint				_pLampProgramHandle;
	GLuint				_vaoId;
	GLuint				_vaoLampId;
	GLuint				_vboBuffers;
	BMPLoader			_diffuseTexture;
	BMPLoader			_specularTexture;
	const glm::vec3		_cubePostitions[10] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	glm::vec3			_pointLightPositions[4] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};
};

#endif //__MultiTexture_H__