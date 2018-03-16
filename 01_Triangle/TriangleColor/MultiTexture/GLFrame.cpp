#include "GLFrame.h"

/** 主程序入口 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	GLApplication * appl = GLApplication::Create("OpenGL");				/**< 创建程序类 */
	if (appl != 0) {
		ret = appl->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);/**< 执行程序主循环 */
		delete appl;													/**< 删除程序类（在继承类中，使用GL_Example * example = new GL_Example(class_name);分配了一块内存）*/
	}
	else {															/**< 创建程序出错 */
		MessageBox(HWND_DESKTOP, "创建程序出错", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	return ret;
}

/** 构造函数 */
GLApplication::GLApplication(const char * className)
	:_mClassName(className)												/**< 保存类名 */
	, _mIsProgramLooping(true)											/**< 设置程序循环为true */
	, _mCreateFullScreen(false)											/**< 使用全屏模式 */
	, _mIsVisible(false)												/**< 不可见 */
	, _mResizeDraw(false)												/**< 在窗口改变大小的时候，不可绘制 */
	, _mLastTickCount(0)
{
}

/**< 切换 全屏/窗口模式 */
void GLApplication::ToggleFullscreen()
{
	PostMessage(_mWindow, WM_TOGGLEFULLSCREEN, 0, 0);					/**< 发送自定的切换消息 */
}

/**< 结束程序 */
void GLApplication::TerminateApplication()
{
	PostMessage(_mWindow, WM_QUIT, 0, 0);								/**< 发送退出消息 */
	_mIsProgramLooping = false;											/**< 停止程序循环 */
}

/** 程序的主循环 */
int GLApplication::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/// 注册一个窗口
	WNDCLASSEX windowClass;												/**< 窗口类 */
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						/**< 清空结构为0 */
	windowClass.cbSize = sizeof(WNDCLASSEX);							/**< 窗口结构的大小 */
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				/**< 设置窗口类型为，移动时重画，并为窗口取得DC */
	windowClass.lpfnWndProc = (WNDPROC)(WindowProc);					/**< WndProc处理消息 */
	windowClass.hInstance = hInstance;									/**< 设置实例 */
	windowClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);			/**< 设置背景 */
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);					/**< 载入光标 */
	windowClass.lpszClassName = _mClassName;							/**< 设置类名 */
	if (RegisterClassEx(&windowClass) == 0)								/**< 尝试注册窗口类 */
	{																	/**< NOTE: Failure, Should Never Happen */
		MessageBox(HWND_DESKTOP, "注册窗口失败!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														/**< 退出并返回FALSE */
	}
	while (_mIsProgramLooping) {										/**< 循环直道WM_QUIT退出程序 */
		/// 创建一个窗口
		if (_mWindow.Create("多重纹理", _mClassName, _mCreateFullScreen, hInstance, this) == true)
		{
			/// 如果初始化失败，则退出
			if (Init() == false)										/**< 调用自定义的初始化函数 */
			{															/**< 失败 */
				TerminateApplication();									/**< 关闭窗口退出程序 */
			}
			else														/**< 成功开始消息循环 */
			{
				MSG msg;												/**< Window消息结构 */
				bool isMessagePumpActive = true;						/**< 当消息不为空时，处理消息循环 */
				_mLastTickCount = GetTickCount();						/**< 返回当前的计时器的值 */
				_mKeys.Clear();											/**< 清空所有的按键信息 */
				while (isMessagePumpActive == true)						/**< 当消息不为空时，处理消息循环 */
				{														/**< 成功创建窗口，检测窗口消息 */
					if (PeekMessage(&msg, _mWindow, 0, 0, PM_REMOVE) != 0)
					{
						/// 检测是否为WM_QUIT消息
						if (msg.message != WM_QUIT)
						{
							DispatchMessage(&msg);						/**< 如果不是发送消息到消息回调函数中处理 */
						}
						else
						{
							isMessagePumpActive = false;				/**< 如果是，则退出 */
						}
					} else {											/// 如果没有消息
						if (_mIsVisible == false)						/**< 如果窗口不可见 */
						{
							WaitMessage();								/**< 暂停程序，等待消息 */
						}
						else											/**< 如果窗口可见 */
						{												/**< 执行消息循环 */
							DWORD tickCount = GetTickCount();			/**< 返回计时器的当前值 */
							Update(tickCount - _mLastTickCount);		/**< 调用用户自定义的更新函数 */
							_mLastTickCount = tickCount;				/**< 重新设置上一次，计数器的值 */
							Draw();										/**< 调用用户自定义的绘制函数 */
							_mWindow.SwapBuffers();						/**< 交换前后帧缓存 */
						}
					}
				}														/**< 如果isMessagePumpActive == true，则循环 */
			}
			/**< 程序结束 */
			Uninit();													/**< 用户自定义的卸载函数 */
			_mWindow.Destroy();											/**< 删除窗口 */
		}
		else															/**< 如果创建窗口失败 */
		{
			MessageBox(HWND_DESKTOP, "创建OpenGL窗口错误", "Error", MB_OK | MB_ICONEXCLAMATION);
			_mIsProgramLooping = false;									/**< 停止程序循环 */
		}
	}

	UnregisterClass(_mClassName, hInstance);							/**< 取消注册的窗口 */
	return 0;
}

/** 处理窗口消息 */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// LONG user_data = GetWindowLong(hWnd, GWL_USERDATA);					/**< 返回用户自定义的32位的程序附加值 */
	LONG_PTR user_data = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (user_data == 0)
	{
		/// 如果程序第一次运行
		if (uMsg == WM_CREATE)											/**< 处理窗口创建消息 */
		{
			/// 返回窗口结构的指针，它保存刚创建的程序实例的类
			CREATESTRUCT * creation = reinterpret_cast<CREATESTRUCT *>(lParam);
			/// 获得程序实例的指针
			GLApplication * appl = reinterpret_cast<GLApplication *>(creation->lpCreateParams);
			/// 保存程序实例的指针为用户自定义的程序附加值
			// SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(appl));
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(appl));
			appl->_mIsVisible = true;									/**< 设置程序可见 */
			return 0;													/**< 返回 */
		}
	} else {
		/// 如果不是第一次窗口，返回程序实例的指针
		GLApplication * appl = reinterpret_cast<GLApplication *>(user_data);
		return appl->Message(hWnd, uMsg, wParam, lParam);				/**< 调用程序实例自己的消息处理函数 */
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< 调用默认的窗口消息处理函数 */
}

/** 消息循环 */
LRESULT GLApplication::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)												/**< 处理不同的Windows消息 */
	{
	case WM_SYSCOMMAND:											/**< 截获系统命令 */
		switch (wParam)
		{
		case SC_SCREENSAVE:										/**< 截获屏幕保护启动命令 */
		case SC_MONITORPOWER:									/**< 截获显示其省电模式启动命令 */
			return 0;											/**< 不启用这两个命令 */
			break;
		}
		break;													/**< 退出 */

	case WM_CLOSE:												/**< 关闭窗口 */
		TerminateApplication();									/**< 调用TerminateApplication函数 */
		return 0;
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		_mLastTickCount = GetTickCount();							/**< 更新计数器的值 */
		return 0;
		break;

	case WM_MOVE:
		_mWindow.SetPosX(LOWORD(lParam));							/**< 更新鼠标的坐标 */
		_mWindow.SetPosY(HIWORD(lParam));
		return 0;
		break;

	case WM_PAINT:
		if (_mResizeDraw == true)									/**< 如果需要重绘 */
		{
			_mWindow.ReshapeGL();									/**< 重新设置窗口的大小 */
			Draw();													/**< 重新绘制 */
			_mWindow.SwapBuffers();									/**< 交换前后帧缓存 */
		}
		break;

	case WM_SIZING:													/**< 窗口正在改变大小 */
	{
		RECT * rect = (RECT *)lParam;
		_mWindow.SetWidth(rect->right - rect->left);				/**< 设置窗口宽度 */
		_mWindow.SetHeight(rect->bottom - rect->top);				/**< 设置窗口高度 */
		return TRUE;
	}
		break;

	case WM_SIZE:													/**< 窗口改变大小后 */
		switch (wParam)												/**< 处理不同的窗口状态 */
		{
		case SIZE_MINIMIZED:									/**< 是否最小化? */
			_mIsVisible = false;								/**< 如果是，则设置不可见 */
			return 0;
			break;

		case SIZE_MAXIMIZED:									/**< 窗口是否最大化? */
		case SIZE_RESTORED:										/**< 窗口被还原? */
			_mIsVisible = true;									/**< 设置为可见 */
			_mWindow.SetWidth(LOWORD(lParam));					/**< 设置窗口宽度 */
			_mWindow.SetHeight(HIWORD(lParam));					/**< 设置窗口高度 */
			_mWindow.ReshapeGL();								/**< 改变窗口大小 */
			_mLastTickCount = GetTickCount();					/**< 更新计数器的值 */
			return 0;
			break;
		}
		break;

	case WM_KEYDOWN:												/**< 更新按键信息 */
		_mKeys.SetPressed(wParam);
		return 0;
		break;

	case WM_KEYUP:													/**< 更新释放键信息 */
		_mKeys.SetReleased(wParam);
		return 0;
		break;

	case WM_TOGGLEFULLSCREEN:										/**< 切换 全屏/窗口模式 */
		/*_mWindow.SetFullScreen(!_mWindow.GetFullScreen());
		if(!_mWindow.GetFullScreen())
			ShowCursor(true);
		else
			ShowCursor(false);*/

		_mCreateFullScreen = !_mCreateFullScreen;
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< 调用默认的窗口消息处理函数 */
}