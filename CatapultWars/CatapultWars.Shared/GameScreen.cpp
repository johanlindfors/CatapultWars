#include "pch.h"

using namespace CatapultWars;

void GameScreen::ExitScreen() {
	if (TransitionOffTime == 0) {
		// If the screen has a zero transition time, remove it immediately.
		Manager->RemoveScreen(this);
	} else {
		// Otherwise flag that it should transition off and then exit.
		IsExiting = true;
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
	TransitionPosition += transitionDelta * direction;

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