#include "MultiTexture.h"
#include "GLUtil.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader/sphere_position.frag"
#include "shader/sphere_position.vert"
#include "shader/quad_position.frag"
#include "shader/quad_position.vert"
MultiTexture::MultiTexture(const char * class_name) 
	: GLApplication(class_name)
	, _mAngle(0.0f)
	, _pShaderProgramHandle(0)
	, _planeProgramHandle(0)
	, _vaoId{0}
	, _vboBuffers{0}
	, _fboId(0)
	, _colorTextId(0)
	, _depthStencilTextId(0)
	, _textId(0)
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
	_cubeTexture.Load("container.bmp");
	_planeTexture.Load("metal.bmp");

	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle) && CompileShaderProgram(quad_vert, quad_frag, &_planeProgramHandle)) {
		// 指定立方体顶点属性数据 顶点位置 纹理
		GLfloat cubeVertices[] = {
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,		// A
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,		// B
			 0.5f,  0.5f, 0.5f, 1.0f, 1.0f,		// C
			 0.5f,  0.5f, 0.5f, 1.0f, 1.0f,		// C
			-0.5f,  0.5f, 0.5f, 0.0f, 1.0f,		// D
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,		// A

			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,	// E
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,	// H
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,	// G
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,	// G
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,	// F
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,	// E

			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,	// D
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,	// H
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,	// E
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,	// E
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,	// A
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,	// D

			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,		// F
			0.5f,  0.5f, -0.5f, 1.0f, 1.0f,		// G
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f,		// C
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f,		// C
			0.5f, -0.5f,  0.5f, 0.0f, 0.0f,		// B
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,		// F

			 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,		// G
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,		// H
			-0.5f, 0.5f,  0.5f, 0.0f, 0.0f,		// D
			-0.5f, 0.5f,  0.5f, 0.0f, 0.0f,		// D
			 0.5f, 0.5f,  0.5f, 1.0f, 0.0f,		// C
			 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,		// G

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,	// A
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,	// E
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,	// F
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,	// F
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,	// B
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,	// A
		};
		// 地板顶点属性数据 顶点位置 纹理坐标(设置的值大于1.0用于重复)
		GLfloat planeVertices[] = {
			 5.0f, -0.5f,  5.0f, 2.0f, 0.0f,	// A
			 5.0f, -0.5f, -5.0f, 2.0f, 2.0f,	// D
			-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,	// C

			-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,	// C
			-5.0f, -0.5f,  5.0f, 0.0f, 0.0f,	// B
			 5.0f, -0.5f,  5.0f, 2.0f, 0.0f,	// A
		};
		// 用于绘制FBO纹理的矩形顶点属性数据
		GLfloat quadVertices[] = {
			// 位置 纹理坐标
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f
		};

		glGenVertexArrays(3, _vaoId);
		glBindVertexArray(_vaoId[0]);
		glGenBuffers(3, _vboBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		// 顶点位置属性
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
		// 顶点纹理坐标
		glVertexAttribPointer(
			1,
			2, 
			GL_FLOAT, 
			GL_FALSE,
			5 * sizeof(GL_FLOAT),
			(GLvoid*)(3 * sizeof(GL_FLOAT))
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glBindVertexArray(_vaoId[1]);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		// 顶点位置数据
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
		// 顶点纹理数据
		glVertexAttribPointer(
			1, 
			2,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(GL_FLOAT),
			(GLvoid*)(3 * sizeof(GL_FLOAT))
		);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		
		glBindVertexArray(_vaoId[2]);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		// 顶点位置数据
		glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			4 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
		// 顶点纹理数据
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT, 
			GL_FALSE,
			4 * sizeof(GL_FLOAT),
			(GLvoid*)(2 * sizeof(GL_FLOAT))
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glGenFramebuffers(1, &_fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
		glGenTextures(1, &_textId);
		glBindTexture(GL_TEXTURE_2D, _textId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // 预分配空间
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textId, 0);

		glGenTextures(1, &_depthStencilTextId);
		glBindTexture(GL_TEXTURE_2D, _depthStencilTextId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 640, 480, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // 预分配空间
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthStencilTextId, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			MessageBox(HWND_DESKTOP, "FBO not complete!", "Error", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}
	return false;
}

void MultiTexture::Uninit()
{
	glDeleteProgram(_pShaderProgramHandle);
	glDeleteProgram(_planeProgramHandle);
	glDeleteBuffers(3, _vboBuffers);
	glDeleteVertexArrays(3, _vaoId);
	glDeleteFramebuffers(1, &_fboId);
	glDeleteTextures(1, &_depthStencilTextId);
}

void MultiTexture::Update(DWORD milliseconds)
{
	if (_mKeys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();
	}
	_mAngle += 0.0001f;					/**< 更新旋转的角度 */
}

void MultiTexture::Draw()
{
	glUseProgram(_pShaderProgramHandle);
	glm::mat4 projection	= glm::perspective(glm::radians(45.0f), static_cast<float>(640) / static_cast<float>(480), 1.0f, 100.0f);
	glm::mat4 view			= glm::lookAt(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	
	// 启用用户自定义的FBO
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	// 清除颜色缓冲区 重置为指定颜色
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(_vaoId[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _cubeTexture._textureId);
	
	// 绘制第一个立方体
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	// 绘制第二个立方体
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	// 绘制平面
	glBindVertexArray(_vaoId[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _planeTexture._textureId);
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// 恢复默认FBO	在矩形上绘制FBO的纹理
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(_planeProgramHandle);
	glBindVertexArray(_vaoId[2]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textId); // 像使用普通纹理一样需要绑定
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
}