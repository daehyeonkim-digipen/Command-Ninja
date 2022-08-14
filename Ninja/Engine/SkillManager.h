#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include <list>
#include <map>

namespace PROJECT {
	class GameObject;
}

class Skill;
class SkillManager : public PROJECT::Component {
	friend class SkillWindow;
public:
	SkillManager();
	~SkillManager();

	void Update(double dt) override;
	void Cast(const std::string& command, PROJECT::GameObject* caster);
	void Unlock(const std::string& command);
	void RandomUnlock();
	void UnlockAll();

	PROJECT::Texture* GetIcon(const std::string& command);
	std::string GetSkillInfo(const std::string& command);
	unsigned short GetCost(const std::string& command);

private:
	void Categorize();
	std::map<std::string, Skill*> skills;
	std::list<Skill*> castingSkills;
	std::vector<std::map<std::string, bool>> cSkillsInfo;

	static constexpr int SkillIconSize = 64;
};
