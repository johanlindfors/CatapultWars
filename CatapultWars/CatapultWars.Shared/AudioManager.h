#pragma once
#include "Audio.h"

class AudioManager
{
public:
	AudioManager();
	~AudioManager() {}

	bool Update();
	void LoadSound(const char* fileName, const char* assetName);
	void PlaySound(const char* assetName);

private:
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unique_ptr<DirectX::SoundEffect> m_soundEffect;

	
};

