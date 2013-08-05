//
//  Enemy.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 04/07/13.
//
//

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "GameObject.h"

using namespace cocos2d;

class Enemy : public GameObject
{
protected:
    
    CharacterWalkRoutine _walkRoutine;
    int _playerDistanceToFollow;
    bool _ignoreRoom;
    int _walkTries;
    
public:
    
    virtual bool init(Dictionary *properties);
    
    bool isPlayerInRoom();
    bool isPlayerInRoom(Point currentMapPosition, Point playerMapPosition);
    Point getTargetPosition();
    virtual bool canWalkTo(Point position);
    
    virtual bool walk(bool forceTry);
    virtual void walkTo(Point position, float time);
    virtual void finishWalk();
    
    virtual void triggerWeightSwitch();
    
};

#endif /* defined(__ENEMY_H__) */
