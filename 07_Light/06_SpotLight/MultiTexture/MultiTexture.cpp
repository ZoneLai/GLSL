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

	_diffuseTexture.Load("container_diffuse.bmp");
	_diffuseTexture.Load("container_specular.bmp");

	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle)) {
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
	_mAngle += 0.0005f;											/**< 更新旋转的角度 */
}

glm::vec3 MultiTexture::getEyePosCircle()
{
	GLfloat radius	= 5.0f;
	GLfloat xPos	= radius * cos(_mAngle);
	GLfloat zPos	= radius * sin(_mAngle);
	return glm::vec3(xPos, 0.0f, zPos);
}

void MultiTexture::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();
	glBindVertexArray(_vaoId);
	glUseProgram(_pShaderProgramHandle);
	// 设置光源属性 FlashLight光源
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.position"), 0.0, 0.0, 2.0);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.direction"), 0.0, 0.0, -1.0);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_light.cutoff"), cos(glm::radians(12.5f)));

	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_light.specular"), 1.0f, 1.0f, 1.0f);
	// 设置衰减系数
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_light.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_light.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_light.quadratic"), 0.032f);
	// 启用diffuseMap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture._textureId);
	glUniform1i(glGetUniformLocation(_pShaderProgramHandle, "u_material.diffuseMap"), 0);
	// 启用specularMap
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _specularTexture._textureId);
	glUniform1i(glGetUniformLocation(_pShaderProgramHandle, "u_material.specularMap"), 1);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_material.shininess"), 32.0f);
	// 投影矩阵
	glm::mat4 projection	= glm::perspective(glm::radians(45.0f), static_cast<float>(640) / static_cast<float>(480), 1.0f, 100.0f);
	// 视变换矩阵
	// glm::vec3 eyePos		= getEyePosCircle();
	glm::mat4 view			= glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	// 绘制多个立方体
	for (auto i = 0; i < sizeof(_cubePostitions) / sizeof(_cubePostitions[0]); ++i) {
		// 模型变换矩阵
		glm::mat4 model(1.0f);
		model			= glm::translate(model, _cubePostitions[i]);
		GLfloat angle	= 20.0f * i;
		model			= glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		model			= glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
}