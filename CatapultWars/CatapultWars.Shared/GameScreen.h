#pragma once

namespace CatapultWars {

	enum ScreenState {
		TransitionOn,
		Active,
		TransitionOff,
		Hidden
	};

	ref class ScreenManager;
	ref class InputState;
	class Stream;

	ref class GameScreen {
	internal:
		bool IsPopup;
		double TransitionOnTime;
		double TransitionOffTime;
		float TransitionPosition;
		property float TransitionAlpha {
			float get() {
				return 1.0f - TransitionPosition;
			}
		}
		ScreenState State;
		bool IsExiting;
		ScreenManager^ Manager;
		bool IsSerializable;

		bool IsActive() {
			return !m_otherScreenHasFocus && (State == ScreenState::TransitionOn || State == ScreenState::Active);
		}

		GameScreen(ScreenManager^ manager);
		void ExitScreen();

		virtual void LoadContent(ID3D11Device* device) {}
		virtual void UnloadContent() {}
		virtual void Update(double elapsedSeconds, bool coveredByOtherScreen, bool otherScreenHasFocus);
		virtual void HandleInput(InputState^ input) {}
		virtual void Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, double elapsedSeconds) {}
		virtual void Serialize(Stream* stream) {}
		virtual void Deserialize(Stream* stream) {}

	internal:
		ScreenManager^ m_screenManager;

	private:
		bool m_otherScreenHasFocus;

		bool UpdateTransition(double gameTime, double time, int direction);
	};
}