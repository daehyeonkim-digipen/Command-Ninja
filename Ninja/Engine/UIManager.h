#pragma once

#include "Engine.h"
#include "GameObject.h"
#include "../Game/UI.h"

#include "Vec2.h"

namespace PROJECT {
	class UIManager : public Component {
	public:
		UIManager() {};
		~UIManager();

		void AddUI(UI* UItype);
		void Update(double dt) override;
		void Draw(); 

		template <typename T>
		T* GetUI(PROJECT::GameObject* object) {
			for (UI* ui : uis) {
				if (ui->object == object) {
					T* find = dynamic_cast<T*>(ui);
					if (find != nullptr) {
						return find;
					}
				}
			}
			return nullptr;
		}
		 
		void RemoveUI(PROJECT::GameObject* object) {
			for (UI* ui : uis) {
				if (ui->object == object) {
					ui->UIDestroy();
				}
			}
		}

	private:
		void DestroyUI();

		std::list<UI*> uis;
		std::vector<UI*> destroyList;
	};
}

