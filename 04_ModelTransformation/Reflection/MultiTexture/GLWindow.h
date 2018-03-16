#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__
#include "stdafx.h"

/** windows������ */
class GLWindow
{
public:
	/** ��Ҫ�Ĵ��ں���  */
	/** ���캯�� */
	GLWindow();
	/** ����windows���� */
	bool		Create(const char* windowTitle, const char* className, bool fullScreen, HINSTANCE hInstance, LPVOID lpParam);
	/** ɾ��OpenGL���� */
	void		Destroy();
	/** �ı䴰�ڵ���ʾ���� */
	bool		ChangeScreenSetting();
	/** �����ڴ�С�ı�ʱ��֪ͨOpenGL������С */
	void		ReshapeGL();
	/** Swap Buffers (Double Buffering) */
	void		SwapBuffers() { ::SwapBuffers(_hDC); }

	/** ���溯��Ϊ����/���ش���������� */
	/** ���ô������Ͻǵ�λ�� */
	void		SetPosX(int x);
	void		SetPosX(unsigned short x) { SetPosX((signed short)x); }
	void		SetPosY(int y);
	void		SetPosY(unsigned short y) { SetPosY((signed short)y); }
	/** ���ش��ڵĴ�С */
	int			GetWidth();
	int			GetHeight();
	/** ���ô��ڵĴ�С */
	void		SetWidth(int width);
	void		SetHeight(int height);
	/** ���ش������Ͻǵ�λ�� */
	int			GetPosX();
	int			GetPosY();
	/** ���ô��ڵ���ɫλ�� */
	void		SetHiColor() { _mBitsPerPixel = 16; }
	void		SetTrueClolr() { _mBitsPerPixel = 32; }
	/** �����������������GL_Window m_Window�������m_Window��Ϊ���ھ��ʹ�� */
	operator HWND() { return _hWnd; }

private:
	HWND		_hWnd;			/** �������ǵĴ��ھ�� */
	HDC			_hDC;			/** ���ڵ��豸������� */
	HGLRC		_hRC ;			/** OpenGL ��Ⱦ�豸��� */

	int			_mWindowPosX;	/** ���������Ͻǵ�Xλ�� */
	int			_mWindowPosY;	/** ���������Ͻǵ�Yλ�� */
	int			_mWindowWidth;	/** ���ڵĿ�� */
	int			_mWindowHeight;	/** ���ڵĸ߶� */
	int			_mScreenWidth;	/** ȫ���Ŀ�� */
	int			_mScreenHeight;	/** ȫ���ĸ߶� */
	int			_mBitsPerPixel;	/** ��ɫλ�� */
	bool		_mIsFullScreen;	/** �Ƿ�ȫ�� */
};

#endif //__GL_WINDOW_H__