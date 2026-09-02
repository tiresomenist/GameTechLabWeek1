#pragma once
#include <xaudio2.h>
#include <map>
#include <vector>
#include <string>

#define SOUND_KEY_BGM L"bgm_2"
#define ENEMY_HIT L"hit_enemy"
#define PLAYER_HIT L"hit_player"
#define GAME_OVER L"game_over"
#define GAME_CLEAR L"game_clear"
#define UI_OPEN L"ui_open"
#define UI_CLOSE L"ui_close"
#define ENEMY_DEAD L"enemy_dead"
#define LEVEL_UP L"level_up"
#define PICK_EXP_ORB L"pickupCoin"

struct SoundData
{
    WAVEFORMATEX wfx;
    std::vector<BYTE> audioBytes;
};

struct WavChunk
{
    char id[4];
    DWORD size;
};

class USoundManager
{
    // Singleton
private:
    USoundManager();
    ~USoundManager() = default;

public:
    static USoundManager* GetInstance();

private:
    static USoundManager* instance;

    // Sound
public:
    void Init();
    void Release();

    void PlayBGM(const std::wstring& key, bool loop = true);
    void PlaySFX(const std::wstring& key);

    void StopBGM();

    void SetBGMVolume(float volume);
    void SetSFXVolume(float volume);
    float GetBGMVolume() { return bgmVolume; };
    float GetSFXVolume() { return sfxVolume; };

private:
    float bgmVolume = 1.0f;
    float sfxVolume = 1.0f;

    bool LoadWavFile(const std::wstring& filePath, SoundData& outData);
    bool LoadBGM(const std::wstring& key, const std::wstring& filePath);
    bool LoadSFX(const std::wstring& key, const std::wstring& filePath);

    // XAudio2
    IXAudio2* XAudio2;
    IXAudio2MasteringVoice* MasterVoice;

    // Sound Resource
    std::map<std::wstring, SoundData> bgmMap;
    std::map<std::wstring, SoundData> sfxDataMap;
    std::map<std::wstring, std::vector<IXAudio2SourceVoice*>> sfxPoolMap;
    IXAudio2SourceVoice* currentBGMVoice;
};

