#include <Windows.h>

#pragma once
class SoundtrackProgressbar
{
public:
	SoundtrackProgressbar();
	SoundtrackProgressbar(int x, int y);
	SoundtrackProgressbar(int x, int y, int width, int height);
	~SoundtrackProgressbar();
	bool CheckPoint(int x, int y);
	void LoadCoordinates(int x, int y, int width, int height);
	int getRedBorder();
	int getGreenBorder();
	int getBlueBorder();
	int getRedRegion();
	int getGreenRegion();
	int getBlueRegion();
	void Paint(HDC hdc);
	void Update();
	void SetProgress(int x);
	void SetDelta(int delta);
	int getIndent();
private:
	int x;
	int y;
	int height;
	int width;
	int redBorder;
	int greenBorder;
	int blueBorder;
	int redRegion;
	int greenRegion;
	int blueRegion;
	int progressX;
	const int defaultRedBorder = 255;
	const int defaultGreenBorder = 255;
	const int defaultBlueBorder = 255;
	const int defaultRedRegion = 135;
	const int defaultGreenRegion = 115;
	const int defaultBlueRegion = 238;
	const int indent = 5;
	int delta = 20;
};

