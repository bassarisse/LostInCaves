//
//  BaseLayer.cpp
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#include "BaseLayer.h"

BaseLayer::~BaseLayer() {
    
}

bool BaseLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Director::sharedDirector()->purgeCachedData();
    SpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    
    this->setKeyboardEnabled(true);
    
    return true;
}


void BaseLayer::recursivelyPauseAllChildren(Node *node) {
    node->pauseSchedulerAndActions();
    
    Object *obj;
    CCARRAY_FOREACH(node->getChildren(), obj) {
        Node *n = (Node *)obj;
        this->recursivelyPauseAllChildren(n);
    }
}

void BaseLayer::recursivelyResumeAllChildren(Node *node) {
    node->resumeSchedulerAndActions();
    
    Object *obj;
    CCARRAY_FOREACH(node->getChildren(), obj) {
        Node *n = (Node *)obj;
        this->recursivelyResumeAllChildren(n);
    }
}

void BaseLayer::keyPressed(int keyCode)
{
    this->handleKey(keyCode, true);
}

void BaseLayer::keyReleased(int keyCode)
{
    this->handleKey(keyCode, false);
}

void BaseLayer::handleKey(int keyCode, bool pressed) {
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    switch (keyCode) {
        case 126:
        case 13: // W
            buttonUp(pressed); break;
        case 125:
        case 1: // S
            buttonDown(pressed); break;
        case 123:
        case 0: // A
            buttonLeft(pressed); break;
        case 124:
        case 2: // D
            buttonRight(pressed); break;
        case 6: // Z
        case 40: // K
        case 53: // esc
            buttonB(pressed); break;
        case 7: // X
        case 37: // L
        case 49: // Space
            buttonA(pressed); break;
        case 36: // ENTER
            buttonStart(pressed); break;
        case 51: // BACKSPACE
            buttonSelect(pressed); break;
            
        default: break;
    }
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    switch (keyCode) {
        case 38:
        case 87: // W
            buttonUp(pressed); break;
        case 40:
        case 83: // S
            buttonDown(pressed); break;
        case 37:
        case 65: // A
            buttonLeft(pressed); break;
        case 39:
        case 68: // D
            buttonRight(pressed); break;
        case 90: // Z
        case 75: // K
        case 27: // esc
            buttonB(pressed); break;
        case 88: // X
        case 76: // L
        case 32: // Space
            buttonA(pressed); break;
        case 13: // ENTER
            buttonStart(pressed); break;
        case 8: // BACKSPACE
            buttonSelect(pressed); break;
            
        default: break;
    }
#endif
}

void BaseLayer::buttonUp(bool pressed) {
}

void BaseLayer::buttonDown(bool pressed) {
}

void BaseLayer::buttonLeft(bool pressed) {
}

void BaseLayer::buttonRight(bool pressed) {
}

void BaseLayer::buttonA(bool pressed) {
}

void BaseLayer::buttonB(bool pressed) {
}

void BaseLayer::buttonStart(bool pressed) {
}

void BaseLayer::buttonSelect(bool pressed) {
}