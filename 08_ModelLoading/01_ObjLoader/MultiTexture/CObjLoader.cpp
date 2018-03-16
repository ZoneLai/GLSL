#include "CObjLoader.h"

bool loadFromFile(const std::string path, std::vector<Vertex>& vertData)
{
	std::vector<VertexCombineIndex> vertComIndices;
	std::vector<glm::vec3>			tempVertices;
	std::vector<glm::vec2>			tempTextCoords;
	std::vector<glm::vec3>			tempNormals;
	std::ifstream file(path);
	if (!file) {
		MessageBox(NULL, "不能打开Obj文件", "错误", MB_OK);
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "vt") { // 顶点纹理坐标数据
			std::istringstream s(line.substr(2));
			glm::vec2 v;
			s >> v.x;
			s >> v.y;
			v.y = -v.y;  // 注意这里加载的dds纹理 要对y进行反转
			tempTextCoords.push_back(v);
		} else if (line.substr(0, 2) == "vn") {	// 顶点法向量数据
			std::istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			tempNormals.push_back(v);
		} else if (line.substr(0, 1) == "v") {	// 顶点位置数据
			std::istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			tempVertices.push_back(v);
		} else if (line.substr(0, 1) == "f") {	// 面数据
			std::istringstream vtns(line.substr(2));
			std::string vtn;
			while (vtns >> vtn) {				// 处理一行中多个顶点属性
				VertexCombineIndex vertComIndex;
				std::replace(vtn.begin(), vtn.end(), '/', ' ');
				std::istringstream ivtn(vtn);
				if (vtn.find("  ") != std::string::npos) { // 没有纹理数据
					MessageBox(NULL, "no texture data found within file", "错误", MB_OK);
					return false;
				}
				ivtn >> vertComIndex.posIndex >> vertComIndex.textCoordIndex >> vertComIndex.normIndex;
				vertComIndex.posIndex--;
				vertComIndex.textCoordIndex--;
				vertComIndex.normIndex--;
				vertComIndices.push_back(vertComIndex);
			}
		} else if (line[0] == '#') { // 注释忽略
			;
		} else {
			;						// 其余内容 暂时不处理
		}
	}
	for (std::vector<GLuint>::size_type i = 0; i < vertComIndices.size(); ++i) {
		Vertex vert;
		VertexCombineIndex comIndex = vertComIndices[i];
		vert.position	= tempVertices[comIndex.posIndex];
		vert.texCoords	= tempTextCoords[comIndex.textCoordIndex];
		vert.normal		= tempNormals[comIndex.normIndex];
		vertData.push_back(vert);
	}
	return true;
}

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
Mesh::Mesh(const std::vector<Vertex>& vertData, GLint textureId)
	: _vertData(vertData)
	, _vaoId(0)
	, _vboId(0)
	, _textureId(textureId)
{
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &_vaoId);
	glDeleteBuffers(1, &_vboId);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &_vaoId);
	glBindVertexArray(_vaoId);
	glGenBuffers(1, &_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertData.size(), &_vertData[0], GL_STATIC_DRAW);
	// 顶点位置属性
	glVertexAttribPointer(
		0,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)0
	);
	glEnableVertexAttribArray(0);
	// 顶点纹理坐标
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex),
		(GLvoid*)(3 * sizeof(GL_FLOAT))
	);
	glEnableVertexAttribArray(1);
	// 顶点法向量属性
	glVertexAttribPointer(
		2, 
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)(5 * sizeof(GL_FLOAT))
	);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::drawMesh(GLuint shaderProgramHandle)
{
	glBindVertexArray(_vaoId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glUniform1i(glGetUniformLocation(shaderProgramHandle, "s_texture"), 0);
	glDrawArrays(GL_TRIANGLES, 0, _vertData.size());
	glBindVertexArray(0);
	glUseProgram(0);
}