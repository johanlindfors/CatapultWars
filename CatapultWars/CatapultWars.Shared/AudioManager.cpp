#include "pch.h"
#include "AudioManager.h"

using namespace DirectX;

AudioManager::AudioManager() 
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audioEngine.reset(new AudioEngine(eflags));
}

bool AudioManager::Update() {
	bool result = m_audioEngine->Update();
	if (!result)
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			
		}
	}
	return result;
}

void AudioManager::LoadSound(const char* fileName, const char* assetName) {
	m_soundEffect.reset(new SoundEffect(m_audioEngine.get(), L"Assets\\Sounds\\Win.wav"));
}

void AudioManager::PlaySound(const char* assetName) {
	m_soundEffect->Play();
}
