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
		float TransitionAlpha;
		ScreenState State;
		bool IsExiting;
		ScreenManager^ Manager;
		bool IsSerializable;

		bool IsActive() {
			return !m_otherScreenHasFocus && (State == ScreenState::TransitionOn || State == ScreenState::Active);
		}

		void ExitScreen();

		virtual void LoadContent() {}
		virtual void UnloadContent() {}
		virtual void Update(double elapsedSeconds, bool coveredByOtherScreen, bool otherScreenHasFocus);
		virtual void HandleInput(InputState^ input) {}
		virtual void Draw() {}
		virtual void Serialize(Stream* stream) {}
		virtual void Deserialize(Stream* stream) {}


	private:
		bool m_otherScreenHasFocus;

		bool UpdateTransition(double gameTime, double time, int direction);
	};
}