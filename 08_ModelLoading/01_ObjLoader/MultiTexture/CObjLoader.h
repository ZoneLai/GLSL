//========================================================
/**
*  @file  CObjLoader.h
*
*  项目描述： OpenGL程序框架演示
*  文件描述:  Obj模型加载类
*  适用平台： Windows7/10
*
*  作者：     LAI ZHONG AN
*  电子邮件:  zhonganlai@gmail.com
*  创建日期： 20017-10-08
*  修改日期： 20017-10-08
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

// 表示一个顶点属性
struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

// 表示obj文件中一个顶点的位置、纹理坐标和法向量 索引
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