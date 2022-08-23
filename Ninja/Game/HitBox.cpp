#include "../Engine/UIManager.h"
#include "../Engine/Collision.h"
#include "../Engine/Camera.h"
#include "../Engine/Sprite.h"

#include "PopUpDamage.h"
#include "HitBox.h"

HitBox::HitBox(math::vec2 pos, math::ivec2 size, GameObject* caster_, int damage_)
	: GameObject(pos), caster(caster_), damage(damage_)
{	
	if (caster->GetScale() == math::vec2{ -1, 1 }) 
		size.x *= -1;
	AddGOComponent(new PROJECT::RectCollision({ { 0, 0 }, size }, this));
	attributes.Hp = 30;
}

void HitBox::Update(double dt) {
	GameObject::Update(dt);
	if (attributes.Hp <= 0) {
		Destroy();
	}
	else {
		attributes.Hp--; // ms
	}
}

bool HitBox::CanCollideWith(GameObjectType objectType) {
	return false;
}

void HitBox::ResolveCollision(GameObject* objectA) {
	if (caster == objectA)
		return;

	//std::vector<GameObject*>::iterator iter = std::find(targets.begin(), targets.end(), objectA);
	//if (iter == targets.end()) {
	//	objectA->attributes.Hp -= damage;
	//	PROJECT::UIManager* uim = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::UIManager>();
	//	math::vec2 cameraPosition = Engine::GetGameStateManager().GetGSMComponents().GetComponent<PROJECT::Camera>()->GetPosition();
	//	uim->AddUI(new PopUpDamage(objectA->GetPosition() - cameraPosition + math::vec2{ 0, 80 }, damage));
	//	targets.push_back(objectA);
	//}
}

void HitBox::SetLifeTime(int value_) {
	attributes.Hp = attributes.maxHp = value_;
}