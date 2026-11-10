#include<windows.h>
#include<cstdio>
#include<cstring>
#include<vector>
#include<string>
#include<deque>
#include<list>
#include<queue>
#include<stack>
#include<set>
#include<bitset>
#include<algorithm>
#include<functional>
#include<iterator>
#include<utility>
#include<cmath>
#include<cstdlib>
#include<cctype>
#include<complex>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<sstream>
#include<ctime>
typedef BOOL (WINAPI *SetProcessDPIAwareFunc)(void);

struct Api
{
	void EnableDpiAwareness()
	{
		HMODULE hUser32 = LoadLibraryA("user32.dll");
		if (!hUser32)
			return;
		SetProcessDPIAwareFunc pFunc =
			(SetProcessDPIAwareFunc)GetProcAddress(hUser32, "SetProcessDPIAware");
		if (pFunc)
			pFunc();
		// 注意：这里不 FreeLibrary，因为进程还要继续用
	}
	enum MouseButton
	{
		MOUSE_LEFT   = 0,
		MOUSE_MIDDLE = 1,
		MOUSE_RIGHT  = 2
	};
	class ScreenCache
	{
	public:
		ScreenCache()
			: m_hdcMem(NULL), m_hBmp(NULL), m_pBits(NULL),
			  m_oldBmp(NULL), m_width(0), m_height(0),
			  m_captured(false)
		{
			m_hdcScreen = GetDC(NULL);
		}
	
		~ScreenCache()
		{
			Release();
			if (m_hdcScreen)
				ReleaseDC(NULL, m_hdcScreen);
		}
	
		// 获取整个屏幕的分辨率
		void InitFullScreen()
		{
			if (m_hdcScreen == NULL)
				return;
	
			m_width  = GetDeviceCaps(m_hdcScreen, HORZRES);
			m_height = GetDeviceCaps(m_hdcScreen, VERTRES);
	
			SetupDIB(m_width, m_height);
		}
	
		// 只缓存指定屏幕矩形（左上角 sx,sy，宽 w，高 h）
		void InitRegion(int sx, int sy, int w, int h)
		{
			if (m_hdcScreen == NULL)
				return;
	
			m_regionX = sx;
			m_regionY = sy;
			m_width   = w;
			m_height  = h;
	
			SetupDIB(w, h);
		}
	
		// 重新从屏幕抓取（刷新缓存）
		bool Capture()
		{
			if (m_hdcMem == NULL)
				return false;
	
			int sx = m_regionX;
			int sy = m_regionY;
	
			// 若未指定区域，默认抓整屏
			if (m_width == GetDeviceCaps(m_hdcScreen, HORZRES) &&
				m_height == GetDeviceCaps(m_hdcScreen, VERTRES))
			{
				sx = 0;
				sy = 0;
			}
	
			BOOL ok = BitBlt(m_hdcMem, 0, 0, m_width, m_height,
							 m_hdcScreen, sx, sy, SRCCOPY);
			m_captured = (ok != FALSE);
			return m_captured;
		}
	
		// 从缓存中读取像素（相对缓存左上角坐标）
		bool GetPixelRGB(int x, int y, BYTE* r, BYTE* g, BYTE* b) const
		{
			if (!m_captured || m_pBits == NULL)
				return false;
			if (x < 0 || y < 0 || x >= m_width || y >= m_height)
				return false;
	
			// DIB 每行按 4 字节对齐；32 位色时 rowBytes = width * 4
			int rowBytes = m_width * 4;
			const BYTE* p = m_pBits + y * rowBytes + x * 4;
	
			// 内存中是 BGR 顺序
			if (b) *b = p[0];
			if (g) *g = p[1];
			if (r) *r = p[2];
			return true;
		}
	
		// 直接从缓存中取 COLORREF（0x00BBGGRR）
		COLORREF GetPixelColor(int x, int y) const
		{
			BYTE r = 0, g = 0, b = 0;
			if (!GetPixelRGB(x, y, &r, &g, &b))
				return CLR_INVALID;
			return RGB(r, g, b);
		}
	
		int Width() const
		{
			return m_width;
		}
		int Height() const
		{
			return m_height;
		}
	
	private:
		void SetupDIB(int w, int h)
		{
			if (m_hdcScreen == NULL || w <= 0 || h <= 0)
				return;
	
			// 清理旧的
			if (m_hBmp)
			{
				if (m_hdcMem && m_oldBmp)
					SelectObject(m_hdcMem, m_oldBmp);
				DeleteObject(m_hBmp);
				m_hBmp = NULL;
				m_oldBmp = NULL;
			}
			if (m_hdcMem)
			{
				DeleteDC(m_hdcMem);
				m_hdcMem = NULL;
			}
	
			m_hdcMem = CreateCompatibleDC(m_hdcScreen);
			if (m_hdcMem == NULL)
				return;
	
			BITMAPINFO bmi;
			memset(&bmi, 0, sizeof(bmi));
			bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth	   = w;
			bmi.bmiHeader.biHeight	  = -h;   // 负数 = 自上而下，行 0 在顶部
			bmi.bmiHeader.biPlanes	  = 1;
			bmi.bmiHeader.biBitCount	= 32;
			bmi.bmiHeader.biCompression = BI_RGB;
	
			void* bits = NULL;
			m_hBmp = CreateDIBSection(m_hdcScreen, &bmi, DIB_RGB_COLORS,
									  &bits, NULL, 0);
			if (m_hBmp == NULL)
				return;
	
			m_pBits = (BYTE*)bits;
			m_oldBmp = (HBITMAP)SelectObject(m_hdcMem, m_hBmp);
		}
	
		void Release()
		{
			if (m_hdcMem && m_oldBmp)
			{
				SelectObject(m_hdcMem, m_oldBmp);
			}
			if (m_hBmp)
			{
				DeleteObject(m_hBmp);
			}
			if (m_hdcMem)
			{
				DeleteDC(m_hdcMem);
			}
			m_hdcMem=NULL;
			m_hBmp=NULL;
			m_pBits=NULL;
			m_oldBmp=NULL;
		}
		HDC	m_hdcScreen;
		HDC	m_hdcMem;
		HBITMAP m_hBmp;
		BYTE* m_pBits;
		HBITMAP m_oldBmp;
		int m_width;
		int m_height;
		int m_regionX;
		int m_regionY;
		bool m_captured;
	};
	ScreenCache* screen;
	bool move_mouse_to(int x,int y)
	{
		return SetCursorPos(x,y)!=false;
	}
	bool mouse_button_down(MouseButton button)
	{
		DWORD flag=0;
		switch(button)
			{
			case MOUSE_LEFT:   flag=MOUSEEVENTF_LEFTDOWN;   break;
			case MOUSE_MIDDLE: flag=MOUSEEVENTF_MIDDLEDOWN; break;
			case MOUSE_RIGHT:  flag=MOUSEEVENTF_RIGHTDOWN;  break;
			default:		   return false;
		}
		mouse_event(flag, 0, 0, 0, 0);
		return true;
	}
	bool mouse_button_up(MouseButton button)
	{
		DWORD flag = 0;
		switch (button)
		{
		case MOUSE_LEFT:   flag = MOUSEEVENTF_LEFTUP;   break;
		case MOUSE_MIDDLE: flag = MOUSEEVENTF_MIDDLEUP; break;
		case MOUSE_RIGHT:  flag = MOUSEEVENTF_RIGHTUP;  break;
		default:		   return false;
		}
		mouse_event(flag, 0, 0, 0, 0);
		return true;
	}
	bool mouse_button_click(MouseButton button)
	{
		if(!mouse_button_down(button))
		{
			return false;
		}
		return mouse_button_up(button);
	}
	
	bool capture()
	{
		return screen->Capture();
	}
	bool get_pixel_rgb(int x,int y,BYTE* r,BYTE* g,BYTE* b)
	{
		return screen->GetPixelRGB(x,y,r,g,b);
	}
	void init()
	{
		printf("[api]: initing...\n");
		EnableDpiAwareness();
		printf("Screen: %d * %d\n",
			   GetDeviceCaps(GetDC(NULL), HORZRES),
			   GetDeviceCaps(GetDC(NULL), VERTRES));
		printf("Physical: %d * %d\n",
			   GetSystemMetrics(SM_CXSCREEN),
			   GetSystemMetrics(SM_CYSCREEN));
		
		screen=new ScreenCache;
		screen->InitFullScreen();
		printf("[api]: init complete\n");
	}
		// ==================== 键盘控制 ====================
	
	// 将字符转换为虚拟键码（支持 a-z, A-Z, 0-9 及常见符号）
	static BYTE CharToVK(char c)
	{
		if (c >= 'a' && c <= 'z') return (BYTE)(c - 'a' + 'A');
		if (c >= 'A' && c <= 'Z') return (BYTE)c;
		if (c >= '0' && c <= '9') return (BYTE)c;
		switch (c)
		{
		case ' ':  return VK_SPACE;
		case '\n': return VK_RETURN;
		case '\t': return VK_TAB;
		case '\b': return VK_BACK;
		case 27:   return VK_ESCAPE;
		default:   return 0;
		}
	}
	
	// 内部通用：按下或抬起一个虚拟键
	bool key_event_impl(WORD vk, bool down)
	{
		if (vk == 0)
			return false;
	
		INPUT input;
		memset(&input, 0, sizeof(input));
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = vk;
	
		// 扩展键标志：方向键、Insert/Delete/Home/End/PgUp/PgDn、右 Ctrl/Alt、
		// 小键盘除号、NumLock 等。这里对常见扩展键做标记。
		switch (vk)
		{
		case VK_INSERT: case VK_DELETE:
		case VK_HOME:   case VK_END:
		case VK_PRIOR:  case VK_NEXT:
		case VK_LEFT:   case VK_RIGHT:
		case VK_UP:     case VK_DOWN:
		case VK_NUMLOCK: case VK_DIVIDE:
		case VK_RCONTROL: case VK_RMENU:
			input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
			break;
		default:
			break;
		}
	
		if (!down)
			input.ki.dwFlags |= KEYEVENTF_KEYUP;
	
		UINT sent = SendInput(1, &input, sizeof(INPUT));
		return sent == 1;
	}
	
	// 按下某个键。参数可以是：
	//   - 虚拟键码（如 VK_RETURN, VK_SHIFT, VK_F1, 'A' 等）
	//   - 单个字符（如 'a', 'A', '1', ' '）
	bool key_button_down(WORD vk)
	{
		return key_event_impl(vk, true);
	}
	
	bool key_button_down(char c)
	{
		return key_event_impl(CharToVK(c), true);
	}
	
	// 抬起某个键
	bool key_button_up(WORD vk)
	{
		return key_event_impl(vk, false);
	}
	
	bool key_button_up(char c)
	{
		return key_event_impl(CharToVK(c), false);
	}
	
	// ==================== 鼠标位置 ====================
	
	// 读取当前鼠标屏幕坐标。成功返回 true，坐标写入 x / y。
	bool mouse_pos(int* x, int* y)
	{
		POINT pt;
		if (!GetCursorPos(&pt))
			return false;
		if (x) *x = pt.x;
		if (y) *y = pt.y;
		return true;
	}
	
	// 重载：返回 POINT
	POINT mouse_pos()
	{
		POINT pt = { 0, 0 };
		GetCursorPos(&pt);
		return pt;
	}
};
Api* api=nullptr;
int main()
{
	api=new Api;
	api->init();
	return 0;
}

