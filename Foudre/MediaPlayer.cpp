#include "MediaPlayer.h"
#include "Song.h"

MediaPlayer::MediaPlayer()
{
	
}

MediaPlayer::MediaPlayer(RECT rect)
{

}

MediaPlayer::~MediaPlayer()
{
    delete this->player;
    delete this->song;
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
    this->player = new Player(&Handle);
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
    MFTIME time;
    this->player->GetDuration(&time);
    int seconds = this->ConvertMFTIMEToSeconds(time);
    std::wstring strSongTime = this->GetTimeInStr(this->secondsPlayed);
    if (this->secondsPlayed >= seconds)
    {
        this->secondsPlayed = 0;
        seconds = 0;
    }
    else
    {
        strSongTime += L"/" + this->GetTimeInStr(seconds);
        this->TextBoxTime.SetText(strSongTime.c_str());
        this->ProgressBar.SetDelta(this->ProgressBar.getWidth() / seconds);
        this->ProgressBar.Update();
    }
}

void MediaPlayer::LoadSong(PCWSTR songname)
{
    this->player = new Player(&Handle);
    this->song = new Song(songname, &Handle);
    Handle = this->player->OpenURL(songname);
    this->TextBoxSong.SetText(song->Title);
    this->Update();  
}

void MediaPlayer::SetTime(int x)
{
    this->ProgressBar.SetProgress(x);
    MFTIME time;
    this->player->GetDuration(&time);
    MFTIME currentTime = time * this->ProgressBar.getProgress() / this->ProgressBar.getWidth();
    this->player->SetPosition(currentTime);
    this->secondsPlayed = this->ConvertMFTIMEToSeconds(currentTime);
}


int MediaPlayer::ConvertMFTIMEToSeconds(MFTIME time)
{
    const MFTIME ONE_SECOND = 10000000;
    return time / ONE_SECOND;
}

std::wstring MediaPlayer::GetTimeInStr(int time)
{
    const int MinutesPerHour = 60;
    const int SecondsPerMinute = 60;
    
    int seconds = time % SecondsPerMinute;
    int minutes = time / SecondsPerMinute;
    int hours = minutes / MinutesPerHour;
    minutes = minutes % MinutesPerHour;


    WCHAR arrayseconds[4];
    WCHAR arrayminutes[4];
    WCHAR arrayhours[4];
    _itow_s(seconds, arrayseconds, 10);
    _itow_s(minutes, arrayminutes, 10);
    _itow_s(hours, arrayhours, 10);
    LPCWSTR strseconds = arrayseconds;
    LPCWSTR strminutes = arrayminutes;
    LPCWSTR strhours = arrayhours;
    LPCWSTR separator = L":";
    LPCWSTR zero = L"0";

    std::wstring df = L"";

    if (hours < 10)
        df += zero + std::wstring(strhours);
    else
        df += std::wstring(strhours);

    df += separator;

    if (minutes < 10)
        df += zero + std::wstring(strminutes);
    else
        df += std::wstring(strminutes);

    df += separator;

    if (seconds < 10)
        df += zero + std::wstring(strseconds);
    else
        df += std::wstring(strseconds);
    return df;
}