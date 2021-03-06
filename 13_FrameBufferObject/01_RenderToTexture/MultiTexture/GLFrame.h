#ifndef	__GLFRAME_H__
#define	__GLFRAME_H__
#include "GLWindow.h"
class Keys
{
public:
	/** 构造函数 */
	Keys() { Clear(); }
	/** 清空所有的按键信息 */
	void Clear() { ZeroMemory(&_mKeyDown, sizeof(_mKeyDown)); }
	/** 判断某个键是否按下 */
	bool IsPressed(unsigned int key) { return (key < MAX_KEYS) ? (_mKeyDown[key] == true) : false; }
	/** 设置某个键被按下 */
	void SetPressed(unsigned int key) { if (key < MAX_KEYS) _mKeyDown[key] = true; }
	/** 设置某个键被释放 */
	void SetReleased(unsigned int key) { if (key < MAX_KEYS) _mKeyDown[key] = false; }

private:
	static const unsigned int	MAX_KEYS = 256;
	bool						_mKeyDown[MAX_KEYS];
};

/** 基本的程序类，继承它用来创建OpenGL程序 */
class GLApplication
{
public:
	/** 创建一个全局的Create函数，这个函数必须被继承类实现 */
	static GLApplication * Create(const char * class_name);	/**< 创建你自己的子类 */
	
	/** 虚析构函数 */
	virtual ~GLApplication() {}

protected:
	/** 下面的函数必须被继承类实现，完成基本的OpenGL渲染过程 */
	virtual bool	Init() = 0;							/**< OpenGL的初始化 */
	virtual void	Uninit() = 0;						/**< OpenGL的卸载 */
	virtual void	Update(DWORD milliseconds) = 0;		/**< 执行OpenGL程序的更新 */
	virtual void	Draw() = 0;							/**< 绘制OpenGL场景 */

	/** 通用的函数 */
	void	ToggleFullscreen();									/**< 切换 全屏/窗口模式 */
	void	TerminateApplication();								/**< 结束程序 */
	void	ResizeDraw(bool enable) { _mResizeDraw = enable; }	/**< 设置在窗口改变大小的时候，可以绘制 */

	Keys	_mKeys;												/**< 按键类 */
	/** 构造函数 */
	GLApplication(const char * class_name);
	GLWindow	_mWindow;										/**< Window类 */

private:
	/** 程序的主循环 */
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	int		Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	/** 消息处理回调函数 */
	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT	Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static const UINT WM_TOGGLEFULLSCREEN = (WM_USER + 1);		/**< 自定义消息，在切换窗口模式的时候发送 */

	const char* _mClassName;									/**< 程序名 */
	bool		_mIsProgramLooping;								/**< 程序循环标记，如果为false，则退出程序 */
	bool        _mCreateFullScreen;                             /**< 若为true，则创建全屏模式 */
	bool		_mIsVisible;									/**< 窗口是否可见 */
	bool		_mResizeDraw;									/**< 是否在改变大小时，调用了绘制函数 */
	DWORD		_mLastTickCount;								/**< 上一次计时器的值 */
};

#endif	// __GLFRAMEWORK_H__