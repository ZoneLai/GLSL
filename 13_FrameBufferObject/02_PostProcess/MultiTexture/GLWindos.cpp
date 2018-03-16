//========================================================
/**
*  @file  GLWindows.h
*
*  ��Ŀ������ OpenGL��������ʾ
*  �ļ�����:  ���򴰿���
*  ����ƽ̨�� Windows7/XP
*
*  ���ߣ�     LAI ZHONG AN
*  �����ʼ�:  zhonganlai@gmail.com
*  �������ڣ� 20017-06-11
*  �޸����ڣ� 20017-06-11
*
*/
//========================================================
#include "GLWindow.h"													/**< ����GLwindows.hͷ�ļ� */
GLWindow::GLWindow()
	: _hWnd(nullptr)
	,_hDC(nullptr)
	, _hRC(nullptr)
	, _mWindowPosX(0)
	, _mWindowPosY(0)
	, _mWindowWidth(640)
	, _mWindowHeight(480)
	, _mScreenWidth(1024)
	, _mScreenHeight(720)
	, _mBitsPerPixel(16)
{
}

/** ����windows���� */
bool GLWindow::Create(const char* windowTitle, const char* className, bool fullScreen, HINSTANCE hInstance, LPVOID lpParam)
{
	_mIsFullScreen = fullScreen;
	int nX = 0;
	int nY = 0;
	PIXELFORMATDESCRIPTOR pfd =											/**< �������������ṹ */
	{
		sizeof(PIXELFORMATDESCRIPTOR),									/**< ���������ṹ�Ĵ�С */
		1,																/**< �汾�� */
		PFD_DRAW_TO_WINDOW |											/**< �������������ʾ��һ�������� */
		PFD_SUPPORT_OPENGL |											/**< ������֧��OpenGL��ͼ */
		PFD_STEREO |													/**< ��ɫ�����������建�� */
		PFD_DOUBLEBUFFER,												/**< ��ɫ��������˫���� */
		PFD_TYPE_RGBA,													/**< ʹ��RGBA��ɫ��ʽ */
		_mBitsPerPixel,													/**< ��ɫ����������ɫֵ��ռ��λ�� */
		0, 0, 0, 0, 0, 0,												/**< ʹ��Ĭ�ϵ���ɫ���� */
		0,																/**< ��Alpha���� */
		0,																/**< ��ɫ��������alpha�ɷֵ���λ���� */
		0,																/**< ���ۼƻ����� */
		0, 0, 0, 0,														/**< �ۼƻ���������λ */
		32,																/**< 32λ��Ȼ��� */
		0,																/**< ���ɰ滺�� */
		0,																/**< �޸��������� */
		PFD_MAIN_PLANE,													/**< ����ΪPFD_MAIN_PLANE������Ϊ����ͼ�� */
		0,																/**< ��ʾOpenGLʵ����֧�ֵ��ϲ���²�ƽ������� */
		0, 0, 0															/**< ��ʱ���Ѳ���ʹ�� */
	};
	// DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX;	/**< �������Ǵ������ͣ�ʹ�ó����趨��ȥ����󻯰�ť�������ܸı䴰���С */
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;

	if (_mIsFullScreen == true)	{										/**< ���Ϊȫ��ģʽ������ת��Ϊȫ��ģʽ */
		if (ChangeScreenSetting() == false)
		{																/**< ȫ��ģʽת��ʧ�ܣ������Ի�����ʾ�������Դ���ģʽ */
			MessageBox(HWND_DESKTOP, "ģʽת��ʧ��.\n�ڴ���ģʽ������.", "Error", MB_OK | MB_ICONEXCLAMATION);
			_mIsFullScreen = false;										/**< ����Ϊ����ģʽ */
		}
		else															/**< ���Ϊ����ģʽ */
		{
			ShowCursor(false);											/**< ������� */
			windowStyle = WS_POPUP;										/**< ���ô���ģʽΪ���㴰�� */
			windowExtendedStyle |= WS_EX_TOPMOST;
		}
	}

	/// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
	RECT windowRect = { GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight() };
	if (_mIsFullScreen == false)										/**< �ڴ���ģʽ��ʹ�� */
	{
		windowExtendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	/**< ʹ���ھ���3D��� */
		int wid = GetSystemMetrics(SM_CXSCREEN);		/**< ��ȡ��ǰ��Ļ�� */
		int hei = GetSystemMetrics(SM_CYSCREEN);		/**< ��ȡ��ǰ��Ļ�� */
		nX = (wid - GetWidth()) / 2;                    /**< ���㴰�ھ����� */
		nY = (hei - GetHeight()) / 2;
		/// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
		/// �жϴ��ڵ����Ͻ��Ƿ�������������
		if (windowRect.left < 0)										/**< �������X����Ϊ�����ƶ����굽0�������������ڵ�λ�� */
		{
			windowRect.right -= windowRect.left;
			windowRect.left = 0;
		}
		if (windowRect.top < 0)											/**< �������Y����Ϊ�����ƶ����굽0�������������ڵ�λ�� */
		{
			windowRect.bottom -= windowRect.top;
			windowRect.top = 0;
		}
	}

	/// ��������
	_hWnd = CreateWindowEx(windowExtendedStyle,		/**< ���ڵ���չ��� */
		className,									/**< ���ڵ����� */
		windowTitle,								/**< ���ڱ��� */
		windowStyle,								/**< ���ڵķ�� */
		nX, nY,                                     /**< ���ڵ����Ͻ�λ�� */
		windowRect.right - windowRect.left,			/**< ���ڵĿ�� */
		windowRect.bottom - windowRect.top,			/**< ���ڵĸ߶� */
		HWND_DESKTOP,								/**< ���ڵĸ�����Ϊ���� */
		0,											/**< �޲˵� */
		hInstance,									/**< ���봰�ڵ�ʵ����� */
		lpParam);									/**< ������������ */

	while (_hWnd != 0)													/**< �����Ƿ񴴽��ɹ� */
	{
		_hDC = GetDC(_hWnd);											/**< ���ش��ڵ��豸������ */
		if (_hDC == 0)													/**< ���Ϊ�� */
		{																/**< ʧ�� */
			break;
		}

		GLuint PixelFormat = ChoosePixelFormat(_hDC, &pfd);				/**< ����һ�����ݵ����ظ�ʽ */
		if (PixelFormat == 0)											/**< ���û�ҵ� */
		{																/**< ʧ�� */
			break;
		}
		if (SetPixelFormat(_hDC, PixelFormat, &pfd) == false)			/**< �������ظ�ʽ */
		{																/**< ʧ�� */
			break;
		}
		_hRC = wglCreateContext(_hDC);									/**< ����OpenGL����Ⱦ������ */
		if (_hRC == 0)													/**< ���Ϊ�� */
		{																/**< ʧ�� */
			break;
		}
		if (wglMakeCurrent(_hDC, _hRC) == false)						/**< ���õ�ǰ��OpenGL����Ⱦ����Ϊ��ǰ�Ĵ��� */
		{																/**< ʧ�� */
			break;
		}

		ShowWindow(_hWnd, SW_NORMAL);									/**< ��ʾ���� */
		ReshapeGL();													/**< ����OpenGL�������ڴ�С */
		return true;													/**< �ɹ����� */
	}

	Destroy();															/**< �ͷ���Դ */
	return false;														/**< ����ʧ�� */
}

/** ɾ��OpenGL���� */
void GLWindow::Destroy()
{
	if (_hWnd != 0)											/**< ���ھ���Ƿ���� */
	{
		if (_hDC != 0)										/**< �����豸�������Ƿ���� */
		{
			wglMakeCurrent(_hDC, 0);						/**< ���õ�ǰ��OpenGL����Ⱦ����Ϊ��NULL */
			if (_hRC != 0)									/**< OpenGL����Ⱦ�������Ƿ���� */
			{
				wglDeleteContext(_hRC);						/**< �ͷ�OpenGL����Ⱦ������ */
				_hRC = 0;									/**< ����OpenGL����Ⱦ������Ϊ0 */
			}
			ReleaseDC(_hWnd, _hDC);							/**< �ͷŴ��ڵ��豸������ */
			_hDC = 0;										/**< ���ô��ڵ��豸������Ϊ0 */
		}
		DestroyWindow(_hWnd);								/**< ɾ������ */
		_hWnd = 0;											/**< ���ô��ھ��Ϊ0 */
	}

	if (_mIsFullScreen)										/**< ���Ϊȫ��ģʽ���ڳ�������󣬱任������ģʽ������ʾ��� */
	{
		ChangeDisplaySettings(NULL, 0);						/**< �任������ģʽ */
		ShowCursor(true);									/**< ��ʾ��� */
	}
}

/** �ı䴰�ڵ���ʾ���� */
bool GLWindow::ChangeScreenSetting()
{
	DEVMODE dmScreenSettings;											/**< �豸ģʽ */
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));						/**< ����ṹ */
	dmScreenSettings.dmSize = sizeof(DEVMODE);							/**< �ṹ��С */
	dmScreenSettings.dmPelsWidth = GetWidth();							/**< ���ÿ�� */
	dmScreenSettings.dmPelsHeight = GetHeight();						/**< ���ø߶� */
	dmScreenSettings.dmBitsPerPel = _mBitsPerPixel;						/**< ����λ��� */
	//dmScreenSettings.dmDisplayFrequency = 75;                         /**< ������Ļˢ���� */
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//| DM_DISPLAYFREQUENCY;

	/// �ı���ʾģʽ
	if (ChangeDisplaySettings(&dmScreenSettings, 0) != DISP_CHANGE_SUCCESSFUL)
	{
		return false;													/**< ���ʧ�ܣ�����false */
	}

	return true;														/**< �ɹ����� */
}

/** �����ڴ�С�ı�ʱ��֪ͨOpenGL������С */
void GLWindow::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);									/**< ���������ӿ� */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/** ���ô������Ͻǵ�λ�� */
void GLWindow::SetPosX(int x)
{
	if (_mIsFullScreen == false)
	{
		_mWindowPosX = x;
	}
}

void GLWindow::SetPosY(int y)
{
	if (_mIsFullScreen == false)
	{
		_mWindowPosY = y;
	}
}

/** ���ش��ڵĴ�С */
int	GLWindow::GetWidth()
{
	if (_mIsFullScreen == true)
	{
		return _mScreenWidth;
	}
	else
	{
		return _mWindowWidth;
	}
}

int	GLWindow::GetHeight()
{
	if (_mIsFullScreen == true)
	{
		return _mScreenHeight;
	}
	else
	{
		return _mWindowHeight;
	}
}

/** ���ô��ڵĴ�С */
void GLWindow::SetWidth(int width)
{
	if (_mIsFullScreen == true)
	{
		_mScreenWidth = width;
	}
	else
	{
		_mWindowWidth = width;
	}
}

void GLWindow::SetHeight(int height)
{
	if (_mIsFullScreen == true)
	{
		_mScreenHeight = height;
	}
	else
	{
		_mWindowHeight = height;
	}
}

/** ���ش������Ͻǵ�λ�� */
int	GLWindow::GetPosX()
{
	if (_mIsFullScreen == false)
	{
		return _mWindowPosX;
	}
	return 0;
}

int	GLWindow::GetPosY()
{
	if (_mIsFullScreen == false)
	{
		return _mWindowPosY;
	}
	return 0;
}