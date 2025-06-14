#pragma once
#include <functional>
#include <string>

#include "Enums.h"

struct InitialShaderStageInfo {
private:
	ShaderStage stage;
	std::string stageFileLocation;

	const char* embeddedShaderCode;
	bool isEmbeddedFlag;

public:
	InitialShaderStageInfo(ShaderStage stage, std::string stageFileLocation) {
		this->stage = stage;
		this->stageFileLocation = stageFileLocation;
		this->isEmbeddedFlag = false;
	}

	InitialShaderStageInfo(ShaderStage stage, const char* embeddedShaderCode) {
		this->stage = stage;
		this->embeddedShaderCode = embeddedShaderCode;
		this->isEmbeddedFlag = true;
	}

	ShaderStage getShaderStage() {
		return this->stage;
	}

	std::string getShaderFileLocation() {
		return this->stageFileLocation;
	}

	const char* getEmbeddedShader(){
		return this->embeddedShaderCode;
	}

	bool isEmbedded(){
		return this->isEmbeddedFlag;
	}
};

struct LoadedShaderStage {
private:

	std::string stageFileLocation;
	std::string stageSourceCode;
	const char* shaderName;
	GLuint loadedStageId;
	std::function<void()> reloadCallback;

public:
	LoadedShaderStage(std::string stageFileLoc, std::string stageSrcCode, const char* shaderName, GLuint loadedStageId , std::function<void()> reload) {
		this->stageFileLocation = stageFileLoc;
		this->stageSourceCode = stageSrcCode;
		this->loadedStageId = loadedStageId;
		this->shaderName = shaderName;
		this->reloadCallback = reload;
	}

	void setStageFileLocation(std::string newFileLocation) {
		this->stageFileLocation = newFileLocation;
	}

	void setStageSourceCode(std::string newSourceCode) {
		this->stageSourceCode = newSourceCode;
	}

	void setLoadedStageId(GLuint id) {
		this->loadedStageId = id;
	}

	void setReloadCallback(std::function<void()> reload) {
		this->reloadCallback = reload;
	}

	std::string getStageFileLocation() {
		return this->stageFileLocation;
	}

	std::string getStageSourceCode() {
		return this->stageSourceCode;
	}

	GLuint getLoadedStageId() {
		return this->loadedStageId;
	}

	const char* getShaderName() {
		return this->shaderName;
	}

	void reload() {
		if (reloadCallback) {
			reloadCallback();
		}
	}

};