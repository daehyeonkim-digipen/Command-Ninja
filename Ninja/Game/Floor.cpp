/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Floor.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/19/2021
-----------------------------------------------------------------*/

#include "..\Engine\Collision.h"
#include "..\Engine\Rect.h"
#include "GameObjectTypes.h"
#include "Floor.h"
#include "Enemy.h"

Floor::Floor(math::irect2 position, std::map<int, PROJECT::Texture*> type) : GameObject(static_cast<math::vec2>(position.point1)), tiles(type) {
	AddGOComponent(new PROJECT::RectCollision(math::irect2{ math::ivec2{ 0,0 }, math::ivec2{ position.point2 - position.point1 } }, this));
}

bool Floor::CanCollideWith(GameObjectType)
{
	//switch(objectBType)
	//{
	//// case GameObjectType::Player:
	//default:
	//	return false;
	//}
	return false;
}

void Floor::Draw(math::TransformMatrix displayMatrix)
{
	constexpr int blockSize = 32;
	const math::vec2 initPos = GetPosition();
	math::vec2 Pos = initPos;
	const math::vec2 aimPos = GetGOComponent<PROJECT::RectCollision>()->GetWorldCoorRect().point2;
	tiles[4]->Draw(displayMatrix * GetMatrix());
	while(Pos != aimPos)
	{
		
		if(Pos.x != aimPos.x - blockSize)
		{
			Pos.x += blockSize;
		}
		else
		{
			Pos.x = initPos.x;
			Pos.y += blockSize;
			if(Pos.y == aimPos.y)
			{
				break;
			}
		}
		if(Pos.y == aimPos.y - blockSize)
		{
			
			if (Pos.x == initPos.x)
			{
				tiles[7]->Draw(displayMatrix * math::TranslateMatrix(Pos));
			}
			else if(Pos.x == aimPos.x - blockSize)
			{
				tiles[9]->Draw(displayMatrix * math::TranslateMatrix(Pos));
			}
			else
			{
				tiles[8]->Draw(displayMatrix * math::TranslateMatrix(Pos));
			}
		}
		else if(Pos.x == initPos.x)
		{
			tiles[4]->Draw(displayMatrix  * math::TranslateMatrix(Pos));
		}
		else if(Pos.x == aimPos.x - blockSize)
		{
			tiles[6]->Draw(displayMatrix  * math::TranslateMatrix(Pos));
		}
		else
		{
			tiles[5]->Draw(displayMatrix  * math::TranslateMatrix(Pos));
		}
	}
	GameObject::Draw(displayMatrix);
}

