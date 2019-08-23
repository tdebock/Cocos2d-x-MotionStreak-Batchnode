# Cocos2d-x-MotionStreak-Batchode

## A custom cocos2d-x Node to combine all those fancy MotionStreaks into one batched draw call via combined triangle strips.

---

Example usage:
```
#include "BatchableMotionStreak.hpp"
#include "MotionStreakBatchNode.hpp"

#define MOTION_STREAK_BATCH_Z_ORDER 369

...

//add a MotionStreakBatchNode to the root node at a specific z-order to not break other auto-batch rules
MotionStreakBatchNode *motionStreakBatchNode = MotionStreakBatchNode::create("spriteFile.png");
this->addChild(motionStreakBatchNode, MOTION_STREAK_BATCH_Z_ORDER);

...

//Add some test MotionStreak instances
for (int i = 0; i < 100; i++) {
    BatchableMotionStreak *motionStreak = BatchableMotionStreak::create(0.025f, -1, 1, Color3B::WHITE, "spriteFile.png");
    motionStreakBatchNode->addChild(motionStreak);
}
//These are all batched into a single glDrawArrays call now   
```

---

Notes:
- All children of MotionStreakBatchNode must derive from a BatchableMotionStreak class.
- The MotionStreakBatchNode class has some asserts for validation on the BatchableMotionStreak child type constraint.
- If you have MotionStreaks with different textures, add them to different MotionStreakBatchNode instances.
- All children need to share the same texture with the MotionStreakBatchNode.
- The batchnode does not take openGL vertex limits into account. If you exceed the vertex buffer limit, simply create an additional batch and keep the max vertex count within limits.