#pragma once
#include "../Engine/Component.h"
#include "../Engine/GameObject.h"
#include <filesystem>

class Quest : public PROJECT::Component {
	friend class Player;
	friend class QuestWindow;
public:
	Quest();
	Quest(const std::filesystem::path& qstFilePath);

	void Update(double) override;
	int ClearExp();
	bool IsQuestDone();
	bool IsAvailable();
	bool IsLoop();

	const std::string& GetNpcName();
	const std::string& GetTargetName();
private:
	void Load(const std::filesystem::path& qstFilePath);

	int goalNumber;
	int currEnemyNumber;

	bool isLoopQuest;

	std::string npcName;
	std::string targetName;
	GameObjectType targetEnemyType;
};