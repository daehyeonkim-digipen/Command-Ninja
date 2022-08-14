/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameParticles.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/26/2021
-----------------------------------------------------------------*/
#include "../Engine/ParticleEmitter.h"
#include "GameObjectTypes.h"

class AttackEmitter : public PROJECT::ParticleEmitter {
public:
    AttackEmitter();
private:
    class AttackParticle : public PROJECT::ParticleEmitter::Particle {
    public:
        AttackParticle(std::string spriteFile) : PROJECT::ParticleEmitter::Particle(spriteFile) {}
        GameObjectType GetObjectType() { return GameObjectType::Particle; }
        std::string GetObjectTypeName() override { return "Attack"; }
    };
    static constexpr int NumParticles = 5;
};

class FireEmitter : public PROJECT::ParticleEmitter {
public:
    FireEmitter();
private:
    class FireParticle : public PROJECT::ParticleEmitter::Particle {
    public:
        FireParticle(std::string spriteFile) : PROJECT::ParticleEmitter::Particle(spriteFile) {}
        GameObjectType GetObjectType() { return GameObjectType::Particle; }
        std::string GetObjectTypeName() override { return "fire"; } 
    };
    static constexpr int NumParticles = 3;
};