#pragma once

#include "SoundtrackProgressbar.h"
#include "TextBox.h"

class MediaPlayer
{
public:
	SoundtrackProgressBar ProgressBar;
	TextBox TextBoxSong;
	TextBox TextBoxTime;

	MediaPlayer();
	MediaPlayer(RECT rect);
	~MediaPlayer();
	void LoadSong();
	void Init(RECT rect);
	void Display(HDC hdc);
	void Update();
private:
};

