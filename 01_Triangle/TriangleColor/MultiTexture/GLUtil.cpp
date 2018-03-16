#include "GLUtil.h"

void CheckOpenGL(char* location)
{
	GLenum test = glGetError();
	if (test != GL_NO_ERROR){
		switch (test){
			case GL_INVALID_ENUM:
				OutputDebugString("gl error @ %s : GL_INVALID_ENUM\n");
				break;
			case GL_INVALID_VALUE:
				OutputDebugString("gl error @ %s : GL_INVALID_VALUE\n");
				break;
			case GL_INVALID_OPERATION:
				OutputDebugString("gl error @ %s : GL_INVALID_OPERATION\n");
				break;
			case GL_OUT_OF_MEMORY:
				OutputDebugString("gl error @ %s : GL_OUT_OF_MEMORY\n");
				break;
			default:{
				char buffer[1024];
				sprintf(buffer, "gl error @ %s: %X\n", location, test);
				OutputDebugString(buffer);
			}
		}
	}
}

void CheckFrameBufferStatus()
{
	GLenum nStatus;
	nStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (nStatus){
		case GL_FRAMEBUFFER_COMPLETE:
			return;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			OutputDebugString("Unsupported framebuffer format found.");
		default:
			OutputDebugString("other framebuffer error.");
	}
}

void CheckGlError(const char* op)
{
	for (GLint error = glGetError(); error; error = glGetError())
	{
		char buffer[1024];
		sprintf(buffer, "gl error @ %s", op);
		OutputDebugString(buffer);
	}
}

void CheckLocation(int location, char* label)
{
	if (location < 0) {
		OutputDebugString(label);
	}
}

//--------------------------------------------------------------------------------------
// Name: CompileShaderFromString()
// Desc:
//--------------------------------------------------------------------------------------
bool CompileShaderFromString(const char* strShaderSource, GLuint hShaderHandle)
{
	glShaderSource(hShaderHandle, 1, &strShaderSource, nullptr);
	glCompileShader(hShaderHandle);

	// Check for compile success
	GLint nCompileResult = 0;
	glGetShaderiv(hShaderHandle, GL_COMPILE_STATUS, &nCompileResult);
	if (GL_FALSE == nCompileResult) {
		char strInfoLog[1024];
		GLint nLength;
		glGetShaderInfoLog(hShaderHandle, 1024, &nLength, strInfoLog);
		OutputDebugString(strInfoLog);
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------
// Name: LinkShaderProgram()
// Desc: Helper function to link a shader program
//--------------------------------------------------------------------------------------
bool LinkShaderProgram(unsigned int hShaderProgram)
{
	// Link the whole program together
	glLinkProgram(hShaderProgram);

	// Check for link success
	GLint LinkStatus;
	glGetProgramiv(hShaderProgram, GL_LINK_STATUS, &LinkStatus);
	if (false == LinkStatus){
		char  strInfoLog[1024];
		int nLength;
		glGetProgramInfoLog(hShaderProgram, 1024, &nLength, strInfoLog);
		OutputDebugString(strInfoLog);
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------
// Name: CompileShaderProgram()
// Desc:
//--------------------------------------------------------------------------------------
bool CompileShaderProgram(const char* strVertexShader, const char* strFragmentShader, GLuint* pShaderProgramHandle)
{
	// Create the object handles
	GLuint hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Compile the shaders
	if (!CompileShaderFromString(strVertexShader, hVertexShader)) {
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return false;
	}
	if (!CompileShaderFromString(strFragmentShader, hFragmentShader)) {
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return false;
	}

	// Attach the individual shaders to the common shader program
	GLuint hShaderProgram = glCreateProgram();
	glAttachShader(hShaderProgram, hVertexShader);
	glAttachShader(hShaderProgram, hFragmentShader);

	// Link the vertex shader and fragment shader together
	if (false == LinkShaderProgram(hShaderProgram)) {
		glDeleteProgram(hShaderProgram);
		return false;
	}

	// Return the shader program
	(*pShaderProgramHandle) = hShaderProgram;
	return true;
}