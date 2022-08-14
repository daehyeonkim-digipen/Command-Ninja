#include "../Engine/Engine.h"
#include "Bar.h"
#include "Fonts.h"

Bar::Bar(math::vec2 pos, UIType layerLevel, std::string filepath_empty, PROJECT::GameObject* objectT, double isValueFixed_)
	: UI(objectT, layerLevel), isValueFixed(isValueFixed_), currValue(0), maxValue(0), smoothValue(0)
{
	SetPosition(pos);

	AddTexture(filepath_empty); // order
	filepath_empty = filepath_empty.substr(0, filepath_empty.size() - 10);
	AddTexture(filepath_empty + "_full.png");
	AddTexture(filepath_empty + "_full.png");
}

//Bar::~Bar() {
//	delete &infoFont;
//}

void Bar::Update(double) {
	//infoFont = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(std::to_string((int)currValue) +
	//	" l " + std::to_string((int)maxValue), 0xFFFFFFFF, true);
	if (currValue > maxValue) currValue = maxValue;

	if (isValueFixed > 0) {
		currValue *= (isValueFixed / maxValue);
	}
	if (maxValue == 0) currValue = 0;
}

void Bar::Draw() {
	if (isValueFixed > 0) maxValue = isValueFixed;
	maxValue *= 2;
	currValue *= 2;

	math::TransformMatrix displayMatrix = math::TranslateMatrix{ GetPosition() };
	GetTexture(0)->Draw(displayMatrix, math::ivec2{ 0, 0 }, math::ivec2{ (int)maxValue, GetTexture(0)->GetSize().y });
	
	if (smoothValue < currValue) {
		smoothValue += 15;
		if (smoothValue >= currValue)
			smoothValue = currValue;
	}
	if (smoothValue > currValue) {
		smoothValue -= 15;
		if (smoothValue <= currValue) {
			smoothValue = currValue;
		}
	}

	GetTexture(1)->Draw(displayMatrix, math::ivec2{ 0, 0 }, math::ivec2{ (int)smoothValue, GetTexture(1)->GetSize().y });
	GetTexture(2)->Draw(displayMatrix * math::TranslateMatrix{ math::ivec2{ (int)maxValue - tipPixelSize, 0 } },
		math::ivec2{ GetTexture(2)->GetSize().x - tipPixelSize, 0 }, math::ivec2{ tipPixelSize, GetTexture(2)->GetSize().y });

	//if (IsMouseOn() == true) {
	//	infoFont.Draw(math::TranslateMatrix{ Engine::GetInput().GetMousePosition() - math::ivec2{ -15, 35 } } *math::ScaleMatrix{ math::vec2{ 0.4, 0.4 } });
	//}
}

bool Bar::IsMouseOn()
{
	math::vec2 mouse = Engine::GetInput().GetMousePosition().operator math::vec2();

	return GetPosition().x <= mouse.x && mouse.x <= GetPosition().x + maxValue
		&& GetPosition().y <= mouse.y && mouse.y <= GetPosition().y + GetTexture(0)->GetSize().y;
}