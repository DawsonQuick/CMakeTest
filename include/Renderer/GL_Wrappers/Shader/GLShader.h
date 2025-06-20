#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <glm/glm.hpp>

#include "./../../../Common/Enums.h"
#include "./../../../Common/Structs.h"
//#include <glm/glm.hpp>

class GLShader {
private:
	std::unordered_map<std::string, int > m_UniformLocationCache;
	std::map<ShaderStage, LoadedShaderStage> m_LoadedShaderStages;
	GLuint m_ProgramId;

	//Utility
	std::string parseFile(const std::string& filePath);
	void exportShader(const std::string& filePath, std::string sourceCode);

	//Vertex Shader
	GLuint compileVertexShader(const char* sourceCode);
	
	//Fragment Shader
	GLuint compileFragmentShader(const char* sourceCode);

	//Compile
	void createProgram();
	void attachShader(GLuint shaderID);
	void removeShader(GLuint shaderID);
	void detachShader(GLuint shaderID);
	void linkProgram();

	LoadedShaderStage* findLoadedShaderStage(ShaderStage stage);

	unsigned int GetUniformedLocation(const std::string& name);
public:
	//Constructor
	template <typename... Args>
	GLShader(Args&&... args) {
		std::vector<InitialShaderStageInfo> shaderStages = { std::forward<Args>(args)... };

		createProgram();
		GLuint bufferId;
		for (InitialShaderStageInfo info : shaderStages) {
			const char* srcCode;

			if(!info.isEmbedded()){
				srcCode = parseFile(info.getShaderFileLocation()).c_str();
			}else{
				srcCode = info.getEmbeddedShader();
			}
			
			switch (info.getShaderStage()) {
			case FRAGMENT:
				bufferId = compileFragmentShader(srcCode);
				attachShader(bufferId);
				m_LoadedShaderStages.emplace(ShaderStage::FRAGMENT,
					LoadedShaderStage(
						info.getShaderFileLocation(),
						srcCode,
						toString(ShaderStage::FRAGMENT),
						bufferId, 
						[this]() {this->reloadShaderStage(ShaderStage::FRAGMENT); }
				));
				break;
			case VERTEX:
				bufferId = compileVertexShader(srcCode);
				attachShader(bufferId);
				m_LoadedShaderStages.emplace( ShaderStage::VERTEX, 
					
					LoadedShaderStage(
					info.getShaderFileLocation(), 
					srcCode,
					toString(ShaderStage::VERTEX),
					bufferId, 
					[this]() { this->reloadShaderStage(ShaderStage::VERTEX); }
				));
				break;
			default:
				break;
			}
		}

		linkProgram();

	}

	//Deconstructor
	~GLShader();


	void Bind();
	void UnBind();

	//Returns a reference to the internally stored information about a shader stage
	std::map<ShaderStage, LoadedShaderStage>& getLoadedShadeInfo();

	void reloadShaderStage(ShaderStage stage);

	void SetUniform1i(const std::string& name, int value) {
		glUniform1i(GetUniformedLocation(name), value);
	}
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
		glUniform4f(GetUniformedLocation(name), v0, v1, v2, v3);
	}
	void SetUniform3f(const std::string& name, float v0, float v1, float v2) {
		glUniform3f(GetUniformedLocation(name), v0, v1, v2);
	}
	void SetUniform1f(const std::string& name, float v0) {
		glUniform1f(GetUniformedLocation(name), v0);
	}
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
		glUniformMatrix4fv(GetUniformedLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}
};