#include "Win32/Win32Window.h"

#include <utility>

namespace Win32
{

int componentId = 0;
std::vector<std::shared_ptr<class Window>> registeredWindows;

Win32Element::Win32Element(std::string className, std::string windowName, DWORD style, int x, int y, int w, int h, HINSTANCE hInstance, LPVOID param, HWND hwnd,
	CallbackFunction onWmCommandCallback, CallbackFunction onEnChangeCallback)
	: m_className(std::move(className))
	, m_windowName(std::move(windowName))
	, m_style(style)
	, m_x(x)
	, m_y(y)
	, m_w(w)
	, m_h(h)
	, m_hMenu((HMENU)componentId++)
	, m_hInstance(hInstance)
	, m_param(param)
	, m_hwnd(hwnd)
	, onWmCommandCallback(std::move(onWmCommandCallback))
	, onEnChangeCallback(std::move(onEnChangeCallback))
{
}

void Win32Element::show()
{
	ShowWindow(m_hwnd, true);
}

void Win32Element::hide()
{
	ShowWindow(m_hwnd, false);
}

void Win32Element::setText(const std::string& text)
{
	SendMessage(m_hwnd, WM_SETTEXT, 0, (LPARAM)text.c_str());
}

std::string Win32Element::getText()
{
	const char buffer[128] = {0};
	SendMessage(m_hwnd, WM_GETTEXT, (WPARAM)128, (LPARAM)buffer);
	return buffer;
}

void Win32Element::addSubcomponent(Win32Element* subcomponent)
{
	subcomponent->m_hInstance = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);
	subcomponent->m_hwnd = CreateWindowA(
		subcomponent->m_className.c_str(),
		subcomponent->m_windowName.c_str(),
		subcomponent->m_style,
		subcomponent->m_x,
		subcomponent->m_y,
		subcomponent->m_w,
		subcomponent->m_h,
		m_hwnd,
		subcomponent->m_hMenu,
		subcomponent->m_hInstance,
		subcomponent->m_param);

	m_subcomponents.emplace_back(subcomponent);
}

std::vector<std::shared_ptr<Win32Element>>& Win32Element::getSubcomponents()
{
	return m_subcomponents;
}

int Win32Element::getId()
{
	return (int)m_hMenu;
}

Button::Button(const std::string& text, int x, int y, int w, int h, CallbackFunction onPressed, bool visible, bool detached, DWORD styles)
	: Win32Element("BUTTON", text, styles | (detached ? 0 : WS_CHILD) | (visible ? WS_VISIBLE : 0), x, y, w, h, nullptr, nullptr, nullptr, std::move(onPressed),
	nullptr)
{
}

TextBox::TextBox(const std::string& text, int x, int y, int w, int h, CallbackFunction onTextChanged, bool visible, bool detached, DWORD styles)
	: Win32Element("EDIT", text, styles | (detached ? 0 : WS_CHILD) | (visible ? WS_VISIBLE : 0) | ES_AUTOHSCROLL | ES_WANTRETURN | WS_BORDER, x, y, w, h,
	nullptr,
	nullptr, nullptr, nullptr, std::move(onTextChanged))
{
}

static LRESULT CALLBACK winproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT PS;
	switch (message) {
		case WM_CREATE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			BeginPaint(hWnd, &PS);
			EndPaint(hWnd, &PS);
			break;
		case WM_COMMAND:
			for (const auto& window: registeredWindows) {
				for (auto& subcomponent: window->getSubcomponents()) {
					if (LOWORD(wParam) == subcomponent->getId()) {
						if (HIWORD(wParam) == EN_CHANGE) {
							if (subcomponent->onEnChangeCallback) {
								subcomponent->onEnChangeCallback();
							}
						} else if (subcomponent->onWmCommandCallback) {
							subcomponent->onWmCommandCallback();
						}
					}
				}
			}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

Window::Window(const std::string& className, const std::string& windowName, UINT style)
	: Win32Element(className, windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, nullptr,
	nullptr, nullptr)
{

	m_wc.cbSize = sizeof(m_wc);
	m_wc.style = 0;
	m_wc.lpfnWndProc = winproc;
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hInstance = m_hInstance;
	m_wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	m_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_wc.lpszMenuName = nullptr;
	m_wc.lpszClassName = className.c_str();
	m_wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassEx(&m_wc)) {
		MessageBox(nullptr, TEXT("Could not register window class"),
			nullptr, MB_ICONERROR);
		return;
	}

	m_hwnd = CreateWindow(
		className.c_str(),
		windowName.c_str(),
		style,
		m_x,
		m_y,
		m_w,
		m_h,
		nullptr,
		m_hMenu,
		m_hInstance,
		m_param);
	if (!m_hwnd) {
		MessageBox(nullptr, TEXT("Could not create window"), nullptr, MB_ICONERROR);
		return;
	}
	registeredWindows.emplace_back(this);
}

WPARAM Window::handleEvents()
{
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

Label::Label(const std::string& text, int x, int y, int w, int h, CallbackFunction onClicked, bool visible, bool detached, DWORD styles)
	: Win32Element("STATIC", text, styles | (detached ? 0 : WS_CHILD) | (visible ? WS_VISIBLE : 0), x, y, w, h, nullptr, nullptr, nullptr, std::move(onClicked),
	nullptr)
{
}
}