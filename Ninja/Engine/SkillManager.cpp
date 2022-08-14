#include "../Game/GameSkills.h"

#include "SkillManager.h"
#include "Engine.h"

SkillManager::SkillManager() {
	skills["Basic Attack"] = DEBUG_NEW GameSkill::Slash();

	skills["FFA"] = DEBUG_NEW GameSkill::Fire();
	skills["DDA"] = DEBUG_NEW GameSkill::Dodge();
	skills["FASD"] = DEBUG_NEW GameSkill::Swift();
	skills["F"] = DEBUG_NEW GameSkill::SkyWalk();
	skills["A"] = DEBUG_NEW GameSkill::Dash();

	skills["Boss1 Strike"] = DEBUG_NEW GameSkill::GroundStrike();
	skills["Boss1 Stump"] = DEBUG_NEW GameSkill::Stump();
	skills["Boss1 Dash"] = DEBUG_NEW GameSkill::BrtualDash();

	Categorize();
}

void SkillManager::Categorize() {
	for (int i = 0; i < 3; i++) {
		cSkillsInfo.push_back(std::map<std::string, bool>());
	}

	auto insertInfo = [&](__int8 index, std::string cmd) {
		if (skills.find(cmd) == skills.end()) {
			throw std::runtime_error("Undefined Skill Command : " + cmd);
		}

		cSkillsInfo[index].insert(std::pair<std::string, bool>(cmd, skills[cmd]->IsAvailable()));
	};

	enum Category { Attack, Dexterity, Defense };

	insertInfo(Attack, "FFA");
	insertInfo(Dexterity, "FASD");
	insertInfo(Dexterity, "F");
	insertInfo(Dexterity, "A");
	insertInfo(Defense, "DDA");
}

SkillManager::~SkillManager() {
	for (std::pair<const std::string, Skill*>& pair : skills) {
		delete pair.second;
	}
	skills.clear();
}

void SkillManager::Unlock(const std::string& command) {
	if (skills.find(command) == skills.end()) {
		Engine::GetLogger().LogEvent("No skill matches with command : " + command);
		return;
	}

	skills.find(command)->second->Unlock();
	Engine::GetLogger().LogEvent("Skill Unlocked : " + command);
}

void SkillManager::RandomUnlock() {
	std::map<const std::string, Skill*>::iterator iter = skills.begin();

	int length = rand() % static_cast<int>(skills.size());
	for (int i = 0; i < length; i++)
		iter++;

	Unlock(iter->first);
}

void SkillManager::UnlockAll() {
	for (std::pair<const std::string, Skill*>& pair : skills) {
		pair.second->Unlock();
		Engine::GetLogger().LogEvent("Skill Unlocked : " + pair.first);
	}
}

PROJECT::Texture* SkillManager::GetIcon(const std::string& command) {
	std::map<const std::string, Skill*>::iterator searchedIter = skills.find(command);
	if (searchedIter == skills.end()) {
		return nullptr;
	}

	return searchedIter->second->GetIcon();
}

std::string SkillManager::GetSkillInfo(const std::string& command) {
	std::map<const std::string, Skill*>::iterator searchedIter = skills.find(command);
	if (searchedIter == skills.end()) {
		return "empty info";
	}

	return searchedIter->second->GetInfo();
}

unsigned short SkillManager::GetCost(const std::string& command) {
	std::map<const std::string, Skill*>::iterator searchedIter = skills.find(command);
	if (searchedIter == skills.end()) {
		return 0;
	}

	return searchedIter->second->GetCost();
}

void SkillManager::Update(double dt) {
	for (std::pair<const std::string, Skill*>& pair : skills) {
		if (pair.second->ShouldUpdate() == true)
			pair.second->Update(dt);
	}

	for (std::map<std::string, bool>& map : cSkillsInfo) {
		for (std::pair<const std::string, bool>& pair : map) {
			pair.second = skills[pair.first]->IsAvailable();
		}
	}
}

void SkillManager::Cast(const std::string& command, PROJECT::GameObject* caster) {
	if (command.empty() == true)
		return;

	std::map<const std::string, Skill*>::iterator searchedIter = skills.find(command);
	if (searchedIter == skills.end()) {
		Engine::GetLogger().LogEvent("No skill matches with command : " + command);
		return;
	}

	Skill* searchedSkill = searchedIter->second;
	if (caster->attributes.Stamina < searchedSkill->GetCost()) {
		Engine::GetLogger().LogEvent("Stamina Insufficient");
		return;
	}

	if (searchedSkill->IsAvailable() == true) {
		searchedSkill->Cast(caster);
		caster->attributes.Stamina -= searchedSkill->GetCost();
	}
	else {
		Engine::GetLogger().LogEvent("Skill not learned : " + command);
	}
}
