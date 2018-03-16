#include "MultiTexture.h"
#include "GLUtil.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader/sphere_position.frag"
#include "shader/sphere_position.vert"

MultiTexture::MultiTexture(const char * class_name) 
	: GLApplication(class_name)
	, _mAngle(0.0f)
	, _pShaderProgramHandle(0)
	, _pAxisProgramHandle(0)
	, _vaoId(0)
	, _vboBuffers(0)
	, _mesh(nullptr)
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
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	ResizeDraw(true);
	GLenum err = glewInit();
	if (GLEW_OK != err)
		return false;
	//Section1 从obj文件加载数据
	std::vector<Vertex> vertData;
	if (loadFromFile("cube.obj", vertData) == false){
		MessageBox(NULL, "loadFromFile 失败", "错误", MB_OK);
		return false;
	}
	// Section2 准备纹理
	if (_mDDSFile.LoadDDS("cube.DDS") == false) {
		MessageBox(NULL, "_mDDSFile.LoadDDS 失败", "错误", MB_OK);
		return false;
	}
	if (CompileShaderProgram(sphere_vert, sphere_frag, &_pShaderProgramHandle)) {
		// Section3 建立Mesh对象
		_mesh = new Mesh(vertData, _mDDSFile._textureId);
		_mesh->setupMesh();
		return true;
	}
	return false;
}

void MultiTexture::Uninit()
{
	if (_mesh) {
		delete _mesh;
		_mesh = nullptr;
	}
	glDeleteProgram(_pShaderProgramHandle);
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
	glUseProgram(_pShaderProgramHandle);
	glm::mat4 projection	= glm::perspective(glm::radians(45.0f), static_cast<float>(640) / static_cast<float>(480), 1.0f, 100.0f);
	GLfloat radius			= 4.0f;
	GLfloat xPos			= radius * cos(_mAngle);
	GLfloat zPos			= radius * sin(_mAngle);
	// glm::vec3 eyePos(xPos, 2.0f, zPos);
	glm::mat4 view = glm::lookAt(glm::vec3(xPos, 2.0f, zPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));	
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(_pShaderProgramHandle, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	_mesh->drawMesh(_pShaderProgramHandle);
	glFlush();
}