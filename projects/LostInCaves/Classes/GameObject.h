//
//  GameObject.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 05/07/13.
//
//

#ifndef __LostInCaves__GameObject__
#define __LostInCaves__GameObject__
#include "cocos2d.h"
#include "Common.h"

using namespace cocos2d;

class Cave;

class GameObject : public Object
{
protected:
    
    CharacterFacing _facing;
    bool _isWalking;
    int _movementIndex;
    float _walkTime;
    Point _steppingTileCoord;
    
public:
    
    CC_SYNTHESIZE(Cave*, cave, Cave);
    CC_SYNTHESIZE(Node*, node, Node);
    
    virtual bool init(Dictionary *properties);
    virtual void setSpriteByFrameName(const char *frameName);
    const char* getDirectionName();
    const char* getDirectionName(CharacterFacing facing);
    
    Point getFacingTileCoord(int range);
    Point getFacingTileCoord(int range, CharacterFacing facing);
    Point getFacingTileCoord(int range, Point tileCoord);
    Point getFacingTileCoord(int range, CharacterFacing facing, Point tileCoord);
    
    virtual bool canWalkTo(Point position);
    virtual bool walk(bool forceTry);
    virtual void walkTo(Point position, float time);
    virtual void finishWalk();
    virtual void die();
    
    virtual void triggerWeightSwitch();
    
};

#endif /* defined(__LostInCaves__GameObject__) */
