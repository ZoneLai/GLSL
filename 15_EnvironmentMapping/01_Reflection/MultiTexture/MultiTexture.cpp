#include "MultiTexture.h"
#include "GLUtil.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader/skybox_position.frag"
#include "shader/skybox_position.vert"
#include "shader/scene_position.frag"
#include "shader/scene_position.vert"

MultiTexture::MultiTexture(const char * class_name) 
	: GLApplication(class_name)
	, _mAngle(0.0f)
	, _pShaderProgramHandle(0)
	, _skyBoxProgramHandle(0)
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
	ResizeDraw(true);
	GLenum err = glewInit();
	if (GLEW_OK != err)
		return false;
	if (CompileShaderProgram(scene_vert, scene_frag, &_pShaderProgramHandle) && CompileShaderProgram(skybox_vert, skybox_frag, &_skyBoxProgramHandle)) {
		//Section1 顶点属性数据
		// 指定立方体顶点属性数据 顶点位置 纹理
		GLfloat cubeVertices[] = {
			// 背面 ADC CBA
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

			// 正面EFG GHE
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

			// 左侧面 HDA AEH
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

			// 右侧面 GFB BCG
			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

			// 底面 ABF FEA
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

			// 顶面 DHG GCD
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};
		// 指定包围盒的顶点属性 位置
		GLfloat skyboxVertices[] = {
			// 背面
			-1.0f,  1.0f, -1.0f,	// A
			-1.0f, -1.0f, -1.0f,	// B
			 1.0f, -1.0f, -1.0f,	// C
			 1.0f, -1.0f, -1.0f,	// C
			 1.0f,  1.0f, -1.0f,	// D
			-1.0f,  1.0f, -1.0f,	// A

			// 左侧面
			-1.0f, -1.0f,  1.0f,	// E
			-1.0f, -1.0f, -1.0f,	// B
			-1.0f,  1.0f, -1.0f,	// A
			-1.0f,  1.0f, -1.0f,	// A
			-1.0f,  1.0f,  1.0f,	// F
			-1.0f, -1.0f,  1.0f,	// E

			// 右侧面
			1.0f, -1.0f, -1.0f,		// C
			1.0f, -1.0f,  1.0f,		// G
			1.0f,  1.0f,  1.0f,		// H
			1.0f,  1.0f,  1.0f,		// H
			1.0f,  1.0f, -1.0f,		// D
			1.0f, -1.0f, -1.0f,		// C

			// 正面
			-1.0f, -1.0f, 1.0f,		// E
			-1.0f,  1.0f, 1.0f,		// F
			 1.0f,  1.0f, 1.0f,		// H
			 1.0f,  1.0f, 1.0f,		// H
			 1.0f, -1.0f, 1.0f,		// G
			-1.0f, -1.0f, 1.0f,		// E

			// 顶面
			-1.0f, 1.0f, -1.0f,		// A
			 1.0f, 1.0f, -1.0f,		// D
			 1.0f, 1.0f,  1.0f,		// H
			 1.0f, 1.0f,  1.0f,		// H
			-1.0f, 1.0f,  1.0f,		// F
			-1.0f, 1.0f, -1.0f,		// A

			 // 底面
			-1.0f, -1.0f, -1.0f,	// B
			-1.0f, -1.0f,  1.0f,	// E
			 1.0f, -1.0f,  1.0f,    // G
			 1.0f, -1.0f,  1.0f,    // G
			 1.0f, -1.0f, -1.0f,	// C
			-1.0f, -1.0f, -1.0f,	// B
		};

		// Section2 准备缓存对象
		glGenVertexArrays(2, _vaoId);
		glBindVertexArray(_vaoId[0]);
		glGenBuffers(2, _vboBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		// 顶点位置属性
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			6 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
		// 法线数据
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
		
		glBindVertexArray(_vaoId[1]);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		// 顶点位置数据
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(GL_FLOAT),
			(GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Section3 加载纹理
		_mCbue.Load("pic/container.bmp");
		const char* faces[] = {
			"pic/urbansp_rt.tga" ,
			"pic/urbansp_lf.tga",
			"pic/urbansp_up.tga" ,
			"pic/urbansp_dn.tga" ,
			"pic/urbansp_bk.tga",
			"pic/urbansp_ft.tga"
		};
		const char* faces2[] = {
			"sky/sky_rt.jpg" ,
			"sky/sky_lf.jpg",
			"sky/sky_up.jpg" ,
			"sky/sky_dn.jpg" ,
			"sky/sky_bk.jpg",
			"sky/sky_ft.jpg"
		};
		_mDDSFile.LoadCubeMap(faces2);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		return true;
	}
	return false;
}

void MultiTexture::Uninit()
{
	glDeleteProgram(_pShaderProgramHandle);
	glDeleteProgram(_skyBoxProgramHandle);
	glDeleteBuffers(2, _vboBuffers);
	glDeleteVertexArrays(2, _vaoId);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	
	// 先绘制skyBox
	glDepthMask(GL_FALSE); // 禁止写入深度缓冲区
	glUseProgram(_skyBoxProgramHandle);
	glm::mat4 projection	= glm::perspective(glm::radians(45.0f), static_cast<float>(640) / static_cast<float>(480), 1.0f, 100.0f);
	GLfloat radius			= 2.0f;
	GLfloat xPos			= radius * cos(_mAngle);
	GLfloat zPos			= radius * sin(_mAngle);
	glm::mat4 model			= glm::scale(glm::mat4(1.0f), glm::vec3(30.0f, 30.0f, 30.0f));
	glm::mat4 view			= glm::lookAt(glm::vec3(xPos, 0.1f, zPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));	
	glUniformMatrix4fv(glGetUniformLocation(_skyBoxProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_skyBoxProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(_skyBoxProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	
	glBindVertexArray(_vaoId[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _mDDSFile._textureId); // 注意绑定到CUBE_MAP
	glUniform1i(glGetUniformLocation(_skyBoxProgramHandle, "s_texture"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 绘制立方体
	glUseProgram(_pShaderProgramHandle);
	glDepthMask(GL_TRUE);
	glm::mat4 skyboxModel(1.0f);
	skyboxModel = glm::scale(skyboxModel, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(skyboxModel));

	glBindVertexArray(_vaoId[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _mCbue._textureId);
	glUniform1i(glGetUniformLocation(_pShaderProgramHandle, "s_texture"), 0);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_cameraPos"), xPos, 0.1f, zPos);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glUseProgram(0);
	//glFlush();

	glBindVertexArray(0);
}