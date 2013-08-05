#ifndef __CAVE_H__
#define __CAVE_H__

#include "cocos2d.h"
#include "Common.h"
#include "BaseLayer.h"

using namespace cocos2d;

class Enemy;
class Player;

class Cave : public BaseLayer
{
protected:
    
    int _levelNumber;
    
    bool _isPaused;
    int _pausedOptionIndex;
    int _activeTimeSwitchCount;
    
    SpriteBatchNode *_mainSpriteBatch;
    LayerColor *_coverLayer;
    LabelBMFont *_pausedLabel;
    LabelBMFont *_keyQuantityLabel;
    Layer *_mapMainLayer;
    TMXTiledMap *_tiledMap;
    TMXObjectGroup *_abstractObjectGroup;
    
    LabelBMFont *_pausedResumeLabel;
    LabelBMFont *_pausedRestartLabel;
    LabelBMFont *_pausedExitLabel;
    Sprite *_pausedArrowUp;
    Sprite *_pausedArrowDown;
    
    std::vector<Enemy *> _enemies;
    
public:
    
    Cave(int levelNumber);
    virtual ~Cave();
    
    virtual bool init();  

    static Scene* sceneWithLevel(int level);
    static Cave* createWithLevel(int level);
    
    bool up;
    bool down;
    bool left;
    bool right;
    
    CC_SYNTHESIZE(Player*, _player, Player);
    CC_SYNTHESIZE(TMXLayer*, _metaLayer, MetaLayer);
    CC_SYNTHESIZE(TMXLayer*, _objectsLayer, ObjectsLayer);
    CC_SYNTHESIZE(TMXLayer*, _lowerObjectsLayer, LowerObjectsLayer);
    CC_SYNTHESIZE(TMXLayer*, _floorLayer, FloorLayer);
    CC_SYNTHESIZE(Size, _tileSize, TileSize);
    CC_SYNTHESIZE(Size, _mapSize, MapSize);
    
    const char* getMusicName();
    
    int getTileGIDByProperty(const char* key);
    int getTileGIDByProperty(const char* key, const char* value);
    Point getTileCoordForPosition(Point position);
    Point getPositionForTileCoord(Point tileCoord);
    Point getCharacterTileCoordForPosition(Point position);
    Point getCharacterPositionForTileCoord(Point tileCoord);
    bool checkTileProperty(TMXLayer *layer, Point tileCoord, const std::string& key);
    bool checkTileProperty(unsigned int tileGID, const std::string& key);
    Dictionary* getTileProperties(unsigned int tileGID);
    String* getTileProperty(TMXLayer *layer, Point tileCoord, const std::string& key);
    String* getTileProperty(unsigned int tileGID, const std::string& key);
    String* getAbstractObjectProperty(Point tileCoord, const char* key);
    
    bool canWalkTo(Point position);
    bool canDragTo(Point targetCoord, bool pushing);
    Point getMapPosition(Point position);
    void updateMapPosition();
    void updateMapPosition(Point position);
    void updateHud();
    void updatePausedMenu();
    
    void startLevel();
    virtual void update(float deltaTime);
    virtual void handleKey(int keyCode, bool pressed);
    
    virtual void buttonUp(bool pressed);
    virtual void buttonDown(bool pressed);
    virtual void buttonLeft(bool pressed);
    virtual void buttonRight(bool pressed);
    virtual void buttonA(bool pressed);
    virtual void buttonB(bool pressed);
    virtual void buttonStart(bool pressed);
    
    bool useObject(Point target);
    void triggerActionTarget(Point tileCoord, bool sound, bool isOn);
    void triggerWeightSwitch(Point tileCoord, bool sound);
    void triggerWeightSwitch(Point tileCoord);
    void decreaseTimeSwitchCount();
    void timeSwitchTic();
    void timeSwitchTac();
    void playLevelEndMusic();
    void restartLevel();
    void initiateLevelEnd();
    void levelEnd();
    
};

#endif // __CAVE_H__
