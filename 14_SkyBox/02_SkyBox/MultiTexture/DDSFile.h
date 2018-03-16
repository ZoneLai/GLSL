//========================================================
/**
*  @file  DDSFile.h
*
*  ��Ŀ������ OpenGL��������ʾ
*  �ļ�����:  ���������
*  ����ƽ̨�� Windows7/10
*
*  ���ߣ�     LAI ZHONG AN
*  �����ʼ�:  zhonganlai@gmail.com
*  �������ڣ� 20017-10-08
*  �޸����ڣ� 20017-10-08
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
	bool				Load(const std::string fileName);						/**< ������ͨͼƬ�������� */
	bool				LoadDDS(const std::string fileName);					/**< ����DDS���������� */
	bool				LoadCubeMap(const char* filePath[]);			/**< ������պ������������� */
	unsigned int		_textureId;												/**< ���������ID�� */

private:
	int					_imageWidth;								/**< ͼ���� */
	int					_imageHeight;								/**< ͼ��߶� */
	unsigned char*		_imageData;									/**< ָ��ͼ�����ݵ�ָ�� */
};