#include <ParticleEngine/Emitters/EmitterFactory.h>

static std::vector<EmitterFactory::Entry>& registry() {
    static std::vector<EmitterFactory::Entry> emitters;
    return emitters;
}

void EmitterFactory::registerEmitter(const std::string& name, CreatorFunc creator) {
    std::cout << "Registering emitter: " << name << std::endl;
    registry().push_back({ name, creator });
}

std::vector<std::string> EmitterFactory::getEmitterNames() {
    std::vector<std::string> names;
    for (const auto& entry : registry()) {
        names.push_back(entry.name);
    }
    return names;
}

std::unique_ptr<IEmitter> EmitterFactory::create(const std::string& name) {
    for (const auto& entry : registry()) {
        if (entry.name == name) return entry.creator();
    }
    return nullptr;
}