/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Particle.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/26/2021
-----------------------------------------------------------------*/

#pragma once
#include <vector>
#include "..\Engine\Vec2.h"    //Vector2D
#include "..\Engine\GameObject.h"
#include "..\Engine\Component.h"

namespace PROJECT {
    class ParticleEmitter : public Component {
    protected:
        class Particle : public GameObject {
        public:
            Particle(std::string spriteFile);
            void Revive(math::vec2 position, math::vec2 velocity, double life, math::vec2 DEBUG_NEWScale);
            void Update(double dt) override;
            void Draw(math::TransformMatrix matrix) override;
            bool IsAlive() { return life > 0; }
        private:
        	double life;
        };
    public:
        ParticleEmitter(double lifetime);
        virtual ~ParticleEmitter();
        void AddParticle(Particle* particleData);
        void Emit(int number, math::vec2 position, math::vec2 emitterVelocity, math::vec2 emitVector, double spread, math::vec2 scale, double rotate);
    private:

        std::vector<Particle*> particleMemoryPool;
        int particleIndexToUse;
        double lifetime;
    };
}