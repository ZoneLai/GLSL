//========================================================
/**
*  @file  CObjLoader.h
*
*  ��Ŀ������ OpenGL��������ʾ
*  �ļ�����:  Objģ�ͼ�����
*  ����ƽ̨�� Windows7/10
*
*  ���ߣ�     LAI ZHONG AN
*  �����ʼ�:  zhonganlai@gmail.com
*  �������ڣ� 20017-10-08
*  �޸����ڣ� 20017-10-08
*
*/
//========================================================
#pragma once
#include <string>       
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "stdafx.h"

// ��ʾһ����������
struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

// ��ʾobj�ļ���һ�������λ�á���������ͷ����� ����
struct VertexCombineIndex {
	GLuint posIndex;
	GLuint textCoordIndex;
	GLuint normIndex;
};

#ifdef __cplusplus
extern "C" {
#endif

extern bool loadFromFile(const std::string path, std::vector<Vertex>& vertData);

#ifdef __cplusplus
}

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertData, GLint textureId);
	~Mesh();
	void					setupMesh();
	void					drawMesh(GLuint	shaderProgramHandle);

private:
	std::vector<Vertex>		_vertData;
	GLuint					_vaoId;
	GLuint					_vboId;
	GLint					_textureId;
};

#endif