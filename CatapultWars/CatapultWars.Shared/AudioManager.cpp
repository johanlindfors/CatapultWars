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

AudioManager::~AudioManager() {
	for (auto & sound : m_soundBank) {
		if (sound.first) {
			delete(sound.first);
		}
		if (sound.second) {
			sound.second.release();
		}
	}

	for (auto & sound : m_soundEffects) {
		if (sound.first) {
			delete(sound.first);
		}
		if (sound.second) {
			sound.second.release();
		}
	}

	for (auto & soundName : m_soundNames)
	{
		if (soundName.first) {
			delete(soundName.first);
		}
		if (soundName.second) {
			delete(soundName.second);
		}
	}
}

void AudioManager::LoadSounds() {
	m_soundNames["CatapultExplosion"] = L"Assets\\Sounds\\CatapultExplosion.wav";
	m_soundNames["Lose"] = L"Assets\\Sounds\\Lose.wav";
	m_soundNames["Win"] = L"Assets\\Sounds\\Win.wav";
	m_soundNames["BoulderHit"] = L"Assets\\Sounds\\BoulderHit.wav";
	m_soundNames["CatapultFire"] = L"Assets\\Sounds\\CatapultFire.wav";
	m_soundNames["RopeStretch"] = L"Assets\\Sounds\\RopeStretch.wav";

	for (auto & soundName : m_soundNames) {
		auto soundEffect = new SoundEffect(m_audioEngine.get(), soundName.second);
		m_soundBank[soundName.first] = soundEffect->CreateInstance();
	}
}

bool AudioManager::Update() {
	bool result = m_audioEngine->Update();
	if (!result)
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			OutputDebugString(L"Lost the audio device!");
		}
	}
	return result;
}

void AudioManager::PlaySound(const char* assetName, bool looped) {
	//std::string temp(assetName);
	//std::wstring log(temp.begin(), temp.end());
	//OutputDebugString(log.c_str());
	auto state = m_soundBank[assetName]->GetState();
	if (state != DirectX::SoundState::STOPPED) {
		m_soundBank[assetName]->Stop();
	}
	m_soundBank[assetName]->Play(looped);
}

void AudioManager::StopSound(const char* assetName) {
	m_soundBank[assetName]->Stop();
}