#include "TGALoader.h"

TGALoader::TGALoader()
	: _textureId(-1)
	, _imageWidth(0)
	, _imageHeight(0)
	, _imageData(nullptr)
	, _mType(0)
{
}

TGALoader::~TGALoader()
{
	if (_imageData) {
		delete[] _imageData;
		_imageData = nullptr;
	}
}

bool TGALoader::LoadTGA(const std::string fileName)
{
	FILE *pfile;
	unsigned char tempColor;              /**< 用于交换颜色分量 */
	unsigned char bitCount;               /**< 每象素的bit位数 */
	int colorMode;                        /**< 颜色模式 */
	long tgaSize;                         /**< TGA文件大小 */
	unsigned char unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /**< 未压缩TGA文件头 */
	unsigned char tgaHeader[12];          /**< 文件头 */
	unsigned char header[6];              /**< 文件头前6个字节 */

	/** 检查文件名是否为空 */
	if(fileName.empty())
		return false;
   
	/** 打开文件 */
	pfile = fopen(fileName.c_str(), "rb");
	if(!pfile) 
		return false;

	/** 读取文件头前12个字节 */
	fread(tgaHeader, 1, sizeof(tgaHeader), pfile);

	/** 比较文件是否为未压缩文件 */
	if(memcmp(unCompressHeader, tgaHeader, sizeof(unCompressHeader)) != 0) {
		MessageBox(NULL,"文件类型错误!","错误",MB_OK);
		fclose(pfile);
		return false;
	}

	/** 读取6个字节 */
	fread(header, 1, sizeof(header), pfile);

	/** 计算图像的宽度和高度 */
	_imageWidth = header[1] * 256 + header[0];
	_imageHeight = header[3] * 256 + header[2];

	/** 获取每象素的bit位数 */
	bitCount = header[4];

	/**　计算颜色模式和图像大小 */
	colorMode = bitCount / 8;
	tgaSize = _imageWidth * _imageHeight * colorMode;

	/** 分配内存 */
	_imageData = new unsigned char[sizeof(unsigned char) * tgaSize];

	/** 读取数据 */
	fread(_imageData, sizeof(unsigned char), tgaSize, pfile);

	/** 将BGA格式转化为RGA格式 */
	for(long index = 0; index < tgaSize; index += colorMode) {
		tempColor = _imageData[index];
		_imageData[index] = _imageData[index + 2];
		_imageData[index + 2] = tempColor;
	}

	/** 关闭文件 */
	fclose(pfile);

	/** 设置图象类型 */
	if(colorMode == 3) 
		_mType = GL_RGB;
	else 
		_mType = GL_RGBA;

	return true;
}

/** 载入TGA文件并创建纹理 */
bool TGALoader::Load(const std::string fileName)
{
	if(!LoadTGA(fileName)) {
		MessageBox(NULL,"载入TGA文件失败!","错误",MB_OK);
		exit(0);
	}

	/** 生成纹理对象名称 */
	glGenTextures(1, &_textureId);
   
	/** 创建纹理对象 */
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);
	
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   
	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, _mType, _imageWidth, _imageHeight, _mType, GL_UNSIGNED_BYTE, _imageData);
	return true;
}