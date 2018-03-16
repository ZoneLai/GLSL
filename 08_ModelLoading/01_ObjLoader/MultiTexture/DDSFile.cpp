#include "DDSFile.h"
#include "stdafx.h"
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
DDSFile::DDSFile()
	: _textureId(0)
	, _imageWidth(0)
	, _imageHeight(0)
	, _imageData(nullptr)
{
}

DDSFile::~DDSFile()
{
	if (_imageData) {
		SOIL_free_image_data(_imageData);
		_imageData = nullptr;
	}
	glDeleteTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool DDSFile::Load(const std::string fileName)
{
	/** 生成纹理对象名称 */
	glGenTextures(1, &_textureId);
	/** 创建纹理对象 */
	glBindTexture(GL_TEXTURE_2D, _textureId);
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	_imageData = SOIL_load_image(fileName.c_str(), &_imageWidth, &_imageHeight, 0, SOIL_LOAD_RGB);
	if (_imageData == nullptr) {
		MessageBox(NULL, "载入文件失败!", "错误", MB_OK);
		return false;
	}
	/** 创建纹理 */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _imageWidth, _imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _imageData);
	return true;
}

bool DDSFile::LoadDDS(const std::string fileName)
{
	/* try to open the file */
	std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);
	if (!file) {
		MessageBox(NULL, "不能打开文件", "错误", MB_OK);
		return false;
	}
	/* verify the type of file */
	char filecode[4];
	file.read(filecode, 4);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		MessageBox(NULL, "DDS文件格式错误", "错误", MB_OK);
		file.close();
		return false;
	}
	/* get the surface desc */
	char header[124];
	file.read(header, 124);
	unsigned int height			= *(unsigned int*)&(header[8]);
	unsigned int width			= *(unsigned int*)&(header[12]);
	unsigned int linearSize		= *(unsigned int*)&(header[16]);
	unsigned int mipMapCount	= *(unsigned int*)&(header[24]);
	unsigned int fourCC			= *(unsigned int*)&(header[80]);
	/* how big is it going to be including all mipmaps? */
	unsigned int bufsize		= mipMapCount > 1 ? linearSize * 2 : linearSize;
	char* buffer				= new char[bufsize];
	file.read(buffer, bufsize);
	/* close the file pointer */
	file.close();
	unsigned int components		= (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC) {
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		delete[] buffer;
		return 0;
	}
	/** 生成纹理对象名称 */
	glGenTextures(1, &_textureId);
	/** 创建纹理对象 */
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	unsigned int blockSize	= (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset		= 0;
	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);
		offset	+= size;
		width	/= 2;
		height	/= 2;
		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1)
			width	= 1;
		if (height < 1)
			height	= 1;
	}
	delete[] buffer;
	return true;
}