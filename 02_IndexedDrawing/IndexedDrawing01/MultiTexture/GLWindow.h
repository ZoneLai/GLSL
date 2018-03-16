#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__
#include "stdafx.h"

/** windows窗口类 */
class GLWindow
{
public:
	/** 重要的窗口函数  */
	/** 构造函数 */
	GLWindow();
	/** 创建windows窗口 */
	bool		Create(const char* windowTitle, const char* className, bool fullScreen, HINSTANCE hInstance, LPVOID lpParam);
	/** 删除OpenGL窗口 */
	void		Destroy();
	/** 改变窗口的显示设置 */
	bool		ChangeScreenSetting();
	/** 当窗口大小改变时，通知OpenGL调整大小 */
	void		ReshapeGL();
	/** Swap Buffers (Double Buffering) */
	void		SwapBuffers() { ::SwapBuffers(_hDC); }

	/** 下面函数为设置/返回窗口类的属性 */
	/** 设置窗口左上角的位置 */
	void		SetPosX(int x);
	void		SetPosX(unsigned short x) { SetPosX((signed short)x); }
	void		SetPosY(int y);
	void		SetPosY(unsigned short y) { SetPosY((signed short)y); }
	/** 返回窗口的大小 */
	int			GetWidth();
	int			GetHeight();
	/** 设置窗口的大小 */
	void		SetWidth(int width);
	void		SetHeight(int height);
	/** 返回窗口左上角的位置 */
	int			GetPosX();
	int			GetPosY();
	/** 设置窗口的颜色位深 */
	void		SetHiColor() { _mBitsPerPixel = 16; }
	void		SetTrueClolr() { _mBitsPerPixel = 32; }
	/** 重载运算符，可以让GL_Window m_Window声明后的m_Window作为窗口句柄使用 */
	operator HWND() { return _hWnd; }

private:
	HWND		_hWnd;			/** 保存我们的窗口句柄 */
	HDC			_hDC;			/** 窗口的设备环境句柄 */
	HGLRC		_hRC ;			/** OpenGL 渲染设备句柄 */

	int			_mWindowPosX;	/** 窗口在左上角的X位置 */
	int			_mWindowPosY;	/** 窗口在左上角的Y位置 */
	int			_mWindowWidth;	/** 窗口的宽度 */
	int			_mWindowHeight;	/** 窗口的高度 */
	int			_mScreenWidth;	/** 全屏的宽度 */
	int			_mScreenHeight;	/** 全屏的高度 */
	int			_mBitsPerPixel;	/** 颜色位深 */
	bool		_mIsFullScreen;	/** 是否全屏 */
};

#endif //__GL_WINDOW_H__