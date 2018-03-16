//========================================================
/**
*  @file      TGALoader.h
*
*  ��Ŀ������ TGA�ļ�������
*  �ļ�����:  ����ӳ��  
*  ����ƽ̨�� Windows7/10
*  
*  ���ߣ�     WWBOSS
*  �����ʼ�:  wwboss123@gmail.com
*  �������ڣ� 2006-12-06	
*  �޸����ڣ� 2006-12-11
*
*/     
//========================================================
#ifndef __TGALOADER_H__
#define __TGALOADER_H__
#include "stdafx.h"
class TGALoader
{
public:
	  
	TGALoader();								/**< ���캯�� */
    ~TGALoader();

	bool Load(const std::string fileName); 		/**< ����TGA�ļ�Ϊ���� */
	unsigned int		_textureId;				/**< ���������ID�� */


private:
	bool LoadTGA(const std::string file);		/**< ����TGA�ļ� */

    int					_imageWidth;			/**< ͼ���� */
	int					_imageHeight;			/**< ͼ��߶� */
    unsigned char*		_imageData;				/**< ָ��ͼ�����ݵ�ָ�� */
    unsigned int		_mType;					/**< ͼ������GL_RGB ��GL_RGBA */
};


#endif