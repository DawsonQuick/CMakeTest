#include "Renderer/Renderer.h"

//Constructor
Renderer::Renderer(){
     if (glewInit() != GLEW_OK) {
     std::cerr << "Failed to initialize GLEW" << std::endl;
    }
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);


    m_Shader = std::make_unique<GLShader>(InitialShaderStageInfo(ShaderStage::VERTEX, m_VertexShader), InitialShaderStageInfo(ShaderStage::FRAGMENT, m_FragmentShader));

    m_VAO = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(cubeVertices.data(), cubeVertices.size() * sizeof(float), BufferType::STATIC);  

    VertexBufferLayout layout;
    layout.Push<float>(3); // Position attribute
    m_VAO->AddBuffer(*m_VertexBuffer, layout);
 
}

//Deconstructor
Renderer::~Renderer(){
    
}

void Renderer::render(glm::mat4 view, glm::mat4 proj){
    // m_Shader->Bind();
    // m_Shader->SetUniformMat4f("viewMatrix", view);
    // m_Shader->SetUniformMat4f("projectionMatrix", proj);
    // m_VAO->Bind();
    // glDrawArrays(GL_TRIANGLES, 0, 36);
}