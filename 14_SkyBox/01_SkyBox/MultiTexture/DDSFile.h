//========================================================
/**
*  @file  DDSFile.h
*
*  项目描述： OpenGL程序框架演示
*  文件描述:  纹理加载类
*  适用平台： Windows7/10
*
*  作者：     LAI ZHONG AN
*  电子邮件:  zhonganlai@gmail.com
*  创建日期： 20017-10-08
*  修改日期： 20017-10-08
*
*/
//========================================================
#pragma once
#include <iostream>
#include <fstream>
#include "SOIL/SOIL.h"
#pragma comment(lib, "SOIL.lib")
class DDSFile
{
public:
	DDSFile();
	~DDSFile();
	bool				Load(const std::string fileName);						/**< 载入普通图片创建纹理 */
	bool				LoadDDS(const std::string fileName);					/**< 载入DDS并创建纹理 */
	bool				LoadCubeMap(const char* filePath[]);			/**< 载入天空合作并创建纹理 */
	unsigned int		_textureId;												/**< 生成纹理的ID号 */

private:
	int					_imageWidth;								/**< 图像宽度 */
	int					_imageHeight;								/**< 图像高度 */
	unsigned char*		_imageData;									/**< 指向图像数据的指针 */
};