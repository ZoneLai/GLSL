#include <sstream>
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

	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle) && CompileShaderProgram(lamp_vert, lamp_frag, &_pLampProgramHandle)) {
		// ָ�������������� ����λ�� ���� ������
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
		// ����λ������
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);
		// ������������
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)(3 * sizeof(GL_FLOAT))
		);
		glEnableVertexAttribArray(1);
		// ���㷨��������
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
		// ������Դ��VAO
		glGenVertexArrays(1, &_vaoLampId);
		glBindVertexArray(_vaoLampId);
		glBindBuffer(GL_ARRAY_BUFFER, _vboBuffers); // ������������� �����ظ����Ͷ�������
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GL_FLOAT),
			(GLvoid*)0
		);
		glEnableVertexAttribArray(0);				// ֻ��Ҫ����λ�ü���
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
	if (_mKeys.IsPressed(VK_ESCAPE) == true)					/**< ��ESC�˳� */
	{
		TerminateApplication();
	}
	_mAngle += 0.0005f;											/**< ������ת�ĽǶ� */
}

glm::vec3 MultiTexture::getEyePosCircle()
{
	GLfloat radius = 5.0f;
	GLfloat xPos = radius * cos(_mAngle);
	GLfloat zPos = radius * sin(_mAngle);
	return glm::vec3(xPos, 0.0f, zPos);
}

void MultiTexture::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();
	glBindVertexArray(_vaoId);
	glUseProgram(_pShaderProgramHandle);
	setupLights(_pointLightPositions, sizeof(_pointLightPositions) / sizeof(_pointLightPositions[0]));
	// ����diffuseMap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture._textureId);
	glUniform1i(glGetUniformLocation(_pShaderProgramHandle, "u_material.diffuseMap"), 0);
	// ����specularMap
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _specularTexture._textureId);
	glUniform1i(glGetUniformLocation(_pShaderProgramHandle, "u_material.specularMap"), 1);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_material.shininess"), 32.0f);
	// ͶӰ����
	glm::mat4 projection	= glm::perspective(glm::radians(45.0f), static_cast<float>(640) / static_cast<float>(480), 1.0f, 100.0f);
	// �ӱ任����
	glm::vec3 eyePos		= getEyePosCircle();
	glm::mat4 view			= glm::lookAt(eyePos/*glm::vec3(0.0f, 2.0f, 3.0f)*/, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	// ���ƶ��������
	for (auto i = 0; i < sizeof(_cubePostitions) / sizeof(_cubePostitions[0]); ++i) {
		// ģ�ͱ任����
		glm::mat4 model(1.0f);
		model			= glm::translate(model, _cubePostitions[i]);
		GLfloat angle	= 20.0f * i;
		model			= glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		model			= glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	// ���ƹ�Դ �����������
	glBindVertexArray(_vaoLampId);
	glUseProgram(_pLampProgramHandle);
	glUniformMatrix4fv(glGetUniformLocation(_pLampProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pLampProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	for (int i = 0; i < sizeof(_pointLightPositions) / sizeof(_pointLightPositions[0]); ++i) {
		glm::mat4 lampModel = glm::mat4(1.0f);
		lampModel			= glm::translate(lampModel, _pointLightPositions[i]);
		lampModel			= glm::scale(lampModel, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(_pLampProgramHandle, "u_modelMatrix"),1, GL_FALSE, glm::value_ptr(lampModel));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
}

void MultiTexture::setupLights(glm::vec3* PointLightPositions, int pointLightCnt)
{
	// ���÷����Դ
	glm::vec3 lampDir(0.5f, 0.8f, 0.0f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_dirLight.ambient"), 0.0f, 0.1f, 0.4f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_dirLight.diffuse"), 0.0f, 0.2f, 0.8f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_dirLight.specular"), 0.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_dirLight.direction"), lampDir.x, lampDir.y, lampDir.z); // �����Դ
	// ���õ��Դ
	for (auto i = 0; i < pointLightCnt; ++i) {
		// ���ù�Դ���� ���Դ
		std::stringstream indexStr;
		indexStr << i;
		std::string lightName	= "u_pointLights[" + indexStr.str() + "]";  // ʹ����������pointLights[i]
		GLint lightAmbientLoc	= glGetUniformLocation(_pShaderProgramHandle, (lightName + ".ambient").c_str());
		GLint lightDiffuseLoc	= glGetUniformLocation(_pShaderProgramHandle, (lightName + ".diffuse").c_str());
		GLint lightSpecularLoc	= glGetUniformLocation(_pShaderProgramHandle, (lightName + ".specular").c_str());
		GLint lightPosLoc		= glGetUniformLocation(_pShaderProgramHandle, (lightName + ".position").c_str());
		GLint attConstant		= glGetUniformLocation(_pShaderProgramHandle, (lightName + ".constant").c_str());
		GLint attLinear			= glGetUniformLocation(_pShaderProgramHandle, (lightName + ".linear").c_str());
		GLint attQuadratic		= glGetUniformLocation(_pShaderProgramHandle, (lightName + ".quadratic").c_str());
		glUniform3f(lightAmbientLoc, 0.0f, 0.1f, 0.4f);
		glUniform3f(lightDiffuseLoc, 0.0f, 0.1f, 0.6f);
		glUniform3f(lightSpecularLoc, 0.0f, 1.0f, 1.0f);
		glm::vec3 lampPos = PointLightPositions[i];
		glUniform3f(lightPosLoc, lampPos.x, lampPos.y, lampPos.z);
		// ����˥��ϵ��
		glUniform1f(attConstant, 1.0f);
		glUniform1f(attLinear, 0.09f);
		glUniform1f(attQuadratic, 0.032f);
	}
	// ����FlashLight��Դ
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.ambient"), 0.0f, 0.1f, 0.4f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.diffuse"), 0.0f, 0.2f, 0.8f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.specular"), 0.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.position"), 0.0f, 0.0f, 2.0f);
	glUniform3f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.direction"), 0.0f, 0.0f, -1.0f);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.cutoff"), cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.outerCutoff"), cos(glm::radians(17.5f)));
	// ����˥��ϵ��
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(_pShaderProgramHandle, "u_spotLight.quadratic"), 0.032f);
}