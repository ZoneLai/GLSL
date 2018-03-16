#include "MultiTexture.h"
#include "GLUtil.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader/sphere_position.frag"
#include "shader/sphere_position.vert"
MultiTexture::MultiTexture(const char * class_name) 
	: GLApplication(class_name)
	, _mAngle(0.0f)
	, _pShaderProgramHandle(0)
	, _vaoId(0)
	, _vboBuffers{0}
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
	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle)) {
		GLfloat vertices[] = {
			-0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 0
			 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 1
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // 2
			 0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f   // 3
		};
		GLushort indices[] = {
			0, 1, 2,  // 第一个三角形
			0, 3, 1   // 第二个三角形
		};
		glGenVertexArrays(1, &_vaoId);
		glBindVertexArray(_vaoId);

		glGenBuffers(2, _vboBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboBuffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// Step4: 指定解析方式  并启用顶点属性
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GL_FLOAT),
			(GLvoid*)0
			);
		glEnableVertexAttribArray(0);
		// 顶点颜色属性
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GL_FLOAT),
			(GLvoid*)(3 * sizeof(GL_FLOAT))
			);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}
	return false;
}

void MultiTexture::Uninit()
{
	glDeleteProgram(_pShaderProgramHandle);
	glDeleteBuffers(2, _vboBuffers);
	glDeleteVertexArrays(1, &_vaoId);
}

void MultiTexture::Update(DWORD milliseconds)
{
	if (_mKeys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();
	}
	_mAngle += (float)(milliseconds) / 20.0f;					/**< 更新旋转的角度 */
}

void MultiTexture::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();
	glBindVertexArray(_vaoId);
	glUseProgram(_pShaderProgramHandle);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
}