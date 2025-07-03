#include <Common/GLInclude.h>
#include "Common/Enums.h"
#include "Common/Structs.h"
#include "GL_Wrappers/Shader/GLShader.h"
#include <memory>
#include "GL_Wrappers/VertexArray.h"
#include "GL_Wrappers/VertexBuffer.h"
#include "GL_Wrappers/VertexBufferLayout.h"
#include <glm/gtc/matrix_transform.hpp>
class Renderer {
public:

    //Constructor
    Renderer();

    
    void render(glm::mat4 view, glm::mat4 proj);

    //Deconstructor
    ~Renderer();

private:
    std::unique_ptr<GLShader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;

    float size = 10;
    std::vector<float> cubeVertices = {
    // Front face
    -size, -size,  size,  // Bottom-left
     size, -size,  size,  // Bottom-right
     size,  size,  size,  // Top-right

    -size, -size,  size,  // Bottom-left
     size,  size,  size,  // Top-right
    -size,  size,  size,  // Top-left

    // Back face
    -size, -size, -size,
    -size,  size, -size,
     size,  size, -size,

    -size, -size, -size,
     size,  size, -size,
     size, -size, -size,

    // Left face
    -size, -size, -size,
    -size, -size,  size,
    -size,  size,  size,

    -size, -size, -size,
    -size,  size,  size,
    -size,  size, -size,

    // Right face
     size, -size, -size,
     size,  size, -size,
     size,  size,  size,

     size, -size, -size,
     size,  size,  size,
     size, -size,  size,

    // Top face
    -size,  size, -size,
    -size,  size,  size,
     size,  size,  size,

    -size,  size, -size,
     size,  size,  size,
     size,  size, -size,

    // Bottom face
    -size, -size, -size,
     size, -size, -size,
     size, -size,  size,

    -size, -size, -size,
     size, -size,  size,
    -size, -size,  size,
};

    const char* m_VertexShader = R"(#version 330 core
                                    layout(location = 0) in vec3 aPos;      
                                    uniform mat4 viewMatrix;
                                    uniform mat4 projectionMatrix;
                                    void main()
                                    {
                                        gl_Position = projectionMatrix * viewMatrix * vec4(aPos, 1.0); // Project into clip space
                                    };
                                )";

    const char* m_FragmentShader = R"(  #version 330 core
                                        out vec4 FragColor;
                                        void main()
                                        {
                                            FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0); // white color for the trajectory
                                        };
                                    )";
    
};
