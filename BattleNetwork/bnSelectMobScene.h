#pragma once

#include "bnMobRegistration.h"
#include "bnNaviRegistration.h"
#include "bnTextBox.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnStarman.h"
#include "bnMob.h"
#include "bnMemory.h"
#include "bnCamera.h"
#include "bnInputManager.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"
#include "bnBattleScene.h"
#include "bnMobFactory.h"
#include "bnRandomMettaurMob.h"
#include "bnProgsManBossFight.h"
#include "bnTwoMettaurMob.h"
#include "bnCanodumbMob.h"

#include <time.h>
#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Clock;
using sf::Event;
using sf::Font;

#include <Swoosh/Activity.h>
#include "Segues/CrossZoom.h" // <-- GPU intensive and runs slowly on old hardware
#include "Segues/WhiteWashFade.h"
#include "Segues/BlackWashFade.h"

class SelectMobScene : public swoosh::Activity
{
private:
  SelectedNavi selectedNavi;

  Camera camera;
  ChipFolder& selectedFolder;

  Mob* mob;

  // Menu name font
  sf::Font* font;
  sf::Text* menuLabel;

  // Selection input delays
  double maxSelectInputCooldown;
  double selectInputCooldown;
  double elapsed; // time

  // MOB UI font
  sf::Font *mobFont;
  sf::Text *mobLabel;
  sf::Text *attackLabel;
  sf::Text *speedLabel;
  sf::Text *hpLabel;

  float maxNumberCooldown;
  float numberCooldown;

  bool doOnce; // scene effects
  bool showMob; // toggle hide / show mob

  // select menu graphic
  sf::Sprite bg;

  // Current mob graphic
  sf::Sprite mobSpr;

  // bobbing cursors
  sf::Sprite cursor;

  sf::Sprite navigator;
  Animation navigatorAnimator;

  bool gotoNextScene;

  SmartShader shader;
  float factor;

  // Current selection index
  int mobSelectionIndex;

  // Text box navigator
  TextBox textbox;

public:
  SelectMobScene(swoosh::ActivityController&, SelectedNavi, ChipFolder& selectedFolder);
  ~SelectMobScene();

  virtual void onResume();
  virtual void onUpdate(double elapsed);
  virtual void onDraw(sf::RenderTexture& surface);
  virtual void onStart();
  virtual void onLeave();
  virtual void onExit();
  virtual void onEnter();
  virtual void onEnd();
};

