#pragma once

#include <ParticleEngine/Emitters/EmitterInterface.h>
#include <ParticleEngine/Emitters/EmitterRegistrationMacro.h>
#include <immintrin.h> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

class BasicEmitter: public IEmitter {
    private:
        std::unique_ptr<EmitterComputeShader> m_ComputeShader;
    
        size_t m_MaxParticleCount;
        std::vector<glm::vec4> m_Position;
        std::vector<glm::vec4> m_Velocity;
        std::vector<glm::vec4> m_Acceleration;
        std::vector<float> m_LifeTime;
        
        void update(Particle& particle, float deltaTime);
        void generateRandomParticles(int numberOfParticles);
        std::string basicEmitterComputeCode = R"(
            void updateParticle(uint id) {
                positions[id].x = positions[id].x + (velocities[id].x * 0.1f);
                positions[id].y = positions[id].y + (velocities[id].y * 0.1f);
                positions[id].z = positions[id].z + (velocities[id].z * 0.1f);
                velocities[id].x = velocities[id].x + (accelerations[id].x * 0.1f);
                velocities[id].y = velocities[id].y + (accelerations[id].y * 0.1f);
                velocities[id].z = velocities[id].z + (accelerations[id].z * 0.1f);
            }
        )";
        
    public:
        BasicEmitter();
        void emit(std::vector<glm::vec4>& particles, float deltaTime) override;
        const char* getName() const override { return "BasicEmitter"; }
};
REGISTER_EMITTER(BasicEmitter);