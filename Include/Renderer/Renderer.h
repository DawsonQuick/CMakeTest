#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Common/Enums.h"
#include "Common/Structs.h"
#include "GL_Wrappers/Shader/GLShader.h"
#include "GL_Wrappers/VertexArray.h"
#include "GL_Wrappers/VertexBuffer.h"
#include "GL_Wrappers/VertexBufferLayout.h"

class Renderer {
public:

    //Constructor
    Renderer();

    
    void render();

    //Deconstructor
    ~Renderer();

private:
	glm::mat4 m_Proj;
	glm::mat4 m_View;

    std::unique_ptr<GLShader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;

    std::vector<float> m_square = {
        0.0, 0.0, 0.0,
        0.5, 0.0, 0.0,
        0.5, 0.5, 0.0,

        0.5, 0.5, 0.0,
        0.0, 0.5, 0.0,
        0.0, 0.0, 0.0
    };

    const char* m_VertexShader = R"(#version 450 core
                                    layout(location = 0) in vec3 aPos;      
                                    uniform mat4 u_MVP;
                                    void main()
                                    {
                                        gl_Position = vec4(aPos, 1.0); // Project into clip space
                                    };
                                )";

    const char* m_FragmentShader = R"(  #version 450 core
                                        out vec4 FragColor;
                                        void main()
                                        {
                                            FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0); // white color for the trajectory
                                        };
                                    )";
    
};