#pragma once
#include "Audio.h"

class AudioManager
{
public:
	AudioManager();

	void Update();
	void LoadSound();
	void PlaySound();

private:
	std::unique_ptr<DirectX::AudioEngine> audEngine;
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
};

