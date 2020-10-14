#include "soundtrackprogressbar.h"
#include <Windows.h>

SoundtrackProgressbar::SoundtrackProgressbar()
{
	this->x = 0;
	this->y = 0;
	this->height = 0;
	this->width = 0;
	this->redBorder = defaultRedBorder;
	this->greenBorder = defaultGreenBorder;
	this->blueBorder = defaultBlueBorder;
	this->redRegion = defaultRedRegion;
	this->greenRegion = defaultGreenRegion;
	this->blueRegion = defaultBlueRegion;
	this->progressX = 0;
}

SoundtrackProgressbar::SoundtrackProgressbar(int x, int y)
{
	this->x = x;
	this->y = y;
	this->height = 0;
	this->width = 0;
	this->redBorder = defaultRedBorder;
	this->greenBorder = defaultGreenBorder;
	this->blueBorder = defaultBlueBorder;
	this->redRegion = defaultRedRegion;
	this->greenRegion = defaultGreenRegion;
	this->blueRegion = defaultBlueRegion;
	this->progressX = 0;
}

SoundtrackProgressbar::SoundtrackProgressbar(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->redBorder = defaultRedBorder;
	this->greenBorder = defaultGreenBorder;
	this->blueBorder = defaultBlueBorder;
	this->redRegion = defaultRedRegion;
	this->greenRegion = defaultGreenRegion;
	this->blueRegion = defaultBlueRegion;
	this->progressX = 0;
}

SoundtrackProgressbar::~SoundtrackProgressbar()
{

}

bool SoundtrackProgressbar::CheckPoint(int x, int y)
{
	bool rightX = ((x > this->x) && (x < this->x + this->width));
	bool rightY = ((y > this->y) && (y < this->y + this->height));
	if (rightX && rightY)
		return true;
	else
		return false;
}

int SoundtrackProgressbar::getRedBorder()
{
	return this->redBorder;
}

int SoundtrackProgressbar::getGreenBorder()
{
	return this->greenBorder;
}

int SoundtrackProgressbar::getBlueBorder()
{
	return this->blueBorder;
}

int SoundtrackProgressbar::getRedRegion()
{
	return this->redRegion;
}

int SoundtrackProgressbar::getGreenRegion()
{
	return this->greenRegion;
}

int SoundtrackProgressbar::getBlueRegion()
{
	return this->blueRegion;
}

void SoundtrackProgressbar::Paint(HDC hdc)
{
	RECT rect;
	rect.left = this->x;
	rect.top = this->y;
	rect.right = this->x + this->width;
	rect.bottom = this->y + this->height;

	COLORREF colorBorder = RGB(this->redBorder,
							   this->greenBorder,
							   this->blueBorder);
	COLORREF colorRegion = RGB(this->redRegion,
							   this->greenRegion,
							   this->blueRegion);
	HBRUSH hBrushBorder = CreateSolidBrush(colorBorder);
	HBRUSH hBrushRegion = CreateSolidBrush(colorRegion);

	FillRect(hdc, &rect, hBrushBorder);

	rect.left = this->x + indent;
	rect.top = this->y + indent;
	rect.right = this->x + this->progressX;
	rect.bottom = this->y + this->height - indent;
	FillRect(hdc, &rect, hBrushRegion);
}

void SoundtrackProgressbar::Update()
{
	this->progressX += this->delta;
	if ((this->progressX < this->x) || (this->progressX > this->x + this->width))
	{
		this->progressX = 0;
	}
}

void SoundtrackProgressbar::SetProgress(int x)
{
	this->progressX = x;
}

void SoundtrackProgressbar::SetDelta(int delta)
{
	this->delta = delta;
}

void SoundtrackProgressbar::LoadCoordinates(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

int SoundtrackProgressbar::getIndent()
{
	return this->indent;
}