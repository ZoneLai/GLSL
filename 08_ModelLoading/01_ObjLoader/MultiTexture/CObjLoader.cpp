#include "CObjLoader.h"

bool loadFromFile(const std::string path, std::vector<Vertex>& vertData)
{
	std::vector<VertexCombineIndex> vertComIndices;
	std::vector<glm::vec3>			tempVertices;
	std::vector<glm::vec2>			tempTextCoords;
	std::vector<glm::vec3>			tempNormals;
	std::ifstream file(path);
	if (!file) {
		MessageBox(NULL, "���ܴ�Obj�ļ�", "����", MB_OK);
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "vt") { // ����������������
			std::istringstream s(line.substr(2));
			glm::vec2 v;
			s >> v.x;
			s >> v.y;
			v.y = -v.y;  // ע��������ص�dds���� Ҫ��y���з�ת
			tempTextCoords.push_back(v);
		} else if (line.substr(0, 2) == "vn") {	// ���㷨��������
			std::istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			tempNormals.push_back(v);
		} else if (line.substr(0, 1) == "v") {	// ����λ������
			std::istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			tempVertices.push_back(v);
		} else if (line.substr(0, 1) == "f") {	// ������
			std::istringstream vtns(line.substr(2));
			std::string vtn;
			while (vtns >> vtn) {				// ����һ���ж����������
				VertexCombineIndex vertComIndex;
				std::replace(vtn.begin(), vtn.end(), '/', ' ');
				std::istringstream ivtn(vtn);
				if (vtn.find("  ") != std::string::npos) { // û����������
					MessageBox(NULL, "no texture data found within file", "����", MB_OK);
					return false;
				}
				ivtn >> vertComIndex.posIndex >> vertComIndex.textCoordIndex >> vertComIndex.normIndex;
				vertComIndex.posIndex--;
				vertComIndex.textCoordIndex--;
				vertComIndex.normIndex--;
				vertComIndices.push_back(vertComIndex);
			}
		} else if (line[0] == '#') { // ע�ͺ���
			;
		} else {
			;						// �������� ��ʱ������
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
	// ����λ������
	glVertexAttribPointer(
		0,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)0
	);
	glEnableVertexAttribArray(0);
	// ������������
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex),
		(GLvoid*)(3 * sizeof(GL_FLOAT))
	);
	glEnableVertexAttribArray(1);
	// ���㷨��������
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