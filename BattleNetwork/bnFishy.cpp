#include "bnFishy.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

Fishy::Fishy(Field* _field, Team _team, double speed) : Obstacle(field, team) {
  SetLayer(0);
  field = _field;
  direction = Direction::NONE;
  deleted = false;
  hit = false;
  health = 1;
  texture = TEXTURES.LoadTextureFromFile("resources/spells/fishy_temp.png");

  // why do we need this??
  this->team = _team;

  this->speed = speed;

  this->slideTime = sf::seconds(0.1f);

  AUDIO.Play(AudioType::TOSS_ITEM_LITE, AudioPriority::LOWEST);

  Hit::Properties props;
  props.damage = 80;
  props.flags = Hit::recoil | Hit::flinch;
  props.secs = 3;
  this->SetHitboxProperties(props);

  EnableTileHighlight(false);
  this->SetFloatShoe(true);
}

Fishy::~Fishy(void) {
}

void Fishy::Update(float _elapsed) {
  setTexture(*texture);
  setScale(2.f, 2.f);
  setPosition(tile->getPosition().x + tileOffset.x - 40.0f, tile->getPosition().y + tileOffset.y - 120.0f);

  if (this->GetTile()->GetX() == 6) {
    this->Delete();
  }

  // Keep moving
  if (!this->isSliding) {
    this->SlideToTile(true);
    this->Move(this->GetDirection());
  }

  tile->AffectEntities(this);

  Entity::Update(_elapsed);
}

bool Fishy::CanMoveTo(Battle::Tile* tile) {
  return true;
}


const bool Fishy::Hit(Hit::Properties props) {
  return true; // fishy blocks everything
}

void Fishy::Attack(Character* _entity) {
  std::cout << "fishy team: " << (int)this->GetTeam() << std::endl;

  if (!hit) {

    hit = _entity->Hit(this->GetHitboxProperties());
    if (hit) {
      // end this
      this->Delete();
    }
  }
}