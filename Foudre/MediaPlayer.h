#pragma once

#include "SoundtrackProgressbar.h"
#include "TextBox.h"
#include "Player.h"
#include "Song.h"
#include <string>

class MediaPlayer
{
public:
	SoundtrackProgressBar ProgressBar;
	TextBox TextBoxSong;
	TextBox TextBoxTime;
	Player* player;
	HRESULT Handle;
	MediaPlayer();
	MediaPlayer(RECT rect);
	~MediaPlayer();
	void LoadSong(PCWSTR songname);
	void Init(RECT rect);
	void Display(HDC hdc);
	void Update();
	int secondsPlayed = 0;
	void SetTime(int x);
private:
	Song* song;
	std::wstring GetTimeInStr(int time);
	int ConvertMFTIMEToSeconds(MFTIME time);
};

