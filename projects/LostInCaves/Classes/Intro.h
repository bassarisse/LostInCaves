//
//  Intro.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#ifndef __LostInCaves__Intro__
#define __LostInCaves__Intro__

#include "cocos2d.h"
#include "BaseLayer.h"

using namespace cocos2d;

class Intro : public BaseLayer
{
public:
    
    ~Intro();
    
    virtual bool init();
    
    static Scene* scene();
    
    CREATE_FUNC(Intro);
    
    virtual void buttonStart(bool pressed);
    
    int labelAnimationFinishCount;
    
    void animateLabel(Node *node, float delay);
    void finishLabelAnimation();
    
};

#endif /* defined(__LostInCaves__Intro__) */
