#include "MediaPlayer.h"

MediaPlayer::MediaPlayer()
{
	
}

MediaPlayer::MediaPlayer(RECT rect)
{

}

MediaPlayer::~MediaPlayer()
{

}

void MediaPlayer::Init(RECT rect)
{
    this->ProgressBar.LoadCoordinates(0, rect.bottom / 2, rect.right, rect.bottom / 6);


    this->TextBoxSong.SetRect(rect.right / 2 - 150,
        rect.bottom / 2 - 50,
        rect.right / 2 + 150,
        rect.bottom / 2 + 50);
    this->TextBoxSong.SetText(L"NONAME");

    this->TextBoxTime.SetRect(rect.left + 5, rect.top + 65, 200, 150);
    this->TextBoxTime.SetText(L"00:00:00/00:00:00");
    this->TextBoxTime.SetFontCharacteristics(20, 10, 10, 0);
}

void MediaPlayer::Display(HDC hdc)
{
    this->ProgressBar.Paint(hdc);

    SetBkMode(hdc, TRANSPARENT);
    this->TextBoxSong.ShowText(hdc, DT_CENTER);
    this->TextBoxTime.ShowText(hdc, DT_LEFT);
}

void MediaPlayer::Update()
{
    this->ProgressBar.Update();
}

void MediaPlayer::LoadSong()
{

}