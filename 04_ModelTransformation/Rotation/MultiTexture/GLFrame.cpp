#include "GLFrame.h"

/** ��������� */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	GLApplication * appl = GLApplication::Create("OpenGL");				/**< ���������� */
	if (appl != 0) {
		ret = appl->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);/**< ִ�г�����ѭ�� */
		delete appl;													/**< ɾ�������ࣨ�ڼ̳����У�ʹ��GL_Example * example = new GL_Example(class_name);������һ���ڴ棩*/
	}
	else {															/**< ����������� */
		MessageBox(HWND_DESKTOP, "�����������", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	return ret;
}

/** ���캯�� */
GLApplication::GLApplication(const char * className)
	:_mClassName(className)												/**< �������� */
	, _mIsProgramLooping(true)											/**< ���ó���ѭ��Ϊtrue */
	, _mCreateFullScreen(false)											/**< ʹ��ȫ��ģʽ */
	, _mIsVisible(false)												/**< ���ɼ� */
	, _mResizeDraw(false)												/**< �ڴ��ڸı��С��ʱ�򣬲��ɻ��� */
	, _mLastTickCount(0)
{
}

/**< �л� ȫ��/����ģʽ */
void GLApplication::ToggleFullscreen()
{
	PostMessage(_mWindow, WM_TOGGLEFULLSCREEN, 0, 0);					/**< �����Զ����л���Ϣ */
}

/**< �������� */
void GLApplication::TerminateApplication()
{
	PostMessage(_mWindow, WM_QUIT, 0, 0);								/**< �����˳���Ϣ */
	_mIsProgramLooping = false;											/**< ֹͣ����ѭ�� */
}

/** �������ѭ�� */
int GLApplication::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/// ע��һ������
	WNDCLASSEX windowClass;												/**< ������ */
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						/**< ��սṹΪ0 */
	windowClass.cbSize = sizeof(WNDCLASSEX);							/**< ���ڽṹ�Ĵ�С */
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				/**< ���ô�������Ϊ���ƶ�ʱ�ػ�����Ϊ����ȡ��DC */
	windowClass.lpfnWndProc = (WNDPROC)(WindowProc);					/**< WndProc������Ϣ */
	windowClass.hInstance = hInstance;									/**< ����ʵ�� */
	windowClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);			/**< ���ñ��� */
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);					/**< ������ */
	windowClass.lpszClassName = _mClassName;							/**< �������� */
	if (RegisterClassEx(&windowClass) == 0)								/**< ����ע�ᴰ���� */
	{																	/**< NOTE: Failure, Should Never Happen */
		MessageBox(HWND_DESKTOP, "ע�ᴰ��ʧ��!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														/**< �˳�������FALSE */
	}
	while (_mIsProgramLooping) {										/**< ѭ��ֱ��WM_QUIT�˳����� */
		/// ����һ������
		if (_mWindow.Create("��ת�任", _mClassName, _mCreateFullScreen, hInstance, this) == true)
		{
			/// �����ʼ��ʧ�ܣ����˳�
			if (Init() == false)										/**< �����Զ���ĳ�ʼ������ */
			{															/**< ʧ�� */
				TerminateApplication();									/**< �رմ����˳����� */
			}
			else														/**< �ɹ���ʼ��Ϣѭ�� */
			{
				MSG msg;												/**< Window��Ϣ�ṹ */
				bool isMessagePumpActive = true;						/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
				_mLastTickCount = GetTickCount();						/**< ���ص�ǰ�ļ�ʱ����ֵ */
				_mKeys.Clear();											/**< ������еİ�����Ϣ */
				while (isMessagePumpActive == true)						/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
				{														/**< �ɹ��������ڣ���ⴰ����Ϣ */
					if (PeekMessage(&msg, _mWindow, 0, 0, PM_REMOVE) != 0)
					{
						/// ����Ƿ�ΪWM_QUIT��Ϣ
						if (msg.message != WM_QUIT)
						{
							DispatchMessage(&msg);						/**< ������Ƿ�����Ϣ����Ϣ�ص������д��� */
						}
						else
						{
							isMessagePumpActive = false;				/**< ����ǣ����˳� */
						}
					} else {											/// ���û����Ϣ
						if (_mIsVisible == false)						/**< ������ڲ��ɼ� */
						{
							WaitMessage();								/**< ��ͣ���򣬵ȴ���Ϣ */
						}
						else											/**< ������ڿɼ� */
						{												/**< ִ����Ϣѭ�� */
							DWORD tickCount = GetTickCount();			/**< ���ؼ�ʱ���ĵ�ǰֵ */
							Update(tickCount - _mLastTickCount);		/**< �����û��Զ���ĸ��º��� */
							_mLastTickCount = tickCount;				/**< ����������һ�Σ���������ֵ */
							Draw();										/**< �����û��Զ���Ļ��ƺ��� */
							_mWindow.SwapBuffers();						/**< ����ǰ��֡���� */
						}
					}
				}														/**< ���isMessagePumpActive == true����ѭ�� */
			}
			/**< ������� */
			Uninit();													/**< �û��Զ����ж�غ��� */
			_mWindow.Destroy();											/**< ɾ������ */
		}
		else															/**< �����������ʧ�� */
		{
			MessageBox(HWND_DESKTOP, "����OpenGL���ڴ���", "Error", MB_OK | MB_ICONEXCLAMATION);
			_mIsProgramLooping = false;									/**< ֹͣ����ѭ�� */
		}
	}

	UnregisterClass(_mClassName, hInstance);							/**< ȡ��ע��Ĵ��� */
	return 0;
}

/** ��������Ϣ */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// LONG user_data = GetWindowLong(hWnd, GWL_USERDATA);					/**< �����û��Զ����32λ�ĳ��򸽼�ֵ */
	LONG_PTR user_data = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (user_data == 0)
	{
		/// ��������һ������
		if (uMsg == WM_CREATE)											/**< �����ڴ�����Ϣ */
		{
			/// ���ش��ڽṹ��ָ�룬������մ����ĳ���ʵ������
			CREATESTRUCT * creation = reinterpret_cast<CREATESTRUCT *>(lParam);
			/// ��ó���ʵ����ָ��
			GLApplication * appl = reinterpret_cast<GLApplication *>(creation->lpCreateParams);
			/// �������ʵ����ָ��Ϊ�û��Զ���ĳ��򸽼�ֵ
			// SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(appl));
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(appl));
			appl->_mIsVisible = true;									/**< ���ó���ɼ� */
			return 0;													/**< ���� */
		}
	} else {
		/// ������ǵ�һ�δ��ڣ����س���ʵ����ָ��
		GLApplication * appl = reinterpret_cast<GLApplication *>(user_data);
		return appl->Message(hWnd, uMsg, wParam, lParam);				/**< ���ó���ʵ���Լ�����Ϣ������ */
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< ����Ĭ�ϵĴ�����Ϣ������ */
}

/** ��Ϣѭ�� */
LRESULT GLApplication::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)												/**< ����ͬ��Windows��Ϣ */
	{
	case WM_SYSCOMMAND:											/**< �ػ�ϵͳ���� */
		switch (wParam)
		{
		case SC_SCREENSAVE:										/**< �ػ���Ļ������������ */
		case SC_MONITORPOWER:									/**< �ػ���ʾ��ʡ��ģʽ�������� */
			return 0;											/**< ���������������� */
			break;
		}
		break;													/**< �˳� */

	case WM_CLOSE:												/**< �رմ��� */
		TerminateApplication();									/**< ����TerminateApplication���� */
		return 0;
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		_mLastTickCount = GetTickCount();							/**< ���¼�������ֵ */
		return 0;
		break;

	case WM_MOVE:
		_mWindow.SetPosX(LOWORD(lParam));							/**< ������������ */
		_mWindow.SetPosY(HIWORD(lParam));
		return 0;
		break;

	case WM_PAINT:
		if (_mResizeDraw == true)									/**< �����Ҫ�ػ� */
		{
			_mWindow.ReshapeGL();									/**< �������ô��ڵĴ�С */
			Draw();													/**< ���»��� */
			_mWindow.SwapBuffers();									/**< ����ǰ��֡���� */
		}
		break;

	case WM_SIZING:													/**< �������ڸı��С */
	{
		RECT * rect = (RECT *)lParam;
		_mWindow.SetWidth(rect->right - rect->left);				/**< ���ô��ڿ�� */
		_mWindow.SetHeight(rect->bottom - rect->top);				/**< ���ô��ڸ߶� */
		return TRUE;
	}
		break;

	case WM_SIZE:													/**< ���ڸı��С�� */
		switch (wParam)												/**< ����ͬ�Ĵ���״̬ */
		{
		case SIZE_MINIMIZED:									/**< �Ƿ���С��? */
			_mIsVisible = false;								/**< ����ǣ������ò��ɼ� */
			return 0;
			break;

		case SIZE_MAXIMIZED:									/**< �����Ƿ����? */
		case SIZE_RESTORED:										/**< ���ڱ���ԭ? */
			_mIsVisible = true;									/**< ����Ϊ�ɼ� */
			_mWindow.SetWidth(LOWORD(lParam));					/**< ���ô��ڿ�� */
			_mWindow.SetHeight(HIWORD(lParam));					/**< ���ô��ڸ߶� */
			_mWindow.ReshapeGL();								/**< �ı䴰�ڴ�С */
			_mLastTickCount = GetTickCount();					/**< ���¼�������ֵ */
			return 0;
			break;
		}
		break;

	case WM_KEYDOWN:												/**< ���°�����Ϣ */
		_mKeys.SetPressed(wParam);
		return 0;
		break;

	case WM_KEYUP:													/**< �����ͷż���Ϣ */
		_mKeys.SetReleased(wParam);
		return 0;
		break;

	case WM_TOGGLEFULLSCREEN:										/**< �л� ȫ��/����ģʽ */
		/*_mWindow.SetFullScreen(!_mWindow.GetFullScreen());
		if(!_mWindow.GetFullScreen())
			ShowCursor(true);
		else
			ShowCursor(false);*/

		_mCreateFullScreen = !_mCreateFullScreen;
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< ����Ĭ�ϵĴ�����Ϣ������ */
}