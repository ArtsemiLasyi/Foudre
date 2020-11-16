#pragma once

#include <Windows.h>

class TextBox
{
public:
	TextBox();
	~TextBox();
	TextBox(RECT rect);
	TextBox(RECT rect, LPCWSTR text);
	void SetText(LPCWSTR text);
	void SetRect(RECT rect);
	void SetRect(int left, int top, int right, int bottom);
	void ShowText(HDC hdc, int styles);
	void SetFontCharacteristics(int height, int width, int weight, int escapement);
private:
	LPCWSTR text;
	RECT rect;
	LOGFONT logFont;
	void InitFont();
	const int defaultFontHeight = 30;
	const int defaultFontWidth = 15;
	const int defaultFontWeight = 20;
	const int defaultFontEscapement = 0;
};
