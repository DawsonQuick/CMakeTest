#pragma once
#include <functional>
#include <string>
#include <ParticleEngine/Emitters/EmitterInterface.h>
class EmitterFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<IEmitter>()>;

    struct Entry {
        std::string name;
        CreatorFunc creator;
    };

    static void registerEmitter(const std::string& name, CreatorFunc creator);
    static std::vector<std::string> getEmitterNames();
    static std::unique_ptr<IEmitter> create(const std::string& name);
};