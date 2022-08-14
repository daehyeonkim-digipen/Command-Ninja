#include "UIManager.h"

PROJECT::UIManager::~UIManager() {
	for (UI* ui : uis) {
		destroyList.push_back(ui);
	}
	DestroyUI();
}

void PROJECT::UIManager::AddUI(UI* ui) {
	uis.push_back(ui);
}

void PROJECT::UIManager::Update(double dt) {
	destroyList.clear();
	uis.sort([](UI* ui1, UI* ui2) { return ui1->GetLevel() < ui2->GetLevel(); });
	for (UI* ui : uis) {
		//if (ui->IsVisible() == true)
			ui->Update(dt);
		if (ui->shouldDestroyed == true) {
			destroyList.push_back(ui);
		}
	} 

	DestroyUI();
}

void PROJECT::UIManager::DestroyUI() {
	for (UI* object : destroyList) {
		uis.remove(object);
		delete object;
	}
	destroyList.clear();
}

void PROJECT::UIManager::Draw() {
	for (UI* ui : uis) {
		if (ui->IsVisible() == true)
			ui->Draw();
	}
}
