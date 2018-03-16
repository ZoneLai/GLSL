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
	unsigned char tempColor;              /**< ���ڽ�����ɫ���� */
	unsigned char bitCount;               /**< ÿ���ص�bitλ�� */
	int colorMode;                        /**< ��ɫģʽ */
	long tgaSize;                         /**< TGA�ļ���С */
	unsigned char unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /**< δѹ��TGA�ļ�ͷ */
	unsigned char tgaHeader[12];          /**< �ļ�ͷ */
	unsigned char header[6];              /**< �ļ�ͷǰ6���ֽ� */

	/** ����ļ����Ƿ�Ϊ�� */
	if(fileName.empty())
		return false;
   
	/** ���ļ� */
	pfile = fopen(fileName.c_str(), "rb");
	if(!pfile) 
		return false;

	/** ��ȡ�ļ�ͷǰ12���ֽ� */
	fread(tgaHeader, 1, sizeof(tgaHeader), pfile);

	/** �Ƚ��ļ��Ƿ�Ϊδѹ���ļ� */
	if(memcmp(unCompressHeader, tgaHeader, sizeof(unCompressHeader)) != 0) {
		MessageBox(NULL,"�ļ����ʹ���!","����",MB_OK);
		fclose(pfile);
		return false;
	}

	/** ��ȡ6���ֽ� */
	fread(header, 1, sizeof(header), pfile);

	/** ����ͼ��Ŀ�Ⱥ͸߶� */
	_imageWidth = header[1] * 256 + header[0];
	_imageHeight = header[3] * 256 + header[2];

	/** ��ȡÿ���ص�bitλ�� */
	bitCount = header[4];

	/**��������ɫģʽ��ͼ���С */
	colorMode = bitCount / 8;
	tgaSize = _imageWidth * _imageHeight * colorMode;

	/** �����ڴ� */
	_imageData = new unsigned char[sizeof(unsigned char) * tgaSize];

	/** ��ȡ���� */
	fread(_imageData, sizeof(unsigned char), tgaSize, pfile);

	/** ��BGA��ʽת��ΪRGA��ʽ */
	for(long index = 0; index < tgaSize; index += colorMode) {
		tempColor = _imageData[index];
		_imageData[index] = _imageData[index + 2];
		_imageData[index + 2] = tempColor;
	}

	/** �ر��ļ� */
	fclose(pfile);

	/** ����ͼ������ */
	if(colorMode == 3) 
		_mType = GL_RGB;
	else 
		_mType = GL_RGBA;

	return true;
}

/** ����TGA�ļ����������� */
bool TGALoader::Load(const std::string fileName)
{
	if(!LoadTGA(fileName)) {
		MessageBox(NULL,"����TGA�ļ�ʧ��!","����",MB_OK);
		exit(0);
	}

	/** ��������������� */
	glGenTextures(1, &_textureId);
   
	/** ����������� */
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);
	
	/** �����˲� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   
	/** �������� */
	gluBuild2DMipmaps(GL_TEXTURE_2D, _mType, _imageWidth, _imageHeight, _mType, GL_UNSIGNED_BYTE, _imageData);
	return true;
}