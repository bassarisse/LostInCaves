#include "Cave.h"
#include "Enemy.h"
#include "GameEnd.h"
#include "TitleScreen.h"
#include "SimpleAudioEngine.h"
#include "Player.h"

using namespace CocosDenshion;

USING_NS_CC;

#define kTimeSwitchTicTacActionTag 555

Cave::Cave(int levelNumber) {
    _levelNumber = levelNumber;
}

Cave::~Cave() {
    
    CC_SAFE_RELEASE(_player);
    
    for(std::vector<Enemy *>::size_type i = 0; i != _enemies.size(); i++) {
        CC_SAFE_RELEASE(_enemies[i]);
    }
    
}

Scene* Cave::sceneWithLevel(int level)
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    Cave *layer = Cave::createWithLevel(level);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

Cave* Cave::createWithLevel(int level)
{
    Cave *pRet = new Cave(level);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool Cave::init()
{
    
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    srand(time(NULL));
    
    int maxLevel = UserDefault::sharedUserDefault()->getIntegerForKey("MaxLevel", 1);
    
    if (_levelNumber > maxLevel) {
        
        UserDefault::sharedUserDefault()->setIntegerForKey("MaxLevel", _levelNumber);
        UserDefault::sharedUserDefault()->flush();
        
    }
    
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_cursor_back.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_cursor_move.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_cursor_select.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_deploy.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_die.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_key.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_push_object.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_tac.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_tic.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_use_trigger.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sfx_levelend.wav");
    
    if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(this->getMusicName());
    
    up = false;
    down = false;
    left = false;
    right = false;
    
    _isPaused = false;
    _pausedOptionIndex = 0;
    _activeTimeSwitchCount = 0;
    
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("chars.plist");
    
    _mainSpriteBatch = SpriteBatchNode::create("chars.png");
    _tiledMap = TMXTiledMap::create(String::createWithFormat("%i.tmx", _levelNumber)->getCString());
    _abstractObjectGroup = _tiledMap->objectGroupNamed("Abstract");
    _metaLayer = _tiledMap->layerNamed("Meta");
    _metaLayer->setVisible(false);
    _objectsLayer = _tiledMap->layerNamed("Objects");
    _lowerObjectsLayer = _tiledMap->layerNamed("LowerObjects");
    _floorLayer = _tiledMap->layerNamed("Floor");
    _tileSize = _tiledMap->getTileSize();
    _mapSize = _tiledMap->getMapSize();
    
    _mapMainLayer = Layer::create();
    _mapMainLayer->setContentSize(CCSizeMake(144, 144));
    _mapMainLayer->addChild(_tiledMap, -1);
    
    Array *abstractObjects = _abstractObjectGroup->getObjects();
    
    Dictionary *spawnPoint = _abstractObjectGroup->objectNamed("Spawn");
    
    _player = new Player();
    _player->setCave(this);
    _player->init(spawnPoint);
    
    if (abstractObjects && abstractObjects->count() > 0)
    {
        Object* pObj = NULL;
        CCARRAY_FOREACH(abstractObjects, pObj)
        {
            Dictionary* pDict = (Dictionary*)pObj;
            String *name = (String*)pDict->objectForKey("name");
            if (name && name->_string == "Monster")
            {
                Enemy *aEnemy = new Enemy();
                aEnemy->setCave(this);
                aEnemy->init(pDict);
                _enemies.push_back(aEnemy);
                
                _mainSpriteBatch->addChild(aEnemy->getNode());
            }
        }
    }
    
    _mainSpriteBatch->addChild(_player->getNode());
    
    LayerColor *bg = LayerColor::create(gbDarkestColor);
    
    Sprite *hudNode = Sprite::createWithSpriteFrameName("hudbg.png");
    hudNode->getTexture()->setAliasTexParameters();
    hudNode->setAnchorPoint(ccp(1, 0));
    hudNode->setPosition(ccp(160, 0));
    
    Sprite *keyIcon = Sprite::createWithSpriteFrameName("key.png");
    keyIcon->setAnchorPoint(ccp(1, 1));
    keyIcon->setPosition(ccp(15, 140));
    
    _keyQuantityLabel = LabelBMFont::create("", "miniwhitefont.fnt", 15, kTextAlignmentCenter);
    _keyQuantityLabel->setColor(gbLightColor3);
    _keyQuantityLabel->getTexture()->setAliasTexParameters();
    //_keyQuantityLabel->setAnchorPoint(ccp(0.5f, 1.0f));
    _keyQuantityLabel->setPosition(ccp(8, 119));
    
    hudNode->addChild(keyIcon);
    hudNode->addChild(_keyQuantityLabel);
    
    _coverLayer = LayerColor::create(gbDarkestColor);
    
    const char* levelStr = String::createWithFormat("Level %03d", _levelNumber)->getCString();
    LabelBMFont *levelLabel = LabelBMFont::create(levelStr, "whitefont.fnt", 160, kTextAlignmentCenter);
    levelLabel->setColor(gbLightColor3);
    levelLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    levelLabel->setPosition(ccp(80, 72));
    
    _pausedLabel = LabelBMFont::create("Paused", "miniwhitefont.fnt", 160, kTextAlignmentCenter);
    _pausedLabel->setColor(gbLightColor3);
    _pausedLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    _pausedLabel->setPosition(ccp(80, 86));
    _pausedLabel->setVisible(false);
    
    _pausedResumeLabel = LabelBMFont::create("Resume", "whitefont.fnt", 160, kTextAlignmentCenter);
    _pausedResumeLabel->setColor(gbLightColor3);
    _pausedResumeLabel->getTexture()->setAliasTexParameters();
    _pausedResumeLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    _pausedResumeLabel->setPosition(ccp(80, 30));
    
    _pausedRestartLabel = LabelBMFont::create("Restart level", "whitefont.fnt", 160, kTextAlignmentCenter);
    _pausedRestartLabel->setColor(gbLightColor3);
    _pausedRestartLabel->getTexture()->setAliasTexParameters();
    _pausedRestartLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    _pausedRestartLabel->setPosition(ccp(80, 30));
    
    _pausedExitLabel = LabelBMFont::create("Exit", "whitefont.fnt", 160, kTextAlignmentCenter);
    _pausedExitLabel->setColor(gbLightColor3);
    _pausedExitLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    _pausedExitLabel->setPosition(ccp(80, 30));
    
    _pausedArrowUp = Sprite::createWithSpriteFrameName("arrowup.png");
    _pausedArrowUp->setAnchorPoint(ccp(0.5f, 0.0f));
    _pausedArrowUp->setPosition(ccp(80, 34));
    _pausedArrowUp->getTexture()->setAliasTexParameters();
    
    _pausedArrowDown = Sprite::createWithSpriteFrameName("arrowdown.png");
    _pausedArrowDown->setAnchorPoint(ccp(0.5f, 1.0f));
    _pausedArrowDown->setPosition(ccp(80, 26));
    
    _coverLayer->addChild(levelLabel);
    _coverLayer->addChild(_pausedLabel);
    _coverLayer->addChild(_pausedResumeLabel);
    _coverLayer->addChild(_pausedRestartLabel);
    _coverLayer->addChild(_pausedExitLabel);
    _coverLayer->addChild(_pausedArrowUp);
    _coverLayer->addChild(_pausedArrowDown);
    
    _mapMainLayer->addChild(_mainSpriteBatch);
    
    this->updateMapPosition();
    this->updateHud();
    this->updatePausedMenu();
    _player->startIdleAnimation();
    
    this->addChild(bg);
    this->addChild(_mapMainLayer);
    this->addChild(hudNode);
    this->addChild(_coverLayer, 10);
    this->scheduleUpdate();
    
    this->recursivelyPauseAllChildren(this);
    
    Action *startLevelAction = Sequence::create(
                                                DelayTime::create(2.5f),
                                                CallFunc::create(this, callfunc_selector(Cave::startLevel)),
                                                NULL
                                                );
    
    _coverLayer->runAction(startLevelAction);
    
    return true;
}

const char* Cave::getMusicName() {
    return String::createWithFormat("song_level%i.wav", 1 + (_levelNumber + kLevelSongs - 1) % kLevelSongs)->getCString();
}

int Cave::getTileGIDByProperty(const char* key) {
    
    DictElement *tileElement;
    DictElement *propertyElement;
    Dictionary *tiles = _tiledMap->tileProperties();
    
    CCDICT_FOREACH(tiles, tileElement) {
        Dictionary *properties = (Dictionary*)tileElement->getObject();
        
        CCDICT_FOREACH(properties, propertyElement) {
            if (strcmp(propertyElement->getStrKey(), key) == 0)
                return tileElement->getIntKey();
        }
    }
    
    return 0;
}

int Cave::getTileGIDByProperty(const char* key, const char* value) {
    
    DictElement *tileElement;
    DictElement *propertyElement;
    Dictionary *tiles = _tiledMap->tileProperties();
    
    CCDICT_FOREACH(tiles, tileElement) {
        Dictionary *properties = (Dictionary*)tileElement->getObject();
        
        CCDICT_FOREACH(properties, propertyElement) {
            if (strcmp(propertyElement->getStrKey(), key) == 0 && ((String *)propertyElement->getObject())->compare(value) == 0)
                return tileElement->getIntKey();
        }
    }
    
    return 0;
}

Point Cave::getTileCoordForPosition(Point position) {
    int x = position.x / _tileSize.width;
    int y = (_mapSize.height * _tileSize.height - position.y - _tileSize.height / 2) / _tileSize.height;
    return ccp(x, y);
}

Point Cave::getPositionForTileCoord(Point tileCoord) {
    int x = tileCoord.x * _tileSize.width;
    int y = ((_mapSize.height - tileCoord.y - 1) * _tileSize.height);// - tileSize.height / 2;
    return ccp(x, y);
}

Point Cave::getCharacterTileCoordForPosition(Point position) {
    int x = position.x / _tileSize.width;
    // we have to consider player height because the anchor point is altered
    int y = (_mapSize.height * _tileSize.height - position.y - _player->getNode()->getContentSize().height / 2) / _tileSize.height;
    return ccp(x, y);
}

Point Cave::getCharacterPositionForTileCoord(Point tileCoord) {
    int x = tileCoord.x * _tileSize.width;
    // we don't have to consider player height because the anchor point is altered
    int y = ((_mapSize.height - tileCoord.y - 1) * _tileSize.height);// - player->getContentSize().height / 2;
    return ccp(x, y);
}

bool Cave::checkTileProperty(TMXLayer *layer, Point tileCoord, const std::string& key) {
    
    int tileGID = layer->tileGIDAt(tileCoord);
    
    return checkTileProperty(tileGID, key);
}

bool Cave::checkTileProperty(unsigned int tileGID, const std::string& key) {
    
    if (tileGID > 0) {
        Dictionary *properties = _tiledMap->propertiesForGID(tileGID);
        if (properties && properties->objectForKey(key))
            return true;
    }
    
    return false;
}

Dictionary* Cave::getTileProperties(unsigned int tileGID) {
    return _tiledMap->propertiesForGID(tileGID);
}

String* Cave::getTileProperty(TMXLayer *layer, Point tileCoord, const std::string& key) {
    
    int tileGID = layer->tileGIDAt(tileCoord);
    
    return getTileProperty(tileGID, key);
}

String* Cave::getTileProperty(unsigned int tileGID, const std::string& key) {
    
    if (tileGID) {
        Dictionary *properties = _tiledMap->propertiesForGID(tileGID);
        if (properties) {
            String *property = (String *)properties->objectForKey(key);
            if (property)
                return property;
        }
    }
    
    return NULL;
}

String* Cave::getAbstractObjectProperty(Point tileCoord, const char* key) {
    
    Array *abstractObjects = _abstractObjectGroup->getObjects();
    
    if (abstractObjects && abstractObjects->count() > 0)
    {
        Object *pObj = NULL;
        DictElement *property = NULL;
        CCARRAY_FOREACH(abstractObjects, pObj)
        {
            Dictionary *pDict = (Dictionary *)pObj;
            
            int x = ((String*)pDict->objectForKey("x"))->intValue();
            int y = ((String*)pDict->objectForKey("y"))->intValue();
            Point abstractObjectCoord = this->getTileCoordForPosition(ccp(x, y));
            
            if (tileCoord.x == abstractObjectCoord.x && tileCoord.y == abstractObjectCoord.y) {
                CCDICT_FOREACH(pDict, property) {
                    if (strcmp(property->getStrKey(), key) == 0)
                        return (String *)property->getObject();
                }
            }
        }
    }
    
    return NULL;
}

bool Cave::canDragTo(Point targetCoord, bool pushing) {
    
    if (targetCoord.x >= _mapSize.width || targetCoord.y >= _mapSize.height || targetCoord.x < 0 || targetCoord.y < 0)
        return false;
    
    int tileGID = _floorLayer->tileGIDAt(targetCoord);
    
    if (this->checkTileProperty(tileGID, "Collidable"))
        return false;
    
    if (pushing && this->checkTileProperty(tileGID, "NarrowPassage"))
        return false;
    
    int objectTileGID = _objectsLayer->tileGIDAt(targetCoord);
    
    if (objectTileGID > 0)
        return false;
    
    if (pushing) {
        for(std::vector<Enemy *>::size_type i = 0; i != _enemies.size(); i++) {
            Enemy *aEnemy = _enemies[i];
            
            Point enemyTile = this->getCharacterTileCoordForPosition(aEnemy->getNode()->getPosition());
            if (targetCoord.x == enemyTile.x && targetCoord.y == enemyTile.y)
                return false;
            
            Point targetTile = this->getCharacterTileCoordForPosition(aEnemy->getTargetPosition());
            if (targetCoord.x == targetTile.x && targetCoord.y == targetTile.y)
                return false;
            
        }
    }
    
    return true;
}

bool Cave::canWalkTo(Point position) {
    
    Point tileCoord = this->getCharacterTileCoordForPosition(position);
    
    if (tileCoord.x >= _mapSize.width || tileCoord.y >= _mapSize.height || tileCoord.x < 0 || tileCoord.y < 0)
        return false;
    
    int floorTileGID = _floorLayer->tileGIDAt(tileCoord);
    if (this->checkTileProperty(floorTileGID, "Collidable"))
        return false;
    
    int objectTileGID = _objectsLayer->tileGIDAt(tileCoord);
    if (objectTileGID > 0)
        return false;
    
    return true;
}

Point Cave::getMapPosition(Point position) {
    return ccp(
               position.x == 0 ? 0 : floorf(position.x / 144.0f),
               position.y == 0 ? 0 : floorf(position.y / 144.0f));
}

void Cave::updateMapPosition() {
    this->updateMapPosition(_player->getNode()->getPosition());
}

void Cave::updateMapPosition(Point position) {
    
    Point mapPosition = this->getMapPosition(position);
    
    _mapMainLayer->setPosition(ccp(mapPosition.x * -144, mapPosition.y * -144));
}

void Cave::updateHud() {
    
    _keyQuantityLabel->setCString(String::createWithFormat("x\n%i", _player->getKeyQuantity())->getCString());
    
}

void Cave::updatePausedMenu() {
    
    _pausedResumeLabel->setVisible(false);
    _pausedRestartLabel->setVisible(false);
    _pausedExitLabel->setVisible(false);
    _pausedArrowUp->setVisible(false);
    _pausedArrowDown->setVisible(false);
    
    if (!_isPaused) return;
    
    if (_pausedOptionIndex == 0) {
        _pausedResumeLabel->setVisible(true);
        _pausedArrowDown->setVisible(true);
    } else if (_pausedOptionIndex == 1) {
        _pausedRestartLabel->setVisible(true);
        _pausedArrowDown->setVisible(true);
        _pausedArrowUp->setVisible(true);
    } else {
        _pausedExitLabel->setVisible(true);
        _pausedArrowUp->setVisible(true);
    }
    
}

void Cave::startLevel() {
    
    _player->isDead = false;
    _mapMainLayer->resumeSchedulerAndActions();
    
    _coverLayer->setVisible(false);
    _pausedLabel->setVisible(true);
    
    if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(this->getMusicName(), true);
    
}

void Cave::update(float deltaTime) {
    
    if (_player->isDead) return;
    
    Point playerPosition = _player->getNode()->getPosition();
    playerPosition.x += 6;
    Point playerTile = this->getCharacterTileCoordForPosition(playerPosition);
    
    for(std::vector<Enemy *>::size_type i = 0; i != _enemies.size(); i++) {
        Enemy *aEnemy = _enemies[i];
        Point enemyPosition = aEnemy->getNode()->getPosition();
        enemyPosition.x += 6;
        Point enemyTile = this->getCharacterTileCoordForPosition(enemyPosition);
        
        if (playerTile.x == enemyTile.x && playerTile.y == enemyTile.y)
            _player->die();
        
        aEnemy->walk(true);
    }
    
}

void Cave::handleKey(int keyCode, bool pressed) {
    
    if (_player->isDead) return;
    
    BaseLayer::handleKey(keyCode, pressed);
}

void Cave::buttonUp(bool pressed) {
    
    if (_isPaused) {
        if (pressed) return;
        _pausedOptionIndex--;
        if (_pausedOptionIndex < 0)
            _pausedOptionIndex = 0;
        else
            SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_move.wav");
        this->updatePausedMenu();
        return;
    }
    
    if (pressed) {
        up = true;
        down = left = right = false;
        _player->walk();
        return;
    }
    up = false;
}

void Cave::buttonDown(bool pressed) {
    
    if (_isPaused) {
        if (pressed) return;
        _pausedOptionIndex++;
        if (_pausedOptionIndex > 2)
            _pausedOptionIndex = 2;
        else
            SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_move.wav");
        this->updatePausedMenu();
        return;
    }
    
    if (pressed) {
        down = true;
        up = left = right = false;
        _player->walk();
        return;
    }
    down = false;
}

void Cave::buttonLeft(bool pressed) {
    
    if (_isPaused) return;
    
    if (pressed) {
        left = true;
        right = up = down = false;
        _player->walk();
        return;
    }
    left = false;
}

void Cave::buttonRight(bool pressed) {
    
    if (_isPaused) return;
    
    if (pressed) {
        right = true;
        left = up = down = false;
        _player->walk();
        return;
    }
    right = false;
}

void Cave::buttonA(bool pressed) {
    
    if (_isPaused) {
        this->buttonStart(pressed);
        return;
    }
    
    _player->action(pressed);
    
    
}

void Cave::buttonB(bool pressed) {
    
    if (_isPaused && !pressed) {
        SimpleAudioEngine::sharedEngine()->playEffect("sfx_cursor_back.wav");
        this->buttonStart(pressed);
    }
    
}

void Cave::buttonStart(bool pressed) {
    if (pressed) return;
    
    if (!_isPaused) {
        
        _isPaused = true;
        this->recursivelyPauseAllChildren(this);
        _pausedOptionIndex = 0;
        this->updatePausedMenu();
        _coverLayer->setVisible(true);
        
    } else {
        
        if (_pausedOptionIndex == 0) {
            _isPaused = false;
            this->recursivelyResumeAllChildren(this);
            _coverLayer->setVisible(false);
        } else if (_pausedOptionIndex == 1) {
            this->restartLevel();
        } else {
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            Director::sharedDirector()->replaceScene(TitleScreen::scene());
        }
        
    }
}

bool Cave::useObject(Point target) {
    
    int tileGID = _objectsLayer->tileGIDAt(target);
    
    if (tileGID == 0) return false;
    
    Dictionary *properties = _tiledMap->propertiesForGID(tileGID);
    
    if (!properties)
        return false;
    
    String *switchType = (String *)properties->objectForKey("Switch");
    if (switchType) {
        
        bool isOn = true;
        String *switchState = (String *)properties->objectForKey(switchType->getCString());
        if (switchState) {
            
            isOn = switchState->compare("on") != 0;
            const char *newSwitchState = isOn ? "on" : "off";
            
            bool isTimeSwitch = switchType->compare("Time") == 0;
            String *deactivateTimeProperty = NULL;
            if (isTimeSwitch)
                deactivateTimeProperty = this->getAbstractObjectProperty(target, "Time");
            
            if (switchType->compare("Lever") == 0 || (isTimeSwitch && strcmp(newSwitchState, "on") == 0 && deactivateTimeProperty)) {
                
                this->triggerActionTarget(target, true, isOn);
                int newTileGID = this->getTileGIDByProperty(switchType->getCString(), newSwitchState);
                _objectsLayer->setTileGID(newTileGID, target);
                
                if (isTimeSwitch) {
                    
                    int aTime = deactivateTimeProperty->intValue();
                    float deactivateTime = aTime == 0 ? 0 : aTime / 1000.0f;
                    
                    std::function<void()> timeSwitchCallback = [this, target](){
                        
                        bool isOn = true;
                        String *switchState = this->getTileProperty(this->getObjectsLayer(), target, "Time");
                        if (switchState) {
                            
                            isOn = switchState->compare("on") != 0;
                            const char *newSwitchState = isOn ? "on" : "off";
                            
                            int newTileGID = this->getTileGIDByProperty("Time", newSwitchState);
                            this->getObjectsLayer()->setTileGID(newTileGID, target);
                            
                            SimpleAudioEngine::sharedEngine()->playEffect("sfx_use_trigger.wav");
                            
                            this->decreaseTimeSwitchCount();
                            
                        }
                        
                        this->triggerActionTarget(target, true, isOn);
                    };
                    
                    Action *timeSwitchAction = Sequence::create(
                                                                DelayTime::create(deactivateTime),
                                                                CallFunc::create(timeSwitchCallback),
                                                                NULL);
                    
                    _tiledMap->runAction(timeSwitchAction);
                    //objectsLayer->tileAt(target)->runAction(timeSwitchAction);
                    
                    if (_activeTimeSwitchCount == 0 && !this->getActionByTag(kTimeSwitchTicTacActionTag)) {
                        
                        ActionInterval *action = Sequence::create(
                                                                  DelayTime::create(0.4f),
                                                                  CallFunc::create(this, callfunc_selector(Cave::timeSwitchTic)),
                                                                  DelayTime::create(0.4f),
                                                                  CallFunc::create(this, callfunc_selector(Cave::timeSwitchTac)),
                                                                  NULL);
                        
                        
                        Action *repeatAction = RepeatForever::create(action);
                        repeatAction->setTag(kTimeSwitchTicTacActionTag);
                        
                        this->runAction(repeatAction);
                        
                    }
                    
                    _activeTimeSwitchCount++;
                    
                }
                
                return true;
                
            } else {
                
                return false;
            }
            
        }
    }
    
    String *doorProperty = (String *)properties->objectForKey("Door");
    
    int keyQuantity = _player->getKeyQuantity();
    
    if (doorProperty && keyQuantity > 0) {
        _player->setKeyQuantity(keyQuantity - 1);
        _objectsLayer->removeTileAt(target);
        this->updateHud();
        return true;
    }
    
    return false;
}

void Cave::triggerActionTarget(Point tileCoord, bool sound, bool isOn) {
    
    Array *abstractObjects = _abstractObjectGroup->getObjects();
    std::vector<const char*> targetNames;
    
    if (abstractObjects && abstractObjects->count() > 0)
    {
        Object *pObj = NULL;
        DictElement *property = NULL;
        CCARRAY_FOREACH(abstractObjects, pObj)
        {
            Dictionary *pDict = (Dictionary *)pObj;
            
            int x = ((String*)pDict->objectForKey("x"))->intValue();
            int y = ((String*)pDict->objectForKey("y"))->intValue();
            Point switchCoord = this->getTileCoordForPosition(ccp(x, y));
            
            if (tileCoord.x == switchCoord.x && tileCoord.y == switchCoord.y) {
                
                if (!isOn && pDict->objectForKey("OnOnly"))
                    continue;
                
                CCDICT_FOREACH(pDict, property) {
                    String *propertyValue = (String *)property->getObject();
                    const char *propertyKey = property->getStrKey();
                    if (propertyValue->compare("") == 0 && strcmp(propertyKey, "name") != 0 && strcmp(propertyKey, "gid") != 0 && strcmp(propertyKey, "type") != 0 && strcmp(propertyKey, "x") != 0 && strcmp(propertyKey, "y") != 0)
                        targetNames.push_back(propertyKey);
                }
            }
        }
        
        CCARRAY_FOREACH(abstractObjects, pObj)
        {
            Dictionary *pDict = (Dictionary *)pObj;
                
            String *targetName = (String*)pDict->objectForKey("name");
            
            if (!targetName)
                continue;
            
            bool isTarget = false;
            
            for(std::vector<const char*>::size_type i = 0; i != targetNames.size(); i++) {
                if (targetName->compare(targetNames[i]) == 0) {
                    isTarget = true;
                    break;
                }
            }
            
            if (isTarget)
            {
                
                String *deployName = (String*)pDict->objectForKey("Deploy");
                bool deployOnly = pDict->objectForKey("DeployOnly") ? true : false;
                bool removeOnly = pDict->objectForKey("RemoveOnly") ? true : false;
                bool deployOnRemoveOff = pDict->objectForKey("DeployOnRemoveOff") ? true : false;
                bool isLowerObject = pDict->objectForKey("Lower") ? true : false;
                
                if (deployOnRemoveOff) {
                    deployOnly = isOn;
                    removeOnly = !isOn;
                }
                
                TMXLayer *deployLayer = isLowerObject ? _lowerObjectsLayer : _objectsLayer;
                
                int x = ((String*)pDict->objectForKey("x"))->intValue();
                int y = ((String*)pDict->objectForKey("y"))->intValue();
                Point targetCoord = this->getTileCoordForPosition(ccp(x, y));
                
                int currentTargetTileGID = deployLayer->tileGIDAt(targetCoord);
                
                // Removing the object
                if (!deployOnly && currentTargetTileGID) {
                    
                    deployLayer->removeTileAt(targetCoord);
                    
                    if (sound) {
                        SimpleAudioEngine::sharedEngine()->playEffect("sfx_deploy.wav");
                        sound = false;
                    }
                    
                    if (!isLowerObject) {
                        
                        Point playerTileCoord = _player->getCurrentTileCoord();
                        
                        if (playerTileCoord.x != targetCoord.x || playerTileCoord.y != targetCoord.y) {
                            this->triggerWeightSwitch(targetCoord, sound);
                        }
                    }
                    
                }
                
                // Deploying the object
                else if (!removeOnly && deployName) {
                    
                    int newTileGID = this->getTileGIDByProperty("DeployName", deployName->getCString());
                    deployLayer->setTileGID(newTileGID, targetCoord);
                    
                    if (sound) {
                        SimpleAudioEngine::sharedEngine()->playEffect("sfx_deploy.wav");
                        sound = false;
                    }
                    
                    Point playerTileCoord = _player->getCurrentTileCoord();
                    
                    bool isPlayerOnTarget = playerTileCoord.x == targetCoord.x && playerTileCoord.y == targetCoord.y;
                    
                    if (isPlayerOnTarget && this->checkTileProperty(newTileGID, "Damage"))
                        _player->die();
                    
                    if (isLowerObject && this->checkTileProperty(_objectsLayer->tileGIDAt(targetCoord), "Draggable")) {
                        
                        this->triggerWeightSwitch(targetCoord, sound);
                    
                    } else if (!isLowerObject) {
                        
                        if (isPlayerOnTarget) {
                            
                            Point pushTileCoord = _player->getFacingTileCoord(-1, playerTileCoord);
                            Point pushPosition = this->getCharacterPositionForTileCoord(pushTileCoord);
                            if (this->canWalkTo(pushPosition)) {
                                _player->getNode()->stopAllActions();
                                _player->getNode()->setPosition(pushPosition);
                                _player->finishWalk();
                            } else {
                                _player->die();
                                return;
                            }
                            
                        } else {
                            
                            this->triggerWeightSwitch(targetCoord, sound);
                            
                        }
                    }
                    
                }
                
            }
        }
    }
    
}

void Cave::triggerWeightSwitch(Point tileCoord, bool sound) {
    
    int lowerObjectTileGID = _lowerObjectsLayer->tileGIDAt(tileCoord);
    if (lowerObjectTileGID) {
        Dictionary *properties = _tiledMap->propertiesForGID(lowerObjectTileGID);
        
        String *switchType = (String *)properties->objectForKey("Switch");
        if (switchType) {
            
            bool isOn = true;
            String *switchState = (String *)properties->objectForKey(switchType->getCString());
            if (switchState) {
                
                isOn = switchState->compare("on") != 0;
                const char *newSwitchState = isOn ? "on" : "off";
                
                int newTileGID = this->getTileGIDByProperty(switchType->getCString(), newSwitchState);
                _lowerObjectsLayer->setTileGID(newTileGID, tileCoord);
                
                if (sound)
                    SimpleAudioEngine::sharedEngine()->playEffect("sfx_use_trigger.wav");
                
            }
            this->triggerActionTarget(tileCoord, sound, isOn);
        }
    }
    
}

void Cave::triggerWeightSwitch(Point tileCoord) {
    this->triggerWeightSwitch(tileCoord, true);
}

void Cave::decreaseTimeSwitchCount() {
    
    _activeTimeSwitchCount--;
    if (_activeTimeSwitchCount < 0) _activeTimeSwitchCount = 0;
    
    if (_activeTimeSwitchCount == 0)
        this->stopActionByTag(kTimeSwitchTicTacActionTag);
    
}

void Cave::timeSwitchTic() {
    SimpleAudioEngine::sharedEngine()->playEffect("sfx_tic.wav");
}

void Cave::timeSwitchTac() {
    SimpleAudioEngine::sharedEngine()->playEffect("sfx_tac.wav");
}

/*
void Cave::deactivateTimeSwitch(Node *node, PointObject *pointObject) {
    Point tileCoord = pointObject->getPoint();
    
    bool isOn = true;
    String *switchState = this->getTileProperty(objectsLayer, tileCoord, "Time");
    if (switchState) {
        
        isOn = switchState->compare("on") != 0;
        const char *newSwitchState = isOn ? "on" : "off";
        
        int newTileGID = this->getTileGIDByProperty("Time", newSwitchState);
        objectsLayer->setTileGID(newTileGID, tileCoord);
        
        SimpleAudioEngine::sharedEngine()->playEffect("sfx_use_trigger.wav");
        
        activeTimeSwitchCount--;
        if (activeTimeSwitchCount < 0) activeTimeSwitchCount = 0;
        
        if (activeTimeSwitchCount == 0)
            this->stopActionByTag(kTimeSwitchTicTacActionTag);
        
    }
    
    this->triggerActionTarget(tileCoord, true, isOn);
    
}
 */

void Cave::playLevelEndMusic() {
    
    SimpleAudioEngine::sharedEngine()->playEffect("sfx_levelend.wav");
    
}

void Cave::restartLevel() {
    
    Director::sharedDirector()->replaceScene(Cave::sceneWithLevel(_levelNumber));
    
}

void Cave::initiateLevelEnd() {
    
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    this->recursivelyPauseAllChildren(_mapMainLayer);
    //this->recursivelyPauseAllChildren(mainSpriteBatch);
    this->stopAllActions();
    
    Action *levelEndAction;
    
    int newLevelNumber = _levelNumber + 1;
    
    if (newLevelNumber <= kLastLevel) {
        levelEndAction = Sequence::create(
                                          DelayTime::create(0.2f),
                                          CallFunc::create(this, callfunc_selector(Cave::playLevelEndMusic)),
                                          DelayTime::create(1.26f),
                                          CallFunc::create(this, callfunc_selector(Cave::levelEnd)),
                                          NULL);
        
    } else {
        levelEndAction = Sequence::create(
                                          DelayTime::create(0.2f),
                                          CallFunc::create(this, callfunc_selector(Cave::levelEnd)),
                                          NULL);
    }
    
    this->runAction(levelEndAction);

}

void Cave::levelEnd() {
    
    int newLevelNumber = _levelNumber + 1;
    
    if (newLevelNumber <= kLastLevel) {
        Director::sharedDirector()->replaceScene(Cave::sceneWithLevel(newLevelNumber));
    } else {
        Director::sharedDirector()->replaceScene(GameEnd::scene());
    }
    
}