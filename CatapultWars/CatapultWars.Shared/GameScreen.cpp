#include "pch.h"
#include "GameScreen.h"

using namespace CatapultWars;

GameScreen::GameScreen(ScreenManager^ manager)
	: m_screenManager(manager)
	, TransitionPosition(1.0f)
{

}

void GameScreen::ExitScreen() {
	if (TransitionOffTime == 0) {
		// If the screen has a zero transition time, remove it immediately.
		Manager->RemoveScreen(this);
	} else {
		// Otherwise flag that it should transition off and then exit.
		IsExiting = true;
	}
}

void GameScreen::Update(double elapsedSeconds, bool otherScreenHasFocus, bool coveredByOtherScreen) {
	m_otherScreenHasFocus = otherScreenHasFocus;

	if (IsExiting) {
		// If the screen is going away to die, it should transition off.
		State = ScreenState::TransitionOff;

		if (!UpdateTransition(elapsedSeconds, TransitionOffTime, 1))
		{
			// When the transition finishes, remove the screen.
			Manager->RemoveScreen(this);
		}
	} else if (coveredByOtherScreen) {
		// If the screen is covered by another, it should transition off.
		if (UpdateTransition(elapsedSeconds, TransitionOffTime, 1))
		{
			// Still busy transitioning.
			State = ScreenState::TransitionOff;
		} else
		{
			// Transition finished!
			State = ScreenState::Hidden;
		}
	} else {
		// Otherwise the screen should transition on and become active.
		if (UpdateTransition(elapsedSeconds, TransitionOnTime, -1))
		{
			// Still busy transitioning.
			State = ScreenState::TransitionOn;
		} else
		{
			// Transition finished!
			State = ScreenState::Active;
		}
	}
}

bool GameScreen::UpdateTransition(double gameTime, double time, int direction) {
	// How much should we move by?
	float transitionDelta;

	if (time == 0)
		transitionDelta = 1;
	else
		transitionDelta = (float)(gameTime/time);

	// Update the transition position.
	TransitionPosition = TransitionPosition + (transitionDelta * direction);

	// Did we reach the end of the transition?
	if (((direction < 0) && (TransitionPosition <= 0)) ||
		((direction > 0) && (TransitionPosition >= 1)))
	{
		TransitionPosition = MathHelper::Clamp(TransitionPosition, 0.0F, 1.0F);
		return false;
	}

	// Otherwise we are still busy transitioning.
	return true;
}