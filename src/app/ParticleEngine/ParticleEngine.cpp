#include <ParticleEngine/ParticleEngine.h>
#include <random>

std::vector<Particle> generateRandomParticles(int numberOfParticles){

    std::vector<Particle> returnList;

    std::random_device rd;                         // Seed
    std::mt19937 gen(rd());                        // Mersenne Twister engine
    std::uniform_real_distribution<> pos(-2.0, 2.0); 
    std::uniform_real_distribution<> vel(-2.0, 2.0); 
    std::uniform_real_distribution<> acc(-2.0, 2.0); 
    std::uniform_int_distribution<> lifetime(0, 25); 

    for(int i = 0; i < numberOfParticles; i++){
        Particle newParticle(glm::vec3(pos(gen),pos(gen),pos(gen)), glm::vec3(vel(gen),vel(gen),vel(gen)),
         glm::vec3(acc(gen),acc(gen),acc(gen)), lifetime(gen));

         returnList.push_back(newParticle);
    }

    return returnList;
}

void buildBillboard(std::vector<glm::vec3>& returnList, glm::vec3& particle, 
                    const glm::vec3& offsetBL, const glm::vec3& offsetBR, const glm::vec3& offsetTR, const glm::vec3& offsetTL){
    returnList.push_back(particle + offsetBL);
    returnList.push_back(particle + offsetBR);
    returnList.push_back(particle + offsetTR);

    returnList.push_back(particle + offsetBL);
    returnList.push_back(particle + offsetTR);
    returnList.push_back(particle + offsetTL);
}

ParticleEngine::ParticleEngine()
{
    m_GuiTransfer = ParticleToGUITransfer(EmitterFactory::getEmitterNames(), [this](std::string name){
         std::cout<<"Creating emitter: " <<name <<std::endl;

         uint64_t newId = generateEmitterId();
         std::vector<glm::vec4> particleList;
         m_Emitters.emplace(newId,EmitterFactory::create(name));
         m_ParticleList.emplace(newId, particleList);
    });

    m_Shader = GLShader(InitialShaderStageInfo(ShaderStage::VERTEX, m_VertexShader), InitialShaderStageInfo(ShaderStage::FRAGMENT, m_FragmentShader));

    m_VAO = VertexArray();

    std::vector<glm::vec3> tmp;
    m_VertexBuffer = VertexBuffer(quadVertex.data(), quadVertex.size() * sizeof(float), BufferType::STATIC);  
    m_InstancedBuffer = VertexBuffer(tmp.data(), tmp.size() * sizeof(glm::vec4), BufferType::DYNAMIC);  

    VertexBufferLayout layout;
    layout.Push<float>(3);
    
    VertexBufferLayout instancedLayout;
    instancedLayout.Push<float>(4);


    m_VAO.AddBuffer(m_VertexBuffer, layout);
    m_VAO.AddMatrixLayout(m_InstancedBuffer, instancedLayout);
}

void ParticleEngine::stepEngine(float timeDelta, ICameraInfo& camInfo){
    //DO STUFF
    //Loop through each emitter, pass in reference vector to update
    //Loop through all Particle vectors and generate buffer for rendering

    glm::vec3 cameraRight = camInfo.m_CameraRight;
    glm::vec3 cameraUp = camInfo.m_CameraUp;

    int totalSize = 0;
    for (auto it = m_ParticleList.begin(); it != m_ParticleList.end(); ) {
        auto& key = it->first;
        auto& particleList = it->second;

            auto emitterIt = m_Emitters.find(key);
            if (emitterIt != m_Emitters.end()) {
                totalSize += particleList.size();
                emitterIt->second->emit(particleList, timeDelta);
            } else {
                std::cout << "Key not found for emitter.\n";
            }

            ++it; // only increment if not erased
        
    }
   
    //If there are no particles to render, return early
    if(totalSize <= 0){
        return;
    }

    float halfSize = 0.5f;
    std::vector<glm::vec4> points;
    glm::vec3 offsetBL = (-cameraRight - cameraUp) * halfSize; // Bottom Left
    glm::vec3 offsetBR = ( cameraRight - cameraUp) * halfSize; // Bottom Right
    glm::vec3 offsetTR = ( cameraRight + cameraUp) * halfSize; // Top Right
    glm::vec3 offsetTL = (-cameraRight + cameraUp) * halfSize; // Top Left
    
    for (auto it = m_ParticleList.begin(); it != m_ParticleList.end(); ) {
        points.insert(points.end(), it->second.begin(), it->second.end());
        it++;
    }
    std::cout<<"Total Size: "<<points.size()<<std::endl;
    updateRenderData(points);

    m_Shader.Bind();
    m_Shader.SetUniformMat4f("viewMatrix", camInfo.view);
    m_Shader.SetUniformMat4f("projectionMatrix", camInfo.projection);
    m_Shader.SetUniform3f("cameraRight",cameraRight.x, cameraRight.y,cameraRight.z);
    m_Shader.SetUniform3f("cameraUp",cameraUp.x,cameraUp.y,cameraUp.z);
    m_Shader.SetUniform1f("size", halfSize);

    m_VAO.Bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, points.size());
    std::vector<glm::vec4>().swap(points);
}

ParticleToGUITransfer ParticleEngine::getGuiInformation(){
    return m_GuiTransfer;
}

void ParticleEngine::updateRenderData(std::vector<glm::vec4>& updatedData) {

    //If there are currently no instances loaded and there are still non to load, return early
    if (m_NumberOfPoints == 0 && updatedData.size() == 0) { return; }

    m_InstancedBuffer.Bind();

    //If # of instances has changed then update variable and buffer
    if (m_NumberOfPoints != updatedData.size()) {
        m_NumberOfPoints = updatedData.size();
        glBufferData(GL_ARRAY_BUFFER, m_NumberOfPoints * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    }
    
    void* ptrInstances = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (ptrInstances) {
        std::memcpy(ptrInstances, updatedData.data(), m_NumberOfPoints * sizeof(glm::vec4));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    else {
        std::cerr << "Failed to map buffer for updating instance data." << std::endl;
    }

}
