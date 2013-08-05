//
//  Enemy.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 05/07/13.
//
//

#include "Enemy.h"
#include "Player.h"
#include "Cave.h"

bool Enemy::init(Dictionary *properties) {
    
    if (!GameObject::init(properties))
        return false;
    
    _walkRoutine = routineNone;
    _playerDistanceToFollow = -1;
    _ignoreRoom = false;
    _walkTries = 0;
    
    String *facingProperty = (String*)properties->objectForKey("Facing");
    String *walkProperty = (String*)properties->objectForKey("Walk");
    String *speedProperty = (String*)properties->objectForKey("Speed");
    
    String *distance = (String*)properties->objectForKey("DistanceToFollow");
    if (distance)
        _playerDistanceToFollow = distance->intValue();
    
    String *ignoreRoomProperty = (String*)properties->objectForKey("IgnoreRoom");
    if (ignoreRoomProperty)
        _ignoreRoom = true;
    
    if (facingProperty) {
        if (facingProperty->compare("up") == 0)
            _facing = facingUp;
        else if (facingProperty->compare("left") == 0)
            _facing = facingLeft;
        else if (facingProperty->compare("right") == 0)
            _facing = facingRight;
    }
    
    if (speedProperty) {
        float speed = speedProperty->intValue();
        if (speed > 0) {
            _walkTime = speed / 1000.0f;
        }
    }
    
    const char *frameName = this->getDirectionName();
    
    this->setSpriteByFrameName(String::createWithFormat("enemy%s1.png", frameName)->getCString());
    
    int x = ((String*)properties->objectForKey("x"))->intValue();
    int y = ((String*)properties->objectForKey("y"))->intValue();
    
    this->getNode()->setAnchorPoint(ccp(0, 0));
    this->getNode()->setPosition(ccp(x, y));
    
    if (walkProperty) {
        if (walkProperty->compare("bf") == 0)
            _walkRoutine = routineBackAndForth;
        else if (walkProperty->compare("cw") == 0)
            _walkRoutine = routineClockwise;
        else if (walkProperty->compare("ccw") == 0)
            _walkRoutine = routineCounterClockwise;
        else if (walkProperty->compare("r") == 0)
            _walkRoutine = routineRandom;
        else if (walkProperty->compare("fp") == 0)
            _walkRoutine = routineFollowPlayer;
        else if (walkProperty->compare("fpr") == 0)
            _walkRoutine = routineFollowPlayerAndRandom;
        
        this->walk(false);
    }
    
    return true;
}

bool Enemy::isPlayerInRoom() {
    
    Point currentMapPosition = this->getCave()->getMapPosition(this->getNode()->getPosition());
    Point playerMapPosition = this->getCave()->getMapPosition(this->getCave()->getPlayer()->getNode()->getPosition());
    
    return this->isPlayerInRoom(currentMapPosition, playerMapPosition);
}

bool Enemy::isPlayerInRoom(Point currentMapPosition, Point playerMapPosition) {
    return !this->getCave()->getPlayer()->isDead && currentMapPosition.x == playerMapPosition.x && currentMapPosition.y == playerMapPosition.y;
}

Point Enemy::getTargetPosition() {
    
    Size tileSize = this->getCave()->getTileSize();
    Point currentPosition = this->getNode()->getPosition();
    Point currentTileCoord = this->getCave()->getCharacterTileCoordForPosition(currentPosition);
    
    if (_walkRoutine == routineFollowPlayer || _walkRoutine == routineFollowPlayerAndRandom) {
        
        Point playerPosition = this->getCave()->getPlayer()->getNode()->getPosition();
        Point playerTileCoord = this->getCave()->getCharacterTileCoordForPosition(playerPosition);
        
        bool playerInRoom = this->isPlayerInRoom();
        int xDifference = currentTileCoord.x - playerTileCoord.x;
        int yDifference = currentTileCoord.y - playerTileCoord.y;
        int xAbsoluteDifference = abs(xDifference);
        int yAbsoluteDifference = abs(yDifference);
        
        if ((playerInRoom || _ignoreRoom) && (_playerDistanceToFollow <= 0 || (xAbsoluteDifference <= _playerDistanceToFollow && yAbsoluteDifference <= _playerDistanceToFollow))) {
            
            // Follows the player
            
            bool moveHorizontal = xAbsoluteDifference == yAbsoluteDifference ? rand() % 2 : xAbsoluteDifference > yAbsoluteDifference;
            
            if (moveHorizontal) {
                _facing = xDifference > 0 ? facingLeft : facingRight;
            } else {
                _facing = yDifference > 0 ? facingUp : facingDown;
            }
            
        } else {
            
            if (_walkRoutine == routineFollowPlayerAndRandom)
                _facing = (CharacterFacing)(rand() % 4); // Walk randomically
            else
                return currentPosition; // Stop
            
        }
        
    } else if (_walkRoutine == routineRandom) {
        
        _facing = (CharacterFacing)(rand() % 4);
        
    } else {
        
        String *goProperty = this->getCave()->getTileProperty(this->getCave()->getMetaLayer(), currentTileCoord, "Go");
        
        if (goProperty) {
            if (goProperty->compare("up") == 0)
                _facing = facingUp;
            else if (goProperty->compare("down") == 0)
                _facing = facingDown;
            else if (goProperty->compare("left") == 0)
                _facing = facingLeft;
            else if (goProperty->compare("right") == 0)
                _facing = facingRight;
            else if (goProperty->compare("upleft") == 0)
                _facing = _facing == facingUp || _facing == facingRight ? facingUp : facingLeft;
            else if (goProperty->compare("upright") == 0)
                _facing = _facing == facingUp || _facing == facingLeft ? facingUp : facingRight;
            else if (goProperty->compare("downleft") == 0)
                _facing = _facing == facingDown || _facing == facingRight ? facingDown : facingLeft;
            else if (goProperty->compare("downright") == 0)
                _facing = _facing == facingDown || _facing == facingLeft ? facingDown : facingRight;
        }
    }
    
    Point targetPosition = currentPosition;
    
    switch (_facing) {
        case facingUp: targetPosition.y += tileSize.height; break;
        case facingDown: targetPosition.y -= tileSize.height; break;
        case facingRight: targetPosition.x += tileSize.width; break;
        case facingLeft: targetPosition.x -= tileSize.width; break;
        default: break;
    }
    
    return targetPosition;
}

bool Enemy::canWalkTo(Point position) {
    
    if (!GameObject::canWalkTo(position))
        return false;
    
    Point tileCoord = this->getCave()->getCharacterTileCoordForPosition(position);
    
    int floorTileGID = this->getCave()->getFloorLayer()->tileGIDAt(tileCoord);
    if (this->getCave()->checkTileProperty(floorTileGID, "NarrowPassage"))
        return false;
    
    if (this->getCave()->getPlayer()->isDraggingTo(tileCoord))
        return false;
    
    return true;
}

void Enemy::walkTo(Point position, float time) {
    
    _steppingTileCoord = this->getCave()->getCharacterTileCoordForPosition(this->getNode()->getPosition());
    
    Action *steppingOutAction = Sequence::create(
                                                 DelayTime::create(time / 2),
                                                 CallFunc::create(this, callfunc_selector(Enemy::triggerWeightSwitch)),
                                                 NULL);
    
    SpriteFrameCache *spriteCache = SpriteFrameCache::sharedSpriteFrameCache();
    
    const char *frameName = this->getDirectionName();
    
    Animation *anim = Animation::create();
    
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("enemy%s2.png", frameName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("enemy%s1.png", frameName)->getCString()));
    
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("enemy%s3.png", frameName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("enemy%s1.png", frameName)->getCString()));
    
    anim->setDelayPerUnit(time / anim->getFrames()->count());
    anim->setRestoreOriginalFrame(false);
    
    Action *action = Sequence::create(MoveTo::create(time, position),
                                      CallFunc::create(this, callfunc_selector(Enemy::finishWalk)),
                                      NULL);
    
    this->getNode()->stopAllActions();
    this->getNode()->runAction(Animate::create(anim));
    this->getNode()->runAction(action);
    this->getNode()->runAction(steppingOutAction);
    
    _movementIndex = _movementIndex == 2 ? 3 : 2;
    
}

bool Enemy::walk(bool forceTry) {
    
    if (_isWalking || (!forceTry && _walkTries > 3))
        return true;
    
    Point currentPosition = this->getNode()->getPosition();
    Point targetPosition = this->getTargetPosition();
    
    if (targetPosition.x == currentPosition.x && targetPosition.y == currentPosition.y)
        return false;
    
    if (!this->canWalkTo(targetPosition)) {
        _walkTries++;
        
        switch (_walkRoutine) {
            case routineBackAndForth:
                switch (_facing) {
                    case facingUp: _facing = facingDown; break;
                    case facingDown: _facing = facingUp; break;
                    case facingRight: _facing = facingLeft; break;
                    case facingLeft: _facing = facingRight; break;
                    default: break;
                }
                break;
                
            case routineClockwise:
                switch (_facing) {
                    case facingUp: _facing = facingRight; break;
                    case facingDown: _facing = facingLeft; break;
                    case facingRight: _facing = facingDown; break;
                    case facingLeft: _facing = facingUp; break;
                    default: break;
                }
                break;
                
            case routineCounterClockwise:
                switch (_facing) {
                    case facingUp: _facing = facingLeft; break;
                    case facingDown: _facing = facingRight; break;
                    case facingRight: _facing = facingUp; break;
                    case facingLeft: _facing = facingDown; break;
                    default: break;
                }
                break;
                
            default: break;
        }
        
        return this->walk(false);
    }
    
    _isWalking = true;
    _walkTries = 0;
    this->walkTo(targetPosition, _walkTime);
    return true;
}

void Enemy::finishWalk() {
    
    Point tileCoord = this->getCave()->getCharacterTileCoordForPosition(this->getNode()->getPosition());
    
    int lowerObjectTileGID = this->getCave()->getLowerObjectsLayer()->tileGIDAt(tileCoord);
    if (lowerObjectTileGID) {
        this->getCave()->triggerWeightSwitch(tileCoord, this->isPlayerInRoom());
    }
    
    _isWalking = false;
    
    this->walk(false);
    
}

void Enemy::triggerWeightSwitch() {
    this->getCave()->triggerWeightSwitch(_steppingTileCoord, this->isPlayerInRoom());
}
