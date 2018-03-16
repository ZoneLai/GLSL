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
	
	float				_mAngle;
	GLuint				_pShaderProgramHandle;
	GLuint				_planeProgramHandle;
	GLuint				_vaoId[3];
	GLuint				_vboBuffers[3];
	GLuint				_fboId;
	GLuint				_textId;
	GLuint				_rboId;
	BMPLoader			_cubeTexture;
	BMPLoader			_planeTexture;
};

#endif //__MultiTexture_H__