/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Particle.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/26/2021
-----------------------------------------------------------------*/

#include "Engine.h"
#include "GameObjectManager.h"
#include "ParticleEmitter.h"
#include "Sprite.h"

PROJECT::ParticleEmitter::ParticleEmitter(double lifetime) : particleIndexToUse(0), lifetime(lifetime) {}

PROJECT::ParticleEmitter::~ParticleEmitter() { particleMemoryPool.clear(); }


void PROJECT::ParticleEmitter::AddParticle(ParticleEmitter::Particle* particleData) {
    particleMemoryPool.push_back(particleData);
    Engine::GetGameStateManager().GetGSMComponents().GetComponent<GameObjectManager>()->Add(particleData);
}

void PROJECT::ParticleEmitter::Emit(int toEmit, math::vec2 emitterPosition, math::vec2 emitterVelocity, math::vec2 emitVector, double spread, math::vec2 scale, double rotate) {
    for (int i = 0; i < toEmit; i++) {
        if (particleMemoryPool[particleIndexToUse]->IsAlive() == true) {
            Engine::GetLogger().LogError("Particle is being overwritten");
        }

        double angleVariation = spread != 0 ? ((rand() % static_cast<int>(spread * 1024)) / 1024.0f) - spread / 2 : 0;
        double C26451 = ((static_cast<double>(rand() % 1024) / 2048.0f) + 0.5f);
        math::vec2 particleVelocity = math::RotateMatrix(rotate + angleVariation) * emitVector * C26451 + emitterVelocity;
        particleMemoryPool[particleIndexToUse++]->Revive(emitterPosition, particleVelocity, lifetime, scale);
        if (particleIndexToUse >= static_cast<int>(particleMemoryPool.size())) {
            particleIndexToUse = 0;
        }
    }
}

PROJECT::ParticleEmitter::Particle::Particle(std::string spriteFile) : GameObject({ 0,0 }), life(0) {
    AddGOComponent(new Sprite(spriteFile, this));
}

void PROJECT::ParticleEmitter::Particle::Revive(math::vec2 newPosition, math::vec2 newVelocity, double newLife, math::vec2 newScale) {
    life = newLife;
    SetPosition(newPosition);
    SetVelocity(newVelocity);
    SetScale(newScale);
    GetGOComponent<Sprite>()->PlayAnimation(0);
}

void PROJECT::ParticleEmitter::Particle::Update(double dt) {
    if (IsAlive() == true) {
        life -= dt;
        GameObject::Update(dt);
    }
    else
    {
        SetScale({0, 0});
    }
	
}

void PROJECT::ParticleEmitter::Particle::Draw(math::TransformMatrix matrix) {
    if (IsAlive() == true) {
        GameObject::Draw(matrix);
    }
}