#include "Renderer/Renderer.h"

//Constructor
Renderer::Renderer(){
    m_Proj = glm::perspective(glm::radians(120.0f), (float)2560/ (float)1440, 0.1f, 5000.0f);
    m_View = glm::translate(glm::mat4(1.0f) , glm::vec3(0, 0, 0));

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
    m_VertexBuffer = std::make_unique<VertexBuffer>(m_square.data(), m_square.size() * sizeof(float));  

    VertexBufferLayout layout;
    layout.Push<float>(3); // Position attribute
    m_VAO->AddBuffer(*m_VertexBuffer, layout);
 
}

//Deconstructor
Renderer::~Renderer(){
    
}

void Renderer::render(){
    m_Shader->Bind();
    m_VAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}