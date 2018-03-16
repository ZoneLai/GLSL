#ifndef __BMPLoader_H__
#define __BMPLoader_H__
#include "stdafx.h"
class BMPLoader
{
   public:
      BMPLoader();
      ~BMPLoader();
	  bool				Load(const std::string fileName);			/**< 载入位图并创建纹理 */
	  unsigned int		_textureId;									/**< 生成纹理的ID号 */

private:
	  bool				CLoadBitmap(const std::string filename);	/**< 装载一个bmp文件 */

      int				_mImageWidth;								/**< 图像宽度 */
      int				_mImageHeight;								/**< 图像高度 */
      unsigned char *	_pImage;									/**< 指向图像数据的指针 */
};

#endif //__BMPLoader_H__

