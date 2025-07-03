#include <ParticleEngine/Emitters/BasicEmitter/BasicEmitter.h>
#include <random>

BasicEmitter::BasicEmitter()
{
    m_ComputeShader = std::make_unique<EmitterComputeShader>("",basicEmitterComputeCode);
    m_MaxParticleCount = 1000;
    generateRandomParticles(m_MaxParticleCount);
}

void BasicEmitter::emit(std::vector<glm::vec4>& particles, float deltaTime){

    if(m_Position.size() < m_MaxParticleCount){
        size_t delta = m_MaxParticleCount - m_Position.size();
        generateRandomParticles(delta);
    }

    const __m128 dt = _mm_set1_ps(deltaTime);

    std::vector<size_t> expiredIndices;
    for(size_t i=0; i<m_Position.size(); i++){

        m_LifeTime[i] -= deltaTime;

        if (m_LifeTime[i] <= 0.0f) {
            expiredIndices.push_back(i);
            continue;
        }

        __m128 vel = _mm_loadu_ps(glm::value_ptr(m_Velocity[i]));
        __m128 acc = _mm_loadu_ps(glm::value_ptr(m_Acceleration[i]));

        // vel += acc * dt
        vel = _mm_add_ps(vel, _mm_mul_ps(acc, dt));
        _mm_storeu_ps(glm::value_ptr(m_Velocity[i]), vel);

        __m128 pos = _mm_loadu_ps(glm::value_ptr(m_Position[i]));

        // pos += vel * dt
        pos = _mm_add_ps(pos, _mm_mul_ps(vel, dt));
        _mm_storeu_ps(glm::value_ptr(m_Position[i]), pos);
    }

    for (auto it = expiredIndices.rbegin(); it != expiredIndices.rend(); ++it) {
        size_t index = *it;
        m_Position.erase(m_Position.begin() + index);
        m_Velocity.erase(m_Velocity.begin() + index);
        m_Acceleration.erase(m_Acceleration.begin() + index);
        m_LifeTime.erase(m_LifeTime.begin() + index);
    }

    particles = m_Position;
}

void BasicEmitter::update(Particle& particle , float deltaTime){
     particle.getLifeTime() = particle.getLifeTime() - deltaTime;
     particle.getPosition() = particle.getPosition() + (particle.getVelocity() * deltaTime);
     particle.getVelocity() = particle.getVelocity() + (particle.getAcceleration() * deltaTime);
}

void BasicEmitter::generateRandomParticles(int numberOfParticles){
    std::random_device rd;                         // Seed
    std::mt19937 gen(rd());                        // Mersenne Twister engine
    std::normal_distribution<> pos(0.0, 0.1); 
    std::normal_distribution<> velx(2.0, 8.0);
    std::normal_distribution<> vely(2.0, 8.0); 
    std::normal_distribution<> velz(2.0, 8.0); 
    std::normal_distribution<> accx(-2.0, 0.1); 
    std::normal_distribution<> accy(-2.0, 0.1); 
    std::normal_distribution<> accz(-2.0, 0.1); 
    std::uniform_int_distribution<> lifetime(0, 25); 

    float radius = 5;
    for(int i = 0; i < numberOfParticles; i++){

        m_Position.push_back(glm::vec4(pos(gen),pos(gen),pos(gen),0.0));
        m_Velocity.push_back(glm::vec4(velx(gen),vely(gen),velz(gen),0.0));
        m_Acceleration.push_back(glm::vec4(accx(gen),accy(gen),accz(gen),0.0));
        m_LifeTime.push_back(lifetime(gen));

            // float angle = glm::linearRand(0.0f, glm::two_pi<float>());
            // glm::vec3 position = glm::vec3(radius * cos(angle), 0.0f, radius * sin(angle));
            // glm::vec3 toCenter = -glm::normalize(position);
            // glm::vec3 tangent = glm::normalize(glm::cross(toCenter, glm::vec3(0, 1, 0)));

            // float G = 10.0f;
            // float speed = std::sqrt(G / radius);
            // glm::vec3 velocity = tangent * speed;
            // glm::vec3 acceleration = toCenter * (G / (radius * radius));

            // m_Position.push_back(glm::vec4(position.x, position.y, position.z, 0.0));
            // m_Velocity.push_back(glm::vec4(velocity.x, velocity.y, velocity.z, 0.0));
            // m_Acceleration.push_back(glm::vec4(acceleration.x, acceleration.y, acceleration.z, 0.0));
            // m_LifeTime.push_back(lifetime(gen));
    }

}