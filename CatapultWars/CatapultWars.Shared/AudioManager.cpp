#include "pch.h"
#include "AudioManager.h"

using namespace DirectX;

AudioManager::AudioManager() 
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	audEngine.reset(new AudioEngine(eflags));

}

void AudioManager::Update() {
	if (!audEngine->Update())
	{
		// No audio device is active
		if (audEngine->IsCriticalError())
		{
			
		}
	}
}

void AudioManager::LoadSound() {
	soundEffect.reset(new SoundEffect(audEngine.get(), L"Assets\\Sounds\\Win.wav"));
}

void AudioManager::PlaySound() {
	soundEffect->Play();
}
