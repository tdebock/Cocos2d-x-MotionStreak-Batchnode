//
//  MotionStreakBatchNode.cpp
//  Tap Transcend 3-mobile
//
//  Created by Thibaut De Bock on 8/22/19.
//

#include "MotionStreakBatchNode.hpp"

MotionStreakBatchNode::MotionStreakBatchNode()
    : _vertices(nullptr)
    , _colorPointer(nullptr)
    , _texCoords(nullptr) {
}

MotionStreakBatchNode::~MotionStreakBatchNode() {
    CC_SAFE_FREE(_vertices);
    CC_SAFE_FREE(_colorPointer);
    CC_SAFE_FREE(_texCoords);
}

MotionStreakBatchNode* MotionStreakBatchNode::createWithTexture(Texture2D* tex) {
    MotionStreakBatchNode *batchNode = new (std::nothrow) MotionStreakBatchNode();
    if(batchNode && batchNode->initWithTexture(tex)) {
        batchNode->autorelease();
        return batchNode;
    }
    
    delete batchNode;
    return nullptr;
}

MotionStreakBatchNode* MotionStreakBatchNode::create(const std::string& fileImage) {
    MotionStreakBatchNode *batchNode = new (std::nothrow) MotionStreakBatchNode();
    if (batchNode && batchNode->initWithFile(fileImage))
    {
        batchNode->autorelease();
        return batchNode;
    }
    
    delete batchNode;
    return nullptr;
}

bool MotionStreakBatchNode::initWithTexture(Texture2D *tex) {
    if(tex == nullptr) {
        return false;
    }

    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
    if(!tex->hasPremultipliedAlpha()) {
        _blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
    }
    
    setTexture(tex);
    updateBlendFunc();
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR, tex));
    scheduleUpdate();
    
    //Allocate some small dummy memory to the pointer arrays
    _vertices = (Vec2*)malloc(1);
    _texCoords = (Tex2F*)malloc(1);
    _colorPointer =  (GLubyte*)malloc(1);
    
    return true;
}

bool MotionStreakBatchNode::initWithFile(const std::string& fileImage) {
    Texture2D *texture2D = Director::getInstance()->getTextureCache()->addImage(fileImage);
    return initWithTexture(texture2D);
}


bool MotionStreakBatchNode::init() {
    Texture2D * texture = new (std::nothrow) Texture2D();
    texture->autorelease();
    return this->initWithTexture(texture);
}

void MotionStreakBatchNode::setBlendFunc(const BlendFunc &blendFunc) {
    _blendFunc = blendFunc;
}

const BlendFunc& MotionStreakBatchNode::getBlendFunc() const {
    return _blendFunc;
}

Texture2D* MotionStreakBatchNode::getTexture() const {
    return _texture;
}

void MotionStreakBatchNode::setTexture(Texture2D *texture) {
    _texture = texture;
    updateBlendFunc();
}

void MotionStreakBatchNode::updateBlendFunc() {
    if (! this->getTexture()->hasPremultipliedAlpha()) {
        _blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
        setOpacityModifyRGB(false);
    } else {
        _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        setOpacityModifyRGB(true);
    }
}

void MotionStreakBatchNode::addedMotionStreak(BatchableMotionStreak *motionStreak) {
    motionStreak->unscheduleAllCallbacks();
    motionStreak->reset();

    //Add 2 points for each MotionStreak to allow some memory padding for the degenerate triangles
    //data is allocated and transfered the exact same method as a base MotionStreak node
    //vertices = n * sizeof(Vec2) * 2
    //colorPointer = n * sizeof(GLubyte) * 2 * 4
    //texCoord = n * sizeof(Tex2F) * 2
    numberMaxPoints += motionStreak->getMaxPoints() + 2;
    _vertices = (Vec2*)realloc(_vertices, sizeof(Vec2) * numberMaxPoints * 2);
    _colorPointer = (GLubyte*)realloc(_colorPointer, sizeof(GLubyte) * numberMaxPoints * 8);
    _texCoords = (Tex2F*)realloc(_texCoords, sizeof(Tex2F) * numberMaxPoints * 2);
}

BatchableMotionStreak* MotionStreakBatchNode::validateChild(Node *child) {
    CCASSERT(child != nullptr, "child should not be null");
    CCASSERT(dynamic_cast<BatchableMotionStreak*>(child) != nullptr, "MotionStreakBatchNode only supports BatchableMotionStreak as children");
    BatchableMotionStreak *motionStreak = static_cast<BatchableMotionStreak*>(child);
    CCASSERT(motionStreak->getTexture()->getName() == this->getTexture()->getName(), "BatchableMotionStreak is not using the same texture id");
    
    return motionStreak;
}

void MotionStreakBatchNode::addChild(Node *child) {
    BatchableMotionStreak *motionStreak = this->validateChild(child);
    this->addedMotionStreak(motionStreak);
    Node::addChild(child, child->getLocalZOrder(), child->getName());
}

void MotionStreakBatchNode::addChild(Node *child, int zOrder) {
    BatchableMotionStreak *motionStreak = this->validateChild(child);
    this->addedMotionStreak(motionStreak);
    Node::addChild(child, zOrder, child->getName());
}

void MotionStreakBatchNode::addChild(Node *child, int zOrder, int tag) {
    BatchableMotionStreak *motionStreak = this->validateChild(child);
    this->addedMotionStreak(motionStreak);
    Node::addChild(child, zOrder, tag);
}

void MotionStreakBatchNode::addChild(Node *child, int zOrder, const std::string &name) {
    BatchableMotionStreak *motionStreak = this->validateChild(child);
    this->addedMotionStreak(motionStreak);
    Node::addChild(child, zOrder, name);
}

void MotionStreakBatchNode::removedMotionStreak(BatchableMotionStreak *motionStreak) {
    numberMaxPoints -= (motionStreak->getMaxPoints() + 2);
    if (numberMaxPoints <= 0) {
        this->cleanedUpAllMotionStreaks();
    } else {
        _vertices = (Vec2*)realloc(_vertices, sizeof(Vec2) * numberMaxPoints * 2);
        _colorPointer = (GLubyte*)realloc(_colorPointer, sizeof(GLubyte) * numberMaxPoints * 8);
        _texCoords = (Tex2F*)realloc(_texCoords, sizeof(Tex2F) * numberMaxPoints * 2);
    }
}

void MotionStreakBatchNode::cleanedUpAllMotionStreaks() {
    //realloc pointer arrays with small dummy memory to not be freed.
    _vertices = (Vec2*)realloc(_vertices, 1);
    _colorPointer = (GLubyte*)realloc(_colorPointer, 1);
    _texCoords = (Tex2F*)realloc(_texCoords, 1);
}

void MotionStreakBatchNode::removeChild(Node *child, bool cleanup) {
    BatchableMotionStreak *motionStreak = static_cast<BatchableMotionStreak*>(child);
    if (motionStreak == nullptr) {
        return;
    }
    
    CCASSERT(_children.contains(motionStreak), "MotionStreakBatchNode should contain the child");
    Node::removeChild(motionStreak, cleanup);
    this->removedMotionStreak(motionStreak);
}

void MotionStreakBatchNode::removeAllChildrenWithCleanup(bool doCleanup) {
    Node::removeAllChildrenWithCleanup(doCleanup);
    this->cleanedUpAllMotionStreaks();
}

void MotionStreakBatchNode::update(float delta) {
    //Combine all pointer arrays from individual MotionStreaks into the BatchNode with degenerate triangle spacing
    numberBatchedPoints = 0;
    numberDegenerateVerts = 0;
    for (int i = 0; i < this->getChildren().size(); i++) {
        Node *child = this->getChildren().at(i);
        CCASSERT(dynamic_cast<BatchableMotionStreak*>(child) != nullptr, "MotionStreakBatchNode only supports BatchableMotionStreaks as children");
        BatchableMotionStreak *motionStreak = static_cast<BatchableMotionStreak*>(child);
        motionStreak->update(delta);
        if (i > 0) {
            this->addLeadingDegenerateTriangleStrip(motionStreak);
        }
        this->processMotionStreakUpdate(motionStreak);
        if (i < this->getChildren().size() - 1) {
            this->addTrailingDegenerateTriangleStrip(motionStreak);
        }
    }
}

void MotionStreakBatchNode::processMotionStreakUpdate(BatchableMotionStreak *motionStreak) {
    int currentNuPoints = motionStreak->getNuPoints();

    std::memcpy(&this->_vertices[numberBatchedPoints * 2 + numberDegenerateVerts], motionStreak->getVertices(), sizeof(Vec2) * currentNuPoints * 2);
    std::memcpy(&this->_colorPointer[numberBatchedPoints * 8 + numberDegenerateVerts * 4], motionStreak->getColorPointers(), sizeof(GLubyte) * currentNuPoints * 8);
    std::memcpy(&this->_texCoords[numberBatchedPoints * 2 + numberDegenerateVerts], motionStreak->getTexCoords(), sizeof(Tex2F) * currentNuPoints * 2);

    numberBatchedPoints += currentNuPoints;
}

void MotionStreakBatchNode::addLeadingDegenerateTriangleStrip(BatchableMotionStreak *motionStreak) {
    std::memcpy(&this->_vertices[numberBatchedPoints * 2 + numberDegenerateVerts], motionStreak->getVertices(), sizeof(Vec2));
    std::memcpy(&this->_colorPointer[numberBatchedPoints * 8 + numberDegenerateVerts * 4], motionStreak->getColorPointers(), sizeof(GLubyte) * 4);
    std::memcpy(&this->_texCoords[numberBatchedPoints * 2 + numberDegenerateVerts], motionStreak->getTexCoords(), sizeof(Tex2F));
    
    numberDegenerateVerts += 1;
}

void MotionStreakBatchNode::addTrailingDegenerateTriangleStrip(BatchableMotionStreak *motionStreak) {
    int currentNuPoints = motionStreak->getNuPoints();
    
    std::memcpy(&this->_vertices[numberBatchedPoints * 2 + numberDegenerateVerts], &motionStreak->getVertices()[currentNuPoints * 2 - 1], sizeof(Vec2));
    std::memcpy(&this->_colorPointer[numberBatchedPoints * 8 + numberDegenerateVerts * 4], &motionStreak->getColorPointers()[currentNuPoints * 8 - 4], sizeof(GLubyte) * 4);
    std::memcpy(&this->_texCoords[numberBatchedPoints * 2 + numberDegenerateVerts], &motionStreak->getTexCoords()[currentNuPoints * 2 - 1], sizeof(Tex2F));
    
    numberDegenerateVerts += 1;
}

void MotionStreakBatchNode::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) {
    if (! _visible) {
        return;
    }
    
    sortAllChildren();
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    if (isVisitableByVisitingCamera()) {
        _director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        _director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
        draw(renderer, _modelViewTransform, flags);
        _director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    }
}

void MotionStreakBatchNode::onDraw(const Mat4 &transform, uint32_t flags) {
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
    GL::blendFunc( _blendFunc.src, _blendFunc.dst );
    GL::bindTexture2D( _texture );

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, _vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, _texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, _colorPointer);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)numberBatchedPoints*2 + numberDegenerateVerts);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, numberBatchedPoints*2 + numberDegenerateVerts);
}

void MotionStreakBatchNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
    if(numberBatchedPoints <= 1) {
        return;
    }
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(MotionStreakBatchNode::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}
