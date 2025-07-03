#pragma once
#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <Common/Structs.h>
#include <Renderer/GL_Wrappers/Shader/GLComputeShader.h>
#include <Renderer/GL_Wrappers/ComputeBuffer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class IEmitter {
public:
    virtual void emit(std::vector<glm::vec4>& particles, float deltaTime) = 0;
    
    virtual const char* getName() const = 0;
    virtual ~IEmitter() = default;

        void setTransformProvider(std::weak_ptr<ITransformProvider> provider) {
        m_transformProvider = std::move(provider);
    }

protected:
    // returns identity if the host disappeared
    glm::mat4 hostTransform() const {
        if (auto p = m_transformProvider.lock()) {
            glm::mat4 rot = glm::toMat4(p->getWorldRotation());
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), p->getWorldPosition());
            return translate * rot;
        }
        return glm::mat4(1.0f);
    }



    int numberOfParticles;

private:
    std::weak_ptr<ITransformProvider> m_transformProvider;

};


class EmitterComputeShader {
public:
    EmitterComputeShader(std::string uniforms, std::string mainFunction){
        replacePlaceholder(baseSource, "==== [UNIFORM PLACEHOLDER] ====", uniforms);
        replacePlaceholder(baseSource, "==== [FUNCTION PLACEHOLDER] ====", mainFunction);

        m_ComputeShader = std::make_unique<GLComputeShader>(baseSource);
        m_PositonBuffer = std::make_unique<ComputeBuffer<glm::vec3>>();
        m_VelocityBuffer = std::make_unique<ComputeBuffer<glm::vec3>>();
        m_AccelerationBuffer = std::make_unique<ComputeBuffer<glm::vec3>>();
    }

    ~EmitterComputeShader(){
        cleanup();
    }

    void bindBase(){
        m_PositonBuffer->updateSSBOData(m_Position, true);
        m_VelocityBuffer->updateSSBOData(m_Velocity,true);
        m_AccelerationBuffer->updateSSBOData(m_Acceleration,true);
        //m_LifeTimeBuffer->updateSSBOData(m_LifeTime, true);

        m_PositonBuffer->bind(0);
        m_VelocityBuffer->bind(1);
        m_AccelerationBuffer->bind(2);
        //m_LifeTimeBuffer->bind(3);
        m_ComputeShader->bind();
    }

    void run(){
        glDispatchCompute(256, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
    }

    void extractData(){
        m_Position = m_PositonBuffer->retreiveData();
        m_Velocity = m_VelocityBuffer->retreiveData();
        m_Acceleration = m_AccelerationBuffer->retreiveData();
        //m_LifeTime = m_LifeTimeBuffer->retreiveData();
    }
    
    void cleanup(){
        m_PositonBuffer->cleanup();
        m_VelocityBuffer->cleanup();
        m_AccelerationBuffer->cleanup();
        m_ComputeShader->cleanup();
    }

    std::vector<glm::vec3>& getPosition(){
        return m_Position;
    }

    std::vector<glm::vec3>& getVelocity(){
        return m_Velocity;
    }

    std::vector<glm::vec3>& getAcceleration(){
        return m_Acceleration;
    }

    // std::vector<float>& getLifeTime(){
    //     return m_LifeTime;
    // }

protected:
    std::vector<glm::vec3> m_Position;
    std::vector<glm::vec3> m_Velocity;
    std::vector<glm::vec3> m_Acceleration;
    //std::vector<float> m_LifeTime;
private:

    std::unique_ptr<GLComputeShader> m_ComputeShader;
    std::unique_ptr<ComputeBuffer<glm::vec3>> m_PositonBuffer;
    std::unique_ptr<ComputeBuffer<glm::vec3>> m_VelocityBuffer;
    std::unique_ptr<ComputeBuffer<glm::vec3>> m_AccelerationBuffer;
    //std::unique_ptr<ComputeBuffer<float>> m_LifeTimeBuffer;

    void replacePlaceholder(std::string& source, const std::string& placeholder, const std::string& replacement) {
        size_t pos = 0;
        while ((pos = source.find(placeholder, pos)) != std::string::npos) {
            source.replace(pos, placeholder.length(), replacement);
            pos += replacement.length(); // Move past the replacement
        }
    }

    std::string baseSource = R"( 
                                #version 430

                                layout(local_size_x = 256) in;

                                layout(std430, binding = 0) buffer Positions     { vec3 positions[]; };
                                layout(std430, binding = 1) buffer Velocities    { vec3 velocities[]; };
                                layout(std430, binding = 2) buffer Accelerations { vec3 accelerations[]; };

                                // ==== [UNIFORM PLACEHOLDER] ====
                                // ==== [FUNCTION PLACEHOLDER] ====

                                void main() {
                                    uint id = gl_GlobalInvocationID.x;
                                    updateParticle(id);
                                }
                                )";
};