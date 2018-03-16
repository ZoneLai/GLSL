#include "MultiTexture.h"
#include "GLUtil.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader/sphere_position.frag"
#include "shader/sphere_position.vert"
MultiTexture::MultiTexture(const char * class_name) 
	: GLApplication(class_name)
	, _mAngle(0.0f)
	, _pShaderProgramHandle(0)
	, _mvpMatrixLoc(0)
	, _sample2DTextLoc(0)
	, _sample2DTextLoc2(0)
	, _mixValueLoc(0)
	, _vaoId(0)
	, _vboBuffers{0}
	, _mvpM(1.0f)
	, _projectionM(1.0f)
	, _viewM(1.0f)
	, _modelM(1.0f)
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
	_mBMPLoader1.Load("wood.bmp");
	_mBMPLoader2.Load("cat.bmp");
	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle)) {
		_mvpMatrixLoc		= glGetUniformLocation(_pShaderProgramHandle, "u_mvpMatrix");
		_sample2DTextLoc	= glGetUniformLocation(_pShaderProgramHandle, "s_texture1");
		_sample2DTextLoc2	= glGetUniformLocation(_pShaderProgramHandle, "s_texture2");
		_mixValueLoc		= glGetUniformLocation(_pShaderProgramHandle, "u_mixValue");
		_modelM				= glm::scale(_modelM, glm::vec3(4.0f, 4.0f, 4.0f));
		_viewM				= glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		_projectionM		= glm::perspective(glm::radians(45.0f), static_cast<float>(640) / static_cast<float>(480), 0.1f, 100.0f);
		_mvpM				= _projectionM * _viewM * _modelM;
		GLfloat vertices[]	= {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 3
		};
		GLushort indices[] = {
			0, 1, 2,  // 第一个三角形
			0, 2, 3   // 第二个三角形
		};
		glGenVertexArrays(1, &_vaoId);
		glBindVertexArray(_vaoId);
		glGenBuffers(2, _vboBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboBuffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
		// 顶点颜色属性
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)(3 * sizeof(GL_FLOAT))
		);
		glEnableVertexAttribArray(1);
		// 顶点纹理坐标
		glVertexAttribPointer(
			2,
			2, 
			GL_FLOAT, 
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)(6 * sizeof(GL_FLOAT))
		);
		glEnableVertexAttribArray(2);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 注意不要解除GL_ELEMENT_ARRAY_BUFFER绑定
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
	glUseProgram(_pShaderProgramHandle);
	glUniformMatrix4fv(_mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(_mvpM));
	
	glBindVertexArray(_vaoId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _mBMPLoader1._textureId);
	glUniform1i(_sample2DTextLoc, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _mBMPLoader2._textureId);
	glUniform1i(_sample2DTextLoc2, 1);

	glUniform1f(_mixValueLoc, 0.5);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glFlush();
}