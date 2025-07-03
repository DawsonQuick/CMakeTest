#pragma once
#include <Common/GLInclude.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class GLComputeShader {
private:
    GLuint m_ShaderProgram;
    std::unordered_map<std::string, int > m_UniformLocationCache;

    void compileAndAttachShader(const std::string& shaderCode) {
        GLuint shader = glCreateShader(GL_COMPUTE_SHADER);

        const char* shaderCodeCStr = shaderCode.c_str();
        glShaderSource(shader, 1, &shaderCodeCStr, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        glAttachShader(m_ShaderProgram, shader);
        glLinkProgram(m_ShaderProgram);

        glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(shader);
    }

    unsigned int getUniformedLocation(const std::string& name) {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
            return m_UniformLocationCache[name];
        }
        int location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        if (location == -1) {
            std::cout << "Warning: uniform '" << name << "'doesn't exist!" << std::endl;
        }

        m_UniformLocationCache[name] = location;

        return location;
    }

public:

    GLComputeShader(const std::string& shaderCode){
        m_ShaderProgram = glCreateProgram();
        compileAndAttachShader(shaderCode);
    }

    void bind(){
        glUseProgram(m_ShaderProgram);
    }

    void unbind(){
        glUseProgram(0);
    }

    void SetUniform1i(const std::string& name, int value) {
		glUniform1i(getUniformedLocation(name), value);
	}
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
		glUniform4f(getUniformedLocation(name), v0, v1, v2, v3);
	}
	void SetUniform3f(const std::string& name, float v0, float v1, float v2) {
		glUniform3f(getUniformedLocation(name), v0, v1, v2);
	}
	void SetUniform1f(const std::string& name, float v0) {
		glUniform1f(getUniformedLocation(name), v0);
	}
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
		glUniformMatrix4fv(getUniformedLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

    void cleanup(){
         glDeleteProgram(m_ShaderProgram);
    }
};