#pragma once
#include "ParticleEngine/Emitters/EmitterFactory.h"

#define REGISTER_EMITTER(classname)                           \
    namespace {                                               \
        struct classname##Registrar {                         \
            classname##Registrar() {                          \
                EmitterFactory::registerEmitter(              \
                    #classname,                               \
                    []() -> std::unique_ptr<IEmitter> {       \
                        return std::make_unique<classname>(); \
                    }                                         \
                );                                            \
            }                                                 \
        };                                                    \
        static classname##Registrar global_##classname##_reg; \
    }