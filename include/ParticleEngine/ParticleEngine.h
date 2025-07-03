#pragma once

#include <atomic>
#include <vector>
#include <unordered_map>
#include <Common/Structs.h>
#include <Camera/CameraInterface.h>
#include <Renderer/GL_Wrappers/VertexBuffer.h>
#include <Renderer/GL_Wrappers/VertexArray.h>
#include <Renderer/GL_Wrappers/Shader/GLShader.h>
#include <ParticleEngine/Emitters/EmitterFactory.h>

class ParticleEngine {
    private: 
        const char* m_VertexShader = R"(
                        #version 330 core
                        layout(location = 0) in vec3 quadVertex;
                        layout(location = 1) in vec4 particlePos;

                        uniform mat4 viewMatrix;
                        uniform mat4 projectionMatrix;
                        uniform vec3 cameraRight;
                        uniform vec3 cameraUp;
                        uniform float size;

                        void main()
                        {
                                vec3 offset = (quadVertex.x * cameraRight + quadVertex.y * cameraUp) * size;
                                vec3 worldPos = particlePos.xyz + offset;
                                gl_Position = projectionMatrix * viewMatrix * vec4(worldPos, 1.0);
                        };
                    )";

        const char* m_FragmentShader = R"(  #version 330 core
                                            out vec4 FragColor;
                                            void main()
                                            {
                                                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // white color for the trajectory
                                            };
                                        )";
        std::atomic<uint64_t> g_EmitterIdCounter = 1;

        uint64_t generateEmitterId() {
            return g_EmitterIdCounter.fetch_add(1, std::memory_order_relaxed);
        }
        std::vector<float> quadVertex = {
            -1.0f,-1.0f,0.0f,
            1.0f,-1.0f,0.0f,
            1.0f, 1.0f,0.0f,
            
            -1.0f,-1.0f,0.0f,
            1.0f, 1.0f,0.0f,
            -1.0f, 1.0f,0.0f
        };

        std::unordered_map<uint64_t, std::unique_ptr<IEmitter>> m_Emitters;
        std::unordered_map<uint64_t, std::vector<glm::vec4>> m_ParticleList;
        ParticleToGUITransfer m_GuiTransfer;
        size_t m_NumberOfPoints = 0;

        GLShader m_Shader;
        VertexArray m_VAO;
        VertexBuffer m_VertexBuffer;
        VertexBuffer m_InstancedBuffer;

        void updateRenderData(std::vector<glm::vec4>& updatedData);
    public:

        //Constructor
        ParticleEngine();
        void stepEngine(float timeDelta, ICameraInfo& camInfo);
        ParticleToGUITransfer getGuiInformation();
        
};