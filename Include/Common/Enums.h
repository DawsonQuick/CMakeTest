#pragma once
enum ShaderStage {
	VERTEX,
	FRAGMENT,
};

inline const char* toString(ShaderStage stage) {
    switch (stage) {
    case ShaderStage::VERTEX:   return "VERTEX";
    case ShaderStage::FRAGMENT: return "FRAGMENT";
    default:                    return "UNKNOWN";
    }
}