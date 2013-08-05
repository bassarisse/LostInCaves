//
//  Player.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 05/07/13.
//
//

#ifndef __LostInCaves__Player__
#define __LostInCaves__Player__

#include "GameObject.h"

using namespace cocos2d;

class Player : public GameObject {
    
protected:
    
    bool _isDraggingObject;
    bool _isPushingObject;
    bool _shouldFinishDrag;
    int _draggingTileGID;
    Point _walkingNewTileCoord;
    Point _draggingTileCoord;
    Point _draggingNewTileCoord;
    
public:
    
    bool isDead;
    
    virtual bool init(Dictionary *properties);
    virtual bool walk(bool forceTry);
    virtual void walkTo(Point position, float time);
    virtual void finishWalk();
    virtual void die();
    
    void updateSprite();
    void startIdleAnimation();
    bool isDraggingTo(Point targetCoord);
    void finishDamage();
    void walk();
    void action(bool pressed);
    Point getCurrentTileCoord();
    void triggerTargetWeightSwitch();
    
    CC_SYNTHESIZE(int, _keyQuantity, KeyQuantity);
    
};

#endif /* defined(__LostInCaves__Player__) */