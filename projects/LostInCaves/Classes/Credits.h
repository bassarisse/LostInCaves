//
//  Credits.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 07/07/13.
//
//

#ifndef __LostInCaves__Credits__
#define __LostInCaves__Credits__

#include "cocos2d.h"
#include "BaseLayer.h"

using namespace cocos2d;

class Credits : public BaseLayer
{
public:
    
    ~Credits();
    
    virtual bool init();
    
    static Scene* scene();
    
    CREATE_FUNC(Credits);
    
    virtual void buttonB(bool pressed);
    
};

#endif /* defined(__LostInCaves__Credits__) */
