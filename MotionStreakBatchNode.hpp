//
//  MotionStreakBatchNode.hpp
//
//  Created by Thibaut De Bock on 8/22/19.
//

#ifndef MotionStreakBatchNode_hpp
#define MotionStreakBatchNode_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BatchableMotionStreak.hpp"

//MotionStreakBatchNode: A custom batch node for MotionStreaks inspired by cocos2d-x SpriteBatchNode
//Note, all childen should be allocated as a BatchableMotionStreak instance

USING_NS_CC;

class MotionStreakBatchNode: public Node, TextureProtocol {
private:
    MotionStreakBatchNode();
    ~MotionStreakBatchNode();
    
    //Texture
    Texture2D* _texture;
    BlendFunc _blendFunc;

    //Render
    Vec2* _vertices;
    GLubyte* _colorPointer;
    Tex2F* _texCoords;
    CustomCommand _customCommand;
    
    //Batch
    unsigned int numberMaxPoints = 0;
    unsigned int numberBatchedPoints = 0;
    unsigned int numberDegenerateVerts = 0;
    
    //Children
    BatchableMotionStreak* validateChild(Node *child);
    void addedMotionStreak(BatchableMotionStreak *motionStreak);
    void removedMotionStreak(BatchableMotionStreak *motionStreak);
    void cleanedUpAllMotionStreaks();
    void processMotionStreakUpdate(BatchableMotionStreak *motionStreak);
    void addLeadingDegenerateTriangleStrip(BatchableMotionStreak *motionStreak);
    void addTrailingDegenerateTriangleStrip(BatchableMotionStreak *motionStreak);
    
protected:
    //Init
    bool initWithTexture(Texture2D *tex);
    bool initWithFile(const std::string& fileImage);
    bool init() override;
    
    //Texture
    virtual Texture2D* getTexture() const override;
    virtual void setTexture(Texture2D *texture) override;
    virtual void setBlendFunc(const BlendFunc &blendFunc) override;
    virtual const BlendFunc& getBlendFunc() const override;
    void updateBlendFunc();
    
    //Render
    void onDraw(const Mat4 &transform, uint32_t flags);

public:
    //Create
    static MotionStreakBatchNode* createWithTexture(Texture2D *tex);
    static MotionStreakBatchNode* create(const std::string& fileImage);
    
    //Visit and Draw
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

    //Children
    virtual void addChild(Node *child) override;
    virtual void addChild(Node *child, int zOrder) override;
    virtual void addChild(Node *child, int zOrder, int tag) override;
    virtual void addChild(Node *child, int zOrder, const std::string &name) override;
    virtual void removeChild(Node *child, bool cleanup) override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;

    //Update
    virtual void update(float delta) override;
};

#endif /* MotionStreakBatchNode_hpp */
