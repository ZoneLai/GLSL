#include "MultiTexture.h"
#include "GLUtil.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader/sphere_position.frag"
#include "shader/sphere_position.vert"

MultiTexture::MultiTexture(const char * class_name) 
	: GLApplication(class_name)
	, _mAngle(0.0f)
	, _mAngle2(0.0f)
	, _pShaderProgramHandle(0)
	, _pAxisProgramHandle(0)
	, _vaoId(0)
	, _vboBuffers(0)
	, _isCircleEyePos(true)
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
	_mBMPLoader1.Load("cat.bmp");
	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle)) {
		// 指定顶点属性数据 顶点位置 颜色 纹理
		GLfloat vertices[] = {
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
			 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// B
			 0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// C
			 0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// C
			-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A

			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// E
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // H
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// E

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // H
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// E
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// E
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D

			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F
			0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   // C
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   // C
			0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	// B
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F

			 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // H
			-0.5f, 0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// D
			-0.5f, 0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// D
			 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,	// C
			 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,	// E
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// F
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// F
			 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// B
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
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
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}
	return false;
}

void MultiTexture::Uninit()
{
	glDeleteProgram(_pShaderProgramHandle);
	glDeleteBuffers(1, &_vboBuffers);
	glDeleteVertexArrays(1, &_vaoId);
}

void MultiTexture::Update(DWORD milliseconds)
{
	if (_mKeys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();
	}
	if (_mKeys.IsPressed(VK_F1) == true)						/**< 切换 */
	{
		_isCircleEyePos = true;
	}
	if (_mKeys.IsPressed(VK_F2) == true)						/**< 切换 */
	{
		_isCircleEyePos = false;
	}

	_mAngle		+= 0.0005f;										/**< 更新旋转的角度 */
	_mAngle2	+= 0.0005f;
}

glm::vec3 MultiTexture::getEyePosCircle()
{
	GLfloat radius	= 5.0f;
	GLfloat xPos	= radius * cos(_mAngle);
	GLfloat zPos	= radius * sin(_mAngle);
	return glm::vec3(xPos, 0.0f, zPos);
}

glm::vec3 MultiTexture::getEyePosSphere()
{
	GLfloat radius	= 5.0f;
	GLfloat theta	= _mAngle;
	GLfloat phi		= _mAngle2 / 2.0f;
	GLfloat xPos	= radius * sin(theta) * cos(phi);
	GLfloat yPos	= radius * sin(theta) * sin(phi);
	GLfloat zPos	= radius * cos(theta);
	return glm::vec3(xPos, yPos, zPos);
}

void MultiTexture::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();
	glBindVertexArray(_vaoId);
	glUseProgram(_pShaderProgramHandle);
	// 投影矩阵
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(640) / static_cast<float>(480), 1.0f, 100.0f);
	// 视变换矩阵
	glm::vec3 eyePos(1.0f);
	if (_isCircleEyePos)
		eyePos = getEyePosCircle();
	else
		eyePos = getEyePosSphere();
	glm::mat4 view = glm::lookAt(eyePos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));		
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	// 启用纹理单元 绑定纹理对象
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _mBMPLoader1._textureId);
	// 设置纹理单元为0号
	glUniform1i(glGetUniformLocation(_pShaderProgramHandle, "s_texture"), 0);
	// 绘制多个立方体
	for (auto i = 0; i < sizeof(_cubePostitions) / sizeof(_cubePostitions[0]); ++i) {
		// 模型变换矩阵
		glm::mat4  model = glm::mat4(1.0f);
		model = glm::translate(model, _cubePostitions[i]);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
}