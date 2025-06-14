#include "Renderer/GL_Wrappers/Shader/GLShader.h"


GLShader::~GLShader() {

}

void GLShader::Bind() {
	glUseProgram(m_ProgramId);
}
void GLShader::UnBind() {
	glUseProgram(0);
}



//------------------------------------------------------------------------------------------------------------



//Utility
std::string GLShader::parseFile(const std::string& filePath) {
	std::ifstream stream(filePath);
	std::stringstream outbound;

	std::string line;
	while (getline(stream, line)) {
		outbound << line << '\n';
	}

	return outbound.str();
}

void GLShader::exportShader(const std::string& filePath, std::string sourceCode) {
	std::ofstream stream(filePath);
	if (stream.is_open()) {
		stream << sourceCode;
		stream.close();
	}
	else {
		std::cerr << "Failed to open shader file" << std::endl;
	}
}


//Vertex Shader
GLuint GLShader::compileVertexShader(const char* sourceCode) {

	GLint success;
	GLchar infoLog[1024];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &sourceCode, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return vertexShader;

}

//Fragment Shader
GLuint GLShader::compileFragmentShader(const char* sourceCode) {

	GLint success;
	GLchar infoLog[1024];

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &sourceCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return fragmentShader;
}


//Compile
void GLShader::createProgram() {
	m_ProgramId = glCreateProgram();
}

void GLShader::attachShader(GLuint shaderID) {
	GLenum error;

	glAttachShader(m_ProgramId, shaderID);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "OpenGL error during Shader attachment: " << error << std::endl;
	}
}
void GLShader::removeShader(GLuint shaderID) {
	glDeleteShader(shaderID);
}
void GLShader::detachShader(GLuint shaderID) {
	glDetachShader(m_ProgramId, shaderID);
}

void GLShader::linkProgram() {
	glLinkProgram(m_ProgramId);
}


std::map<ShaderStage, LoadedShaderStage>& GLShader::getLoadedShadeInfo() {
	return m_LoadedShaderStages;
}

LoadedShaderStage* GLShader::findLoadedShaderStage(ShaderStage stage) {
	auto it = m_LoadedShaderStages.find(stage);
	if (it != m_LoadedShaderStages.end()) {
		return &(it->second);
	}
	return nullptr;
}

void GLShader::reloadShaderStage(ShaderStage stage) {
	std::cout << "Calling reloadShaderStage" << std::endl;
	LoadedShaderStage* prevStage = findLoadedShaderStage(stage);

	GLuint newFragmentId;
	if (prevStage) {
		std::string srcCode = prevStage->getStageSourceCode();
		exportShader(prevStage->getStageFileLocation(), srcCode);
		switch (stage) {
		case FRAGMENT:
			newFragmentId = compileFragmentShader(srcCode.c_str());
			detachShader(prevStage->getLoadedStageId());
			removeShader(prevStage->getLoadedStageId());
			attachShader(newFragmentId);
			linkProgram();

			prevStage->setLoadedStageId(newFragmentId);
			break;
		case VERTEX:
			newFragmentId = compileVertexShader(srcCode.c_str());
			detachShader(prevStage->getLoadedStageId());
			removeShader(prevStage->getLoadedStageId());
			attachShader(newFragmentId);
			linkProgram();

			prevStage->setLoadedStageId(newFragmentId);
			break;
		default:
			break;
		}
	}
	else {
		std::cout << "Shader stage not found." << std::endl;
	}


}

unsigned int GLShader::GetUniformedLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	int location = glGetUniformLocation(m_ProgramId, name.c_str());
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "'doesn't exist!" << std::endl;
	}

	m_UniformLocationCache[name] = location;

	return location;
}