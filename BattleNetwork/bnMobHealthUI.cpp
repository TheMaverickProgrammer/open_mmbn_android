#include <string>
using std::to_string;

#include <Swoosh/Game.h>
#include "bnBattleScene.h"
#include "bnMobHealthUI.h"
#include "bnCharacter.h"
#include "bnTextureResourceManager.h"
#include "bnLogger.h"

MobHealthUI::MobHealthUI(Character* _mob)
  : mob(_mob), UIComponent(_mob) {
  healthCounter = mob->GetHealth();
  loaded = false;
  cooldown = 0;
  color = sf::Color::White;
  glyphs.setTexture(LOAD_TEXTURE(ENEMY_HP_NUMSET));
  glyphs.setScale(2.f, 2.f);
}

MobHealthUI::~MobHealthUI(void) {
}

/*
HP drop is not 1 unit per frame. It is:
10 per frame if difference is 100 or more
~5 per frame if difference is 99-40 range
-3 per frame for anything lower
*/
void MobHealthUI::Update(float elapsed) {
  if (mob) {

    if (cooldown <= 0) { cooldown = 0; }
    else { cooldown -= elapsed; }
   
    if (healthCounter > mob->GetHealth()) {
      int diff = healthCounter - mob->GetHealth();

      if (diff >= 100) {
        healthCounter -= 10;
      }
      else if (diff >= 40) {
        healthCounter -= 5;
      }
      else if (diff >= 3) {
        healthCounter -= 3;
      }
      else {
        healthCounter--;
      }
  
      cooldown = 0.5; //seconds
    }
    else if (healthCounter < mob->GetHealth()) {
      healthCounter++;
      color = sf::Color(0, 255, 80);
    }
    else {
      color = sf::Color::White;
    }

    if (cooldown > 0) {
      color = sf::Color(255, 165, 0);
    }

    if (healthCounter < 0 || mob->GetHealth() <= 0) { healthCounter = 0; }
  }
}

void MobHealthUI::Inject(BattleScene & scene)
{
  // Todo: add this free step to inject step? It's manadatory. No sense repeating this every time
  GetOwner()->FreeComponentByID(this->GetID()); // We are owned by the scene now 
  scene.Inject(*this);
}

void MobHealthUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
  auto this_states = states;
  this_states.transform *= this->getTransform();

  // Glyphs are 8x10
  // First glyph is 9 the last is 0
  // There's 1px space between the glyphs

  if (healthCounter > 0) {
    int size = (int)(std::to_string(healthCounter).size());
    int hp = healthCounter;
    float offsetx = -(((size)*8.0f) / 2.0f)*glyphs.getScale().x;
    int index = 0;
    while (index < size) {
      const char c = std::to_string(healthCounter)[index];
      int number = std::atoi(&c);

      int row = (10-number-1);
      int rowStart = row + (row * 10);

      glyphs.setTextureRect(sf::IntRect(0, rowStart, 8, 10));
      glyphs.setPosition(sf::Vector2f(offsetx, 0.0f) + this->mob->getPosition());
      glyphs.setColor(this->color);

      target.draw(glyphs, this_states);
      //ENGINE.Draw(font);

      offsetx += 8.0f*glyphs.getScale().x;
      index++;
    }
  }

  UIComponent::draw(target, states);
}
