/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameParticles.cpp
Project: PROJECT
Project: PROJECT
Author: Kevin Wright
Creation date: 2/26/2021
-----------------------------------------------------------------*/

#include "GameParticles.h"
#include "..\Engine\Engine.h"

AttackEmitter::AttackEmitter() : PROJECT::ParticleEmitter(1) {
	for (int i = 0; i < AttackEmitter::NumParticles; i++) {
		AddParticle(new AttackParticle("assets/Skills/slash/spriteInfo.spt"));
	}
}

FireEmitter::FireEmitter() : PROJECT::ParticleEmitter(0.45) {
	for (int i = 0; i < FireEmitter::NumParticles; i++) {
		AddParticle(new FireParticle("assets/Skills/fire/spriteInfo.spt"));
	}
}