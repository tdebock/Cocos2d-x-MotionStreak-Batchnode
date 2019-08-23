//
//  BatchableMotionStreak.cpp
//
//  Created by Thibaut De Bock on 8/22/19.
//

#include "BatchableMotionStreak.hpp"

BatchableMotionStreak::BatchableMotionStreak() { }
BatchableMotionStreak::~BatchableMotionStreak() { }

BatchableMotionStreak* BatchableMotionStreak::create(float fade, float minSeg, float stroke, const Color3B& color, const std::string& path) {
    BatchableMotionStreak *ret = new (std::nothrow) BatchableMotionStreak();
    if (ret && ret->initWithFade(fade, minSeg, stroke, color, path)) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

BatchableMotionStreak* BatchableMotionStreak::create(float fade, float minSeg, float stroke, const Color3B& color, Texture2D* texture) {
    BatchableMotionStreak *ret = new (std::nothrow) BatchableMotionStreak();
    if (ret && ret->initWithFade(fade, minSeg, stroke, color, texture)) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}
