#include"CBMPLoader.h"
#define BITMAP_ID 0x4D42	/**< 位图文件的标志 */
BMPLoader::BMPLoader()
	: _textureId(0)
	, _mImageWidth(0)
	, _mImageHeight(0)
	, _pImage(nullptr)
{
}

BMPLoader::~BMPLoader()
{
	if (_pImage) {
		delete[] _pImage;
		_pImage = nullptr;
	}
	glDeleteTextures(1, &_textureId);
}

/** 装载一个位图文件 */
bool BMPLoader::LoadBitmap(const std::string file)
{
	FILE *pFile = nullptr; /**< 文件指针 */
	/** 创建位图文件信息和位图文件头结构 */
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
	unsigned char textureColors = 0;	/**< 用于将图像颜色从BGR变换到RGB */
   /** 打开文件,并检查错误 */
	pFile = fopen(file.c_str(), "rb");
	if(pFile == 0) 
		return false;
	/** 读入位图文件头信息 */ 
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	/** 检查该文件是否为位图文件 */
	if(header.bfType != BITMAP_ID) {
		fclose(pFile);             /**< 若不是位图文件,则关闭文件并返回 */
		return false;
	}
	/** 读入位图文件信息 */
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
	/** 保存图像的宽度和高度 */
	_mImageWidth	= bitmapInfoHeader.biWidth;
	_mImageHeight	= bitmapInfoHeader.biHeight;
    /** 确保读取数据的大小 */
   if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;
	/** 将指针移到数据开始位置 */
	fseek(pFile, header.bfOffBits, SEEK_SET);
	/** 分配内存 */
	_pImage = new unsigned char[bitmapInfoHeader.biSizeImage];
	/** 检查内存分配是否成功 */
	if (!_pImage) {                       /**< 若分配内存失败则返回 */
		delete[] _pImage;
		fclose(pFile);
		return false;
	}
	/** 读取图像数据 */
	fread(_pImage, 1, bitmapInfoHeader.biSizeImage, pFile);
	/** 将图像颜色数据格式进行交换,由BGR转换为RGB */
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index+=3) {
		textureColors		= _pImage[index];
		_pImage[index]		= _pImage[index + 2];
		_pImage[index + 2]	= textureColors;
	}
	fclose(pFile);       /**< 关闭文件 */
	return true;         /**< 成功返回 */
}

/** 载入位图文件，并创建纹理 */
bool BMPLoader::Load(const std::string fileName)
{
	if(!LoadBitmap(fileName)) {
		MessageBox(NULL,"载入位图文件失败!","错误",MB_OK);
		exit(0);
	}
	/** 生成纹理对象名称 */
	glGenTextures(1, &_textureId);
    /** 创建纹理对象 */
	glBindTexture(GL_TEXTURE_2D, _textureId);
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, _mImageWidth,
					  _mImageHeight, GL_RGB, GL_UNSIGNED_BYTE,
					  _pImage);
   return true;
}