#include"CBMPLoader.h"
#define BITMAP_ID 0x4D42	/**< λͼ�ļ��ı�־ */
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

/** װ��һ��λͼ�ļ� */
bool BMPLoader::LoadBitmap(const std::string file)
{
	FILE *pFile = nullptr; /**< �ļ�ָ�� */
	/** ����λͼ�ļ���Ϣ��λͼ�ļ�ͷ�ṹ */
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
	unsigned char textureColors = 0;	/**< ���ڽ�ͼ����ɫ��BGR�任��RGB */
   /** ���ļ�,�������� */
	pFile = fopen(file.c_str(), "rb");
	if(pFile == 0) 
		return false;
	/** ����λͼ�ļ�ͷ��Ϣ */ 
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	/** �����ļ��Ƿ�Ϊλͼ�ļ� */
	if(header.bfType != BITMAP_ID) {
		fclose(pFile);             /**< ������λͼ�ļ�,��ر��ļ������� */
		return false;
	}
	/** ����λͼ�ļ���Ϣ */
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
	/** ����ͼ��Ŀ�Ⱥ͸߶� */
	_mImageWidth	= bitmapInfoHeader.biWidth;
	_mImageHeight	= bitmapInfoHeader.biHeight;
    /** ȷ����ȡ���ݵĴ�С */
   if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;
	/** ��ָ���Ƶ����ݿ�ʼλ�� */
	fseek(pFile, header.bfOffBits, SEEK_SET);
	/** �����ڴ� */
	_pImage = new unsigned char[bitmapInfoHeader.biSizeImage];
	/** ����ڴ�����Ƿ�ɹ� */
	if (!_pImage) {                       /**< �������ڴ�ʧ���򷵻� */
		delete[] _pImage;
		fclose(pFile);
		return false;
	}
	/** ��ȡͼ������ */
	fread(_pImage, 1, bitmapInfoHeader.biSizeImage, pFile);
	/** ��ͼ����ɫ���ݸ�ʽ���н���,��BGRת��ΪRGB */
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index+=3) {
		textureColors		= _pImage[index];
		_pImage[index]		= _pImage[index + 2];
		_pImage[index + 2]	= textureColors;
	}
	fclose(pFile);       /**< �ر��ļ� */
	return true;         /**< �ɹ����� */
}

/** ����λͼ�ļ������������� */
bool BMPLoader::Load(const std::string fileName)
{
	if(!LoadBitmap(fileName)) {
		MessageBox(NULL,"����λͼ�ļ�ʧ��!","����",MB_OK);
		exit(0);
	}
	/** ��������������� */
	glGenTextures(1, &_textureId);
    /** ����������� */
	glBindTexture(GL_TEXTURE_2D, _textureId);
	/** �����˲� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/** �������� */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, _mImageWidth,
					  _mImageHeight, GL_RGB, GL_UNSIGNED_BYTE,
					  _pImage);
   return true;
}