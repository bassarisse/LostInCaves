//
//  GameEnd.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#ifndef __LostInCaves__GameEnd__
#define __LostInCaves__GameEnd__

#include "cocos2d.h"
#include "BaseLayer.h"

using namespace cocos2d;

class GameEnd : public BaseLayer
{
public:
    
    ~GameEnd();
    
    virtual bool init();
    
    static Scene* scene();
    
    CREATE_FUNC(GameEnd);
    
    virtual void buttonStart(bool pressed);
    
    int labelAnimationFinishCount;
    bool finished;
    
    void animateLabel(Node *node, float delay);
    void finishLabelAnimation();
    void goToTitleScreen();
    
};

#endif /* defined(__LostInCaves__GameEnd__) */
