#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace Win32
{

typedef std::function<void()> CallbackFunction;

struct Win32Element
{
	protected:
		std::string m_className;
		std::string m_windowName;
		DWORD m_style;
		int m_x;
		int m_y;
		int m_w;
		int m_h;
		HMENU m_hMenu;
		HINSTANCE m_hInstance;
		LPVOID m_param;
		HWND m_hwnd;
		std::vector<std::shared_ptr<Win32Element>> m_subcomponents;

	public:
		CallbackFunction onWmCommandCallback;
		CallbackFunction onEnChangeCallback;

	public:
		Win32Element(std::string className, std::string windowName, DWORD style, int x, int y, int w, int h, HINSTANCE hInstance, LPVOID param, HWND hwnd,
			CallbackFunction onWmCommandCallback, CallbackFunction onEnChangeCallback);

		void show();
		void hide();
		void setText(const std::string& text);
		std::string getText();
		void addSubcomponent(Win32Element* subcomponent);

		std::vector<std::shared_ptr<Win32Element>>& getSubcomponents();
		int getId();
};

class Button : public Win32Element
{
	public:
		Button(const std::string& text, int x, int y, int w, int h, CallbackFunction onPressed = nullptr, bool visible = true, bool detached = false,
			DWORD styles = 0);
};

class TextBox : public Win32Element
{
	public:
		TextBox(const std::string& text, int x, int y, int w, int h, CallbackFunction onTextChanged = nullptr, bool visible = true, bool detached = false,
			DWORD styles = 0);
};

class Label : public Win32Element
{
	public:
		Label(const std::string& text, int x, int y, int w, int h, CallbackFunction onTextChanged = nullptr, bool visible = true, bool detached = false,
			DWORD styles = 0);
};

class Window : public Win32Element
{
	public:
		Window(const std::string& className, const std::string& windowName, UINT style);

		static WPARAM handleEvents();

	private:
		WNDCLASSEX m_wc = {};
};
}


