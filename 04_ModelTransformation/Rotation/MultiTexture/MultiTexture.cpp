#include "MultiTexture.h"
#include "GLUtil.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader/sphere_position.frag"
#include "shader/sphere_position.vert"
#include "shader/axis.frag"
#include "shader/axis.vert"

MultiTexture::MultiTexture(const char * class_name) 
	: GLApplication(class_name)
	, _mAngle(0.0f)
	, _pShaderProgramHandle(0)
	, _pAxisProgramHandle(0)
	, _vaoId{0}
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
	_mBMPLoader1.Load("cat.bmp");
	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle) && CompileShaderProgram(axis_vert, axis_frag, &_pAxisProgramHandle)) {
		GLfloat vertices[] = {
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
			0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
			0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 3
		};
		GLushort indices[] = {
			0, 1, 2,  // 第一个三角形
			0, 2, 3   // 第二个三角形
		};
		// 指定坐标轴三角形箭头属性数据 顶点 颜色
		GLfloat axisTriangleData[] = {
			 0.945f,     0.03125f,  0.0f,   1.0f, 0.0f, 0.0f, // +x轴三角形
			 1.0f,       0.0f,      0.0f,   1.0f, 0.0f, 0.0f,
			 0.945f,    -0.03125f,  0.0f,   1.0f, 0.0f, 0.0f,
			-0.03125f,   0.945f,    0.0f,   0.0f, 1.0f, 0.0f,// +y轴三角形
			 0.0f,       1.0f,      0.0f,   0.0f, 1.0f, 0.0f,
			 0.03125f,   0.945f,    0.0f,   0.0f, 1.0f, 0.0f,
			-0.03125f,   0.0f,      0.945f, 0.0f, 0.0f, 1.0f,// +z轴三角形
			 0.0f,       0.0f,      1.0f,   0.0f, 0.0f, 1.0f,
			 0.03125f,   0.0f,      0.945f, 0.0f, 0.0f, 1.0f,
		};
		// 指定坐标轴直线属性数据 顶点 颜色
		GLfloat axisLineData[] = {
			-1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
			 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
			 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
			 0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
			 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f
		};
		glGenVertexArrays(3, _vaoId);
		glBindVertexArray(_vaoId[0]);
		glGenBuffers(4, _vboBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboBuffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
		// 准备坐标轴箭头
		glBindVertexArray(_vaoId[1]);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(axisTriangleData), axisTriangleData, GL_STATIC_DRAW);
		glVertexAttribPointer(
			0, 
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
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
		// 准备坐标轴直线
		glBindVertexArray(_vaoId[2]);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(axisLineData), axisLineData, GL_STATIC_DRAW);
		glVertexAttribPointer(
			0, 
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
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
	glDeleteBuffers(4, _vboBuffers);
	glDeleteVertexArrays(3, _vaoId);
}

void MultiTexture::Update(DWORD milliseconds)
{
	if (_mKeys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();
	}
	_mAngle +=  0.05f;											/**< 更新旋转的角度 */
}

void MultiTexture::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();
	glBindVertexArray(_vaoId[0]);
	glUseProgram(_pShaderProgramHandle);
	glm::mat4 projection(1.0f);		// 投影变换矩阵
	glm::mat4 view(1.0f);			// 视变换矩阵
	glm::mat4 model(1.0f);			// 模型变换矩阵
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	// 启用纹理单元 绑定纹理对象
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _mBMPLoader1._textureId);
	// 设置纹理单元为0号
	glUniform1i(glGetUniformLocation(_pShaderProgramHandle, "s_texture"), 0);
	// 绘制第一个矩形
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	// 绘制第二个矩形
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	// 绘制第三个矩形
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
	model = glm::translate(model, glm::vec3(0.25f, 0.25f, 0.0f));
	model = glm::translate(model, glm::vec3(-0.25f, -0.25f, 0.0f));
	model = glm::rotate(model, glm::radians(_mAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	// 绘制第四个矩形
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(_mAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	// 绘制坐标轴
	glBindVertexArray(_vaoId[1]);
	glUseProgram(_pAxisProgramHandle);
	glDrawArrays(GL_TRIANGLES, 0, 9);
	glBindVertexArray(_vaoId[2]);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
}