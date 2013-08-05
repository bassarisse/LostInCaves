//
//  GameObject.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 05/07/13.
//
//

#include "GameObject.h"
#include "Cave.h"

bool GameObject::init(Dictionary *properties) {
    
    _facing = facingDown;
    _isWalking = false;
    _movementIndex = 2;
    _walkTime = 0.6f;
    _steppingTileCoord = ccp(0, 0);
    
    return true;
}

void GameObject::setSpriteByFrameName(const char *frameName) {
    this->setNode(Sprite::createWithSpriteFrameName(frameName));
}

const char* GameObject::getDirectionName() {
    return this->getDirectionName(_facing);
}

const char* GameObject::getDirectionName(CharacterFacing facing) {
    switch (facing) {
        case facingUp: return "up"; break;
        case facingLeft: return "left"; break;
        case facingRight: return "right"; break;
        default: return "down"; break;
    }
}

Point GameObject::getFacingTileCoord(int range) {
    return this->getFacingTileCoord(range, _facing);
}

Point GameObject::getFacingTileCoord(int range, CharacterFacing facing) {
    Point tileCoord = this->getCave()->getCharacterTileCoordForPosition(this->getNode()->getPosition());
    return this->getFacingTileCoord(range, facing, tileCoord);
}

Point GameObject::getFacingTileCoord(int range, Point tileCoord) {
    return this->getFacingTileCoord(range, _facing, tileCoord);
}

Point GameObject::getFacingTileCoord(int range, CharacterFacing facing, Point tileCoord) {
    
    switch (facing) {
        case facingUp: tileCoord.y -= range; break;
        case facingDown: tileCoord.y += range; break;
        case facingLeft: tileCoord.x -= range; break;
        case facingRight: tileCoord.x += range; break;
        default: break;
    }
    
    return tileCoord;
}

bool GameObject::canWalkTo(Point position) {
    return this->getCave()->canWalkTo(position);
}

void GameObject::walkTo(Point position, float time) {    
}

bool GameObject::walk(bool forceTry) {
    return true;
}

void GameObject::finishWalk() {
}

void GameObject::die() {
}

void GameObject::triggerWeightSwitch() {
    this->getCave()->triggerWeightSwitch(_steppingTileCoord);
}