#include "MultiTexture.h"
#include "GLUtil.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader/sphere_position.frag"
#include "shader/sphere_position.vert"
#include "shader/lamp_position.frag"
#include "shader/lamp_position.vert"
MultiTexture::MultiTexture(const char * class_name) 
	: GLApplication(class_name)
	, _mAngle(0.0f)
	, _pShaderProgramHandle(0)
	, _pLampProgramHandle(0)
	, _vaoId(0)
	, _vaoLampId(0)
	, _vboBuffers(0)
{
}

GLApplication * GLApplication::Create(const char * class_name)
{
	MultiTexture* example = new MultiTexture(class_name);
	return reinterpret_cast<GLApplication *>(example);
}

bool MultiTexture::Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	ResizeDraw(true);
	GLenum err = glewInit();
	if (GLEW_OK != err)
		return false;
	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle) && CompileShaderProgram(lamp_vert, lamp_frag, &_pLampProgramHandle)) {
		// 指定顶点属性数据 顶点位置 纹理 法向量
		GLfloat vertices[] = {
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// A
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// B
			 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// C
			 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// C
			-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// D
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// A

			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,	// E
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // H
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,	// G
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,	// G
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,	// F
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,	// E

			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,	// D
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // H
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,	// E
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,	// E
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,	// A
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,	// D

			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// F
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	// G
			 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,	// C
			 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,	// C
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// B
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// F

			 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// G
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // H
			-0.5f, 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// D
			-0.5f, 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// D
			 0.5f, 0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// C
			 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// G

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // A
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,  // E
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,  // F
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,  // F
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // B
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // A
		};
		glGenVertexArrays(1, &_vaoId);
		glBindVertexArray(_vaoId);
		glGenBuffers(1, &_vboBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 顶点位置属性
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
		// 顶点纹理坐标
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)(3 * sizeof(GL_FLOAT))
		);
		glEnableVertexAttribArray(1);
		// 顶点法向量属性
		glVertexAttribPointer(
			2,
			3, 
			GL_FLOAT, 
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)(5 * sizeof(GL_FLOAT))
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		// 创建光源的VAO
		glGenVertexArrays(1, &_vaoLampId);
		glBindVertexArray(_vaoLampId);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers); // 重用上面的数据 无需重复发送顶点数据
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);				// 只需要顶点位置即可
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}
	return false;
}

void MultiTexture::Uninit()
{
	glDeleteProgram(_pShaderProgramHandle);
	glDeleteProgram(_pLampProgramHandle);
	glDeleteBuffers(1, &_vboBuffers);
	glDeleteVertexArrays(1, &_vaoId);
	glDeleteVertexArrays(1, &_vaoLampId);
}

void MultiTexture::Update(DWORD milliseconds)
{
	if (_mKeys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();
	}
	_mAngle += 0.0001f;											/**< 更新旋转的角度 */
}

void MultiTexture::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	glLoadIdentity();
	glBindVertexArray(_vaoId);
	glUseProgram(_pShaderProgramHandle);
	glm::vec3 lightColor(1.0f);
	lightColor.x			= sin(_mAngle * 2.0f);
	lightColor.y			= sin(_mAngle * 0.7f);
	lightColor.z			= sin(_mAngle * 1.3f);
	glm::vec3 diffuseColor	= lightColor   * glm::vec3(0.5f); // 适当减小影响
	glm::vec3 ambientColor	= diffuseColor * glm::vec3(0.2f);
	// 设置光源属性 
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.specular"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.position"), 0.8f, 1.8f, 0.5f);
	// 设置材料光照属性
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_material.ambient"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_material.diffuse"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_material.specular"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_material.shininess"), 32.0f);
	// 投影矩阵
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(640) / static_cast<float>(480), 1.0f, 100.0f);
	// 视变换矩阵
	glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// 模型变换矩阵	
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	// 绘制第一个矩形
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 绘制光源 用立方体代表
	glBindVertexArray(_vaoLampId);
	glUseProgram(_pLampProgramHandle);
	glUniformMatrix4fv(glGetUniformLocation(_pLampProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pLampProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glm::mat4 lampModel = glm::mat4(1.0f);
	lampModel			= glm::translate(lampModel, glm::vec3(0.5f, 1.2f, 0.5f));
	lampModel			= glm::scale(lampModel, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(glGetUniformLocation(_pLampProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(lampModel));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
}