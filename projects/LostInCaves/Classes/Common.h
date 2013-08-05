//
//  Common.h
//  LostInCaves
//
//  Created by Bruno Assarisse on 05/07/13.
//
//

#ifndef LostInCaves_Common_h
#define LostInCaves_Common_h

enum CharacterFacing {
    facingUp = 0,
    facingDown,
    facingLeft,
    facingRight
};

enum CharacterWalkRoutine {
    routineNone = 0,
    routineBackAndForth,
    routineClockwise,
    routineCounterClockwise,
    routineRandom,
    routineFollowPlayer,
    routineFollowPlayerAndRandom
};

#define kLastLevel 12
#define kLevelSongs 3

#define kWalkTime   0.22f
#define kDragTime   0.50f

#define gbDarkestColor ccc4(33, 32, 16, 255)
#define gbDarkestColor3 ccc3(33, 32, 16)
#define gbDarkColor ccc4(107, 105, 49, 255)
#define gbDarkColor3 ccc3(107, 105, 49)
#define gbLightColor ccc4(181, 174, 74, 255)
#define gbLightColor3 ccc3(181, 174, 74)
#define gbLightestColor ccc4(255, 247, 123, 255)
#define gbLightestColor3 ccc3(255, 247, 123)

#endif
