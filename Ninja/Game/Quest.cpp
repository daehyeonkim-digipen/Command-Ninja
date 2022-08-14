#include <fstream>
#include "Quest.h"
#include "GameObjectTypes.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"

Quest::Quest(const std::filesystem::path& qstFilePath)
	: currEnemyNumber(0), goalNumber(-1)
{
	Load(qstFilePath);
}

Quest::Quest()
	: currEnemyNumber(0), goalNumber(-1), targetEnemyType(GameObjectType::Count), isLoopQuest(false)
{
	npcName = targetName = "";
}

void Quest::Load(const std::filesystem::path& qstFilePath) {
	if (qstFilePath.extension() != ".qst") throw std::runtime_error("Expected .qst extension but taken : " + qstFilePath.extension().generic_string());
	std::ifstream inFile{qstFilePath};
	if (inFile.is_open() == false) throw std::runtime_error("File does not opened : " + qstFilePath.generic_string());

	inFile >> npcName;
	while (npcName.find("_") != std::string::npos) {
		npcName.replace(npcName.find("_"), 1, " ");
	}

	std::string text;
	inFile >> text;

	if (text == "OniWisp") {
		targetEnemyType = GameObjectType::OniWisp;
		targetName = "Oni Wisp";
	}
	else if (text == "DullOni") { 
		targetEnemyType = GameObjectType::DullOni;
		targetName = "Dull Oni";
	}
	else if (text == "AngryOni") {
		targetEnemyType = GameObjectType::AngryOni;
		targetName = "Angry Oni";
	}
	else if (text == "OniBoss") {
		targetEnemyType = GameObjectType::OniBoss;
		targetName = "Oni Boss";
	}
	else {
		throw std::runtime_error("undefined enemy type name : " + text);
	}

	inFile >> goalNumber;

	inFile >> std::boolalpha >> isLoopQuest;
}

void Quest::Update(double) {
	const std::list currStateObjects = Engine::GetGameStateManager().GetCurrState()->GetGSComponent<PROJECT::GameObjectManager>()->GetGameObjects();
	for (PROJECT::GameObject* object : currStateObjects) {
		if (object->IsDead() == true) {
			if (object->GetObjectType() == targetEnemyType) {
				currEnemyNumber++;
			}
			object->SetCount();
		}
	}

	if (currEnemyNumber > goalNumber) {
		currEnemyNumber = goalNumber;
	}
}

bool Quest::IsQuestDone() {
	return goalNumber > 0 && currEnemyNumber >= goalNumber;
}

bool Quest::IsAvailable() { 
	return goalNumber > 0; 
}
bool Quest::IsLoop() {
	return isLoopQuest;
}

const std::string& Quest::GetNpcName() {
	return npcName;
}

const std::string& Quest::GetTargetName() {
	return targetName;
}