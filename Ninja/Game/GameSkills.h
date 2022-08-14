#pragma once
#include <map>
#include <list>
#include <string>

#include "../Engine/Rect.h"
#include "../Engine/Engine.h"

#include "Player.h"

class HitBox;
class Skill {
public:
	Skill(unsigned short cost_, const std::filesystem::path& iconTexturePath);
	Skill(unsigned short cost_) : Skill(cost_, "assets/Skills/icon_none.png") {};

	virtual void Update(double dt) {};
	virtual void Cast(PROJECT::GameObject* caster) {};

	unsigned short GetCost() { return cost; };

	bool IsAvailable() { return isAvailable; }
	bool ShouldUpdate() { return shouldUpdate; }

	void Unlock();
	void DrawIcon(math::TransformMatrix displayMatrix);
	PROJECT::Texture* GetIcon() { return icon; };
	virtual std::string GetInfo() { return "Base Skill Info"; };
protected:
	bool shouldUpdate;
private:
	bool isAvailable;
	unsigned short cost;

	PROJECT::Texture* icon;

};

namespace GameSkill {
	class Slash : public Skill {
	public:
		Slash()
			: Skill(0, "assets/Skills/slash/icon.png") {
		}

		void Cast(PROJECT::GameObject* caster) override;
		std::string GetInfo() override { return "Basic Attack."; }
	};

	class Fire : public Skill {
	public:
		Fire()
			: Skill(70, "assets/Skills/fire/icon.png") {
		}

		void Cast(PROJECT::GameObject* caster) override;
		std::string GetInfo() override { return "Cast a small fire spell : FFA"; }
	};

	class  Dodge : public Skill {
	public:
		Dodge()
			: Skill(50, "assets/Skills/dodge_icon.png"), caster(nullptr) {
		}

		void Update(double dt) override;
		void Cast(PROJECT::GameObject* caster) override;
		std::string GetInfo() override { return "Make caster invulnerable for a moment : DDA"; }
	private:
		PROJECT::GameObject* caster;
		static constexpr __int8 dodgeTime = 100; // ms
		__int8 timer = dodgeTime;
	};

	class Swift : public Skill {
	public:
		Swift()
			: Skill(0, "assets/Skills/swift_icon.png"), caster(nullptr) {
		}

		void Update(double dt) override;
		void Cast(PROJECT::GameObject* caster) override;
		std::string GetInfo() override { return "Give caster an ability to move faster : FASD"; }
	private:
		PROJECT::GameObject* caster;
		static constexpr __int8 staminaPerSecond = 15;
		double timer = 1;
	};

	class SkyWalk : public Skill {
	public:
		SkyWalk()
			: Skill(25, "assets/Skills/skywalk_icon.png") {
		}

		void Cast(PROJECT::GameObject* caster) override;
		std::string GetInfo() override { return "Jump slightly again by comsuming stamina : F"; }
	};

	class Dash : public Skill {
	public:
		Dash()
			: Skill(30, "assets/Skills/dash_icon.png") {
		}

		void Cast(PROJECT::GameObject* caster) override;
		std::string GetInfo() override { return "Going forward with acceleration : A"; }
	};

	class Stump : public Skill {
	public:
		Stump() 
			: Skill(40, "assets/Skills/boss1/stump_icon.png"), caster(nullptr), floatTime(0) {
		}
		
		void Update(double dt) override;
		void Cast(PROJECT::GameObject* caster) override;
		std::string GetInfo() override { return "Savage skill from Oni Boss"; }
	private:
		PROJECT::GameObject* caster;
		double floatTime;
	};
	
	class BrtualDash : public Skill {
	public:
		BrtualDash() 
			: Skill(60, "assets/Skills/boss1/brutalDash_icon.png"), caster(nullptr), timer(0), hitbox(nullptr)
		{}

		void Update(double dt) override;
		void Cast(PROJECT::GameObject* caster) override;
		std::string GetInfo() override { return "Savage skill from Oni Boss"; }
	private:
		PROJECT::GameObject* caster;
		double timer;

		HitBox* hitbox;
	};

	class GroundStrike : public Skill {
	public:
		GroundStrike()
			: Skill(20, "assets/Skills/boss1/groundStrike_icon.png"), caster(nullptr), timer(0)
		{}

		void Update(double dt) override;
		void Cast(PROJECT::GameObject* caster) override;
	private:
		PROJECT::GameObject* caster;
		double timer;
	};
}