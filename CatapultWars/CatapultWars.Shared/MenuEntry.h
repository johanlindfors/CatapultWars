#pragma once
#include "MenuEntry.h"
#include "MenuScreen.h"

namespace CatapultWars {
	
	ref class MenuEntry {

	internal:
		MenuEntry(String^ text);
		virtual void Update(MenuScreen^ screen, bool isSelected, double elapsedSeconds);
		virtual void Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, MenuScreen^ screen, bool isSelected, double elapsedSeconds);

		Platform::String^ Text;
		DirectX::SimpleMath::Vector2 Position;

	private:
//		Platform::String^	m_text;
		float m_selectionFade;
//		DirectX::SimpleMath::Vector2 m_position;
	};
}