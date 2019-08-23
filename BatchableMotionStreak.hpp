//
//  BatchableMotionStreak.hpp
//
//  Created by Thibaut De Bock on 8/22/19.
//

#ifndef BatchableMotionStreak_hpp
#define BatchableMotionStreak_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

//BatchableMotionStreak: A subclass of MotionStreak to extract some protected attributes about the instance to work inside the MotionStreakBatchNode

class BatchableMotionStreak: public MotionStreak {
private:
    BatchableMotionStreak();
    ~BatchableMotionStreak();
    
public:
    static BatchableMotionStreak* create(float timeToFade, float minSeg, float strokeWidth, const Color3B& strokeColor, const std::string& imagePath);
    static BatchableMotionStreak* create(float timeToFade, float minSeg, float strokeWidth, const Color3B& strokeColor, Texture2D* texture);
    
    //Get necessary protected attributes from base class
    inline unsigned int getMaxPoints() {
        return _maxPoints;
    }
    
    inline unsigned int getNuPoints() {
        return _nuPoints;
    }
    
    inline Vec2* getVertices() {
        return _vertices;
    }
    
    inline GLubyte* getColorPointers() {
        return _colorPointer;
    }
    
    inline Tex2F* getTexCoords() {
        return _texCoords;
    }
};

#endif /* BatchableMotionStreak_hpp */
