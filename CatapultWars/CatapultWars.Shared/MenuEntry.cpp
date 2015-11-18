#include "pch.h"
#include "MenuEntry.h"

using namespace CatapultWars;

MenuEntry::MenuEntry(String^ text) 
	: Text(text)
{

}

void MenuEntry::Update(MenuScreen^ screen, bool isSelected, double elapsedSeconds) {

}

void MenuEntry::Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, MenuScreen^ screen, bool isSelected, double elapsedSeconds) {
	auto font = screen->Manager->Font;

	Vector2 origin(0, font->GetLineSpacing() / 2);
	float scale = 1.0f;

	font->DrawString(spriteBatch.get(), Text->Data(), Position, Colors::White, 0, origin, scale, SpriteEffects::SpriteEffects_None, 0);
}