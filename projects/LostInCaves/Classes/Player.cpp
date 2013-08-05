//
//  Player.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 05/07/13.
//
//

#include "Player.h"
#include "Cave.h"

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

bool Player::init(Dictionary *properties) {
    
    if (!GameObject::init(properties))
        return false;
    
    _keyQuantity = 0;
    _isDraggingObject = false;
    _isPushingObject = false;
    _shouldFinishDrag = false;
    _draggingTileGID = 0;
    _walkingNewTileCoord = ccp(0, 0);
    _draggingTileCoord = ccp(0, 0);
    _draggingNewTileCoord = ccp(0, 0);
    
    int x = ((String*)properties->objectForKey("x"))->intValue();
    int y = ((String*)properties->objectForKey("y"))->intValue();
    
    this->setSpriteByFrameName("down1.png");
    this->getNode()->setAnchorPoint(ccp(0, 0));
    this->getNode()->setPosition(ccp(x, y));
    
    ((Sprite *)this->getNode())->getTexture()->setAliasTexParameters();
    
    return true;
}

bool Player::isDraggingTo(Point targetCoord) {
    return _isDraggingObject && targetCoord.x == _draggingNewTileCoord.x && targetCoord.y == _draggingNewTileCoord.y;
}

void Player::startIdleAnimation() {
    
    const char* directionName = this->getDirectionName();
    
    SpriteFrameCache *spriteCache = SpriteFrameCache::sharedSpriteFrameCache();
    Animation *anim = Animation::create();
    
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%s1.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%s1.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%s1.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%s1.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%sidle1.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%sidle2.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%sidle2.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%sidle2.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%sidle2.png", directionName)->getCString()));
    anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%sidle1.png", directionName)->getCString()));
    
    anim->setDelayPerUnit(0.1f);
    anim->setRestoreOriginalFrame(true);
    
    this->getNode()->stopAllActions();
    this->getNode()->runAction(RepeatForever::create(Animate::create(anim)));
    
}

void Player::updateSprite() {
    
    if (_isWalking)
        return;
    
    this->getNode()->stopAllActions();
    
    const char *frameName = _isDraggingObject ? "drag" : "";
    
    ((Sprite *)this->getNode())->setDisplayFrame
    (
     SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName
     (
      String::createWithFormat("%s%s1.png", this->getDirectionName(), frameName)->getCString()
      ));
    
    if (!_isDraggingObject)
        this->startIdleAnimation();
}

void Player::die() {
    
    this->getCave()->stopAllActions();
    this->getNode()->stopAllActions();
    isDead = true;
    
    bool fallLeft = _facing == facingLeft || _facing == facingDown;
    int multp = fallLeft ? 1 : -1;
    const char *frameName = fallLeft ? "damageright1.png" : "damageleft1.png";
    
    ((Sprite *)this->getNode())->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName));
    
    Action *action = Sequence::create(
                                      JumpBy::create(0.3f, ccp(8 * multp, 0), 4, 1),
                                      JumpBy::create(0.2f, ccp(4 * multp, 0), 2, 1),
                                      MoveBy::create(0.1f, ccp(2 * multp, 0)),
                                      CallFunc::create(this, callfunc_selector(Player::finishDamage)),
                                      NULL);
    
    this->getNode()->runAction(action);
    SimpleAudioEngine::sharedEngine()->playEffect("sfx_hit.wav");
    //SimpleAudioEngine::sharedEngine()->playEffect("sfx_die.wav");
    
}

bool Player::walk(bool forceTry) {
    
    if (_isWalking && !forceTry) return true;
    //if (!(up || down || left || right)) return false;
    
    Cave *cave = this->getCave();
    
    if (_isDraggingObject) {
        if (
            ((_facing == facingUp || _facing == facingDown) && (cave->left || cave->right)) ||
            ((_facing == facingLeft || _facing == facingRight) && (cave->up || cave->down))
            )
            return false;
    }
    
    Point thisPosition = this->getNode()->getPosition();
    
    Size tileSize = cave->getTileSize();
    const char *facingFrameName;
    CharacterFacing newFacing = _facing;
    
    if (cave->up) {
        if (!_isDraggingObject)
            newFacing = facingUp;
        thisPosition.y += tileSize.height;
        facingFrameName = "up1.png";
    } else if (cave->down) {
        if (!_isDraggingObject)
            newFacing = facingDown;
        thisPosition.y -= tileSize.height;
        facingFrameName = "down1.png";
    } else if (cave->right) {
        if (!_isDraggingObject)
            newFacing = facingRight;
        thisPosition.x += tileSize.width;
        facingFrameName = "right1.png";
    } else if (cave->left) {
        if (!_isDraggingObject)
            newFacing = facingLeft;
        thisPosition.x -= tileSize.width;
        facingFrameName = "left1.png";
    }
    
    if (newFacing != _facing)
        ((Sprite *)this->getNode())->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(facingFrameName));
    _facing = newFacing;
    
    _walkingNewTileCoord = cave->getCharacterTileCoordForPosition(thisPosition);
    
    if (_isDraggingObject) {
        
        _isPushingObject = (cave->up && _facing == facingUp) || (cave->down && _facing == facingDown) || (cave->left && _facing == facingLeft) || (cave->right && _facing == facingRight);
        
        Point targetCoord = this->getFacingTileCoord(_isPushingObject ? 2 : -1);
        
        if (!this->getCave()->canDragTo(targetCoord, _isPushingObject))
            return false;
        
        Point draggingObjectTileCoord = this->getFacingTileCoord(1);
        
        _draggingTileGID = cave->getObjectsLayer()->tileGIDAt(draggingObjectTileCoord);
        _draggingTileCoord = draggingObjectTileCoord;
        _draggingNewTileCoord = draggingObjectTileCoord;
        
        Sprite *draggingObject = cave->getObjectsLayer()->tileAt(draggingObjectTileCoord);
        Point draggingObjectPosition = draggingObject->getPosition();
        
        if (cave->up) {
            draggingObjectPosition.y += tileSize.height;
            _draggingNewTileCoord.y--;
        } else if (cave->down) {
            draggingObjectPosition.y -= tileSize.height;
            _draggingNewTileCoord.y++;
        } else if (cave->right) {
            draggingObjectPosition.x += tileSize.width;
            _draggingNewTileCoord.x++;
        } else if (cave->left) {
            draggingObjectPosition.x -= tileSize.width;
            _draggingNewTileCoord.x--;
        }
        
        if (!_isPushingObject && !this->getCave()->canDragTo(_draggingNewTileCoord, true))
            return false;
        
        _shouldFinishDrag = true;
        
        Action *draggingOutAction = Sequence::create(
                                                     DelayTime::create(kDragTime / 2),
                                                     CallFunc::create(this, callfunc_selector(Player::triggerTargetWeightSwitch)),
                                                     NULL);
        
        this->walkTo(thisPosition, kDragTime);
        draggingObject->runAction(Spawn::create(
                                                MoveTo::create(kDragTime, draggingObjectPosition),
                                                draggingOutAction,
                                                NULL));
        
        SimpleAudioEngine::sharedEngine()->playEffect("sfx_push_object.wav");
        
        return true;
        
    }
    
    if (!this->canWalkTo(thisPosition)) {
        this->updateSprite();
        return false;
    }
    
    this->walkTo(thisPosition, kWalkTime);
    return true;
}

void Player::walkTo(Point position, float time) {
    
    _steppingTileCoord = this->getCave()->getCharacterTileCoordForPosition(this->getNode()->getPosition());
    
    Sequence *steppingOutAction = Sequence::create(
                                                   DelayTime::create(time / 2),
                                                   CallFunc::create(this, callfunc_selector(Player::triggerWeightSwitch)),
                                                   NULL);
    
    SpriteFrameCache *spriteCache = SpriteFrameCache::sharedSpriteFrameCache();
    
    const char *frameName = this->getDirectionName();
    
    Animation *anim = Animation::create();
    
    if (_isDraggingObject) {
        
        int frameNumber = _isPushingObject ? 3 : 2;
        
        anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%sdrag%i.png", frameName, frameNumber)->getCString()));
        anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%sdrag1.png", frameName)->getCString()));
        
    } else {
        
        anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%s%i.png", frameName, _movementIndex)->getCString()));
        anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%s1.png", frameName)->getCString()));
    }
    
    anim->setDelayPerUnit(time / anim->getFrames()->count());
    anim->setRestoreOriginalFrame(false);
    
    Sequence *action = Sequence::create(MoveTo::create(time, position),
                                        CallFunc::create(this, callfunc_selector(Player::finishWalk)),
                                        NULL);
    
    this->getNode()->stopAllActions();
    this->getNode()->runAction(Spawn::create(
                                    action,
                                    Animate::create(anim),
                                    steppingOutAction,
                                    NULL));
    
    _movementIndex = _movementIndex == 2 ? 3 : 2;
    
}

void Player::finishWalk() {
    
    Cave *cave = this->getCave();
    Point thisPosition = this->getNode()->getPosition();
    Point tileCoord = cave->getCharacterTileCoordForPosition(thisPosition);
    
    cave->updateMapPosition(thisPosition);
    
    int lowerObjectTileGID = cave->getLowerObjectsLayer()->tileGIDAt(tileCoord);
    if (lowerObjectTileGID > 0) {
        Dictionary *properties = cave->getTileProperties(lowerObjectTileGID);
        
        if (properties->objectForKey("Damage")) {
            this->die();
            return;
        }
        
        if (properties->objectForKey("LevelEnd")) {
            isDead = true;
            cave->initiateLevelEnd();
            return;
        }
        
        String *collectValue = (String *)properties->objectForKey("Collectable");
        if (collectValue && collectValue->compare("Key") == 0) {
            cave->getLowerObjectsLayer()->removeTileAt(tileCoord);
            _keyQuantity++;
            SimpleAudioEngine::sharedEngine()->playEffect("sfx_key.wav");
            cave->updateHud();
        }
        
        cave->triggerWeightSwitch(tileCoord);
    }
    
    if (_shouldFinishDrag) {
        cave->getObjectsLayer()->removeTileAt(_draggingTileCoord);
        cave->getObjectsLayer()->setTileGID(_draggingTileGID, _draggingNewTileCoord);
        cave->triggerWeightSwitch(_draggingNewTileCoord);
    }
    
    _shouldFinishDrag = false;
    _isWalking = false;
    
    if (cave->up || cave->down || cave->left || cave->right) _isWalking = this->walk(true);
    
    this->updateSprite();
    
}

void Player::finishDamage() {
    
    this->getCave()->restartLevel();
    
}

void Player::walk() {
    
    _isWalking = this->walk(false);
    
}

void Player::action(bool pressed) {
    
    Point actionTileCoord = this->getFacingTileCoord(1);
    
    if (!pressed && _isDraggingObject) {
        _isDraggingObject = false;
        this->updateSprite();
    }
    
    if (!pressed && !_isWalking && this->getCave()->useObject(actionTileCoord)) {
        if (isDead)
            return;
        
        SimpleAudioEngine::sharedEngine()->playEffect("sfx_use_trigger.wav");
        
        const char* playerDirection = this->getDirectionName();
        
        SpriteFrameCache *spriteCache = SpriteFrameCache::sharedSpriteFrameCache();
        Animation *anim = Animation::create();
        
        anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%suse2.png", playerDirection)->getCString()));
        anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%suse2.png", playerDirection)->getCString()));
        anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%suse1.png", playerDirection)->getCString()));
        anim->addSpriteFrame(spriteCache->spriteFrameByName(String::createWithFormat("%s1.png", playerDirection)->getCString()));
        
        anim->setDelayPerUnit(0.1f);
        anim->setRestoreOriginalFrame(false);
        
        this->getNode()->stopAllActions();
        this->getNode()->runAction(Sequence::create(
                                           Animate::create(anim),
                                           CallFunc::create(this, callfunc_selector(Player::startIdleAnimation)),
                                           NULL));
        
        return;
    }
    
    if (this->getCave()->checkTileProperty(this->getCave()->getObjectsLayer(), actionTileCoord, "Draggable")) {
        
        if (pressed && !_isDraggingObject) {
            _isDraggingObject = true;
            this->updateSprite();
        }
        
    }
    
}

Point Player::getCurrentTileCoord() {
    return _isWalking ? _walkingNewTileCoord : this->getCave()->getCharacterTileCoordForPosition(this->getNode()->getPosition());
}

void Player::triggerTargetWeightSwitch() {
    this->getCave()->triggerWeightSwitch(_draggingTileCoord);
}