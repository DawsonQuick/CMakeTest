#pragma once
#include <functional>
#include <string>
#include <Common/GLInclude.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Enums.h"


struct ParticleToGUITransfer {
	//TODO 
	//Add list of Available emitters
	//Add functional callback to create an emitter if GUI is selected
	private:
		std::vector<std::string> m_EmitterNames;
		std::function<void(std::string)> m_AddEmitterCallback;
	public:

	    //Default Constructor
		ParticleToGUITransfer(){

		}

		//Constructor
		ParticleToGUITransfer(std::vector<std::string> emitterNames, std::function<void(std::string)> addEmitterCallback){
			this->m_EmitterNames = emitterNames;
			this->m_AddEmitterCallback = addEmitterCallback;
		}

		std::vector<std::string> getEmitterNames(){
			return this->m_EmitterNames;
		}

		void createEmitter(std::string name){
			m_AddEmitterCallback(name);
		}
};

struct ITransformProvider {
    virtual glm::vec3 getWorldPosition() const = 0;
    virtual glm::quat getWorldRotation() const = 0;   // or a mat4 if you prefer
    virtual ~ITransformProvider() = default;
};

struct Particle {
private:
	
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float lifetime;

public:

	//Default Constructor
	Particle(){

	}

	//Constructor
	Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float lifetime){
		this->position = pos;
		this->velocity = vel;
		this->acceleration = acc;
		this->lifetime = lifetime;
	}

	void setPosition(glm::vec3 newPosition){
		this->position = newPosition;
	}

	void setVelocity(glm::vec3 newVelocity){
		this->velocity = newVelocity;
	}

	void setAcceleration(glm::vec3 newAccleration){
		this->acceleration = newAccleration;
	}

	void setLifetime(float newLifetime){
		this->lifetime = newLifetime;
	}

	glm::vec3& getPosition(){
		return this->position;
	}

	glm::vec3& getVelocity(){
		return this->velocity;
	}

	glm::vec3& getAcceleration(){
		return this->acceleration;
	}

	float& getLifeTime(){
		return this->lifetime;
	}
};


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