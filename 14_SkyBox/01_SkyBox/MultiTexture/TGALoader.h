//========================================================
/**
*  @file      TGALoader.h
*
*  项目描述： TGA文件载入类
*  文件描述:  纹理映射  
*  适用平台： Windows7/10
*  
*  作者：     WWBOSS
*  电子邮件:  wwboss123@gmail.com
*  创建日期： 2006-12-06	
*  修改日期： 2006-12-11
*
*/     
//========================================================
#ifndef __TGALOADER_H__
#define __TGALOADER_H__
#include "stdafx.h"
class TGALoader
{
public:
	  
	TGALoader();								/**< 构造函数 */
    ~TGALoader();

	bool Load(const std::string fileName); 		/**< 载入TGA文件为纹理 */
	unsigned int		_textureId;				/**< 生成纹理的ID号 */


private:
	bool LoadTGA(const std::string file);		/**< 载入TGA文件 */

    int					_imageWidth;			/**< 图像宽度 */
	int					_imageHeight;			/**< 图像高度 */
    unsigned char*		_imageData;				/**< 指向图像数据的指针 */
    unsigned int		_mType;					/**< 图象类型GL_RGB 或GL_RGBA */
};


#endif