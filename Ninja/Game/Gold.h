#pragma once
#include "../Engine/Component.h"

class Gold : public PROJECT::Component {
public:
	Gold() : gold(0) {};

	void Add(int cost) {
		gold += cost;
	};
	int GetValue() { return gold; }
private:
	int gold;
};

