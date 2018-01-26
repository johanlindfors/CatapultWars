#pragma once
#include "Audio.h"

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool Update();
	void LoadSounds();
	void PlaySound(const char* assetName, bool looped = false);
	void StopSound(const char* assetName);

private:
	//void LoadSound(const char* fileName, const char* assetName);
	
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unique_ptr<DirectX::SoundEffectInstance> m_soundEffect;

	std::map<const char*, std::unique_ptr<DirectX::SoundEffect>> m_soundEffects;
	std::map<const char*, std::unique_ptr<DirectX::SoundEffectInstance>> m_soundBank;
	std::map<const char*, const wchar_t*> m_soundNames;
};

