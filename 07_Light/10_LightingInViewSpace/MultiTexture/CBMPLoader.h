#ifndef __BMPLoader_H__
#define __BMPLoader_H__
#include "stdafx.h"
class BMPLoader
{
   public:
      BMPLoader();
      ~BMPLoader();
	  bool				Load(const std::string fileName);			/**< ����λͼ���������� */
	  unsigned int		_textureId;									/**< ���������ID�� */

private:
	  bool				CLoadBitmap(const std::string filename);	/**< װ��һ��bmp�ļ� */

      int				_mImageWidth;								/**< ͼ���� */
      int				_mImageHeight;								/**< ͼ��߶� */
      unsigned char *	_pImage;									/**< ָ��ͼ�����ݵ�ָ�� */
};

#endif //__BMPLoader_H__

