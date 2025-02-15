#include "bnWave.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnSharedHitBox.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

Wave::Wave(Field* _field, Team _team, double speed) : Spell() {
  SetLayer(0);
  field = _field;
  team = _team;
  direction = Direction::NONE;
  deleted = false;
  hit = false;
  texture = TEXTURES.GetTexture(TextureType::SPELL_WAVE);
  this->speed = speed;

  //Components setup and load
  auto onFinish = [this]() {
    if (Move(direction)) {
      AUDIO.Play(AudioType::WAVE);
    }
  };

  animation = Animation("resources/spells/spell_wave.animation");
  animation.SetAnimation("DEFAULT");
  animation << Animate::Mode::Loop << onFinish;

  auto props = Hit::DefaultProperties;
  props.damage = 10;
  this->SetHitboxProperties(props);

  AUDIO.Play(AudioType::WAVE);

  EnableTileHighlight(true);
}

Wave::~Wave(void) {
}

void Wave::Update(float _elapsed) {
  setTexture(*texture);

  int lr = (this->GetDirection() == Direction::LEFT) ? 1 : -1;
  setScale(2.f*(float)lr, 2.f);

  setPosition(tile->getPosition().x, tile->getPosition().y);

  animation.Update(_elapsed*this->speed, *this);

  if (!this->IsDeleted()) {
    tile->AffectEntities(this);
  }

  Entity::Update(_elapsed);
}

bool Wave::Move(Direction _direction) {
  // Drop a shared hitbox when moving
  //SharedHitBox* shb = new SharedHitBox(this, 1.0f/60.0f);
  //GetField()->AddEntity(*shb, tile->GetX(), tile->GetY());
  
  tile->RemoveEntityByID(this->GetID());
  Battle::Tile* next = nullptr;

  if (_direction == Direction::LEFT) {
    if (tile->GetX() - 1 > 0) {
      next = field->GetAt(tile->GetX() - 1, tile->GetY());
    }
  } else if (_direction == Direction::RIGHT) {
    if (tile->GetX() + 1 <= (int)field->GetWidth()) {
      next = field->GetAt(tile->GetX() + 1, tile->GetY());
    }
  }

  if (next && next->IsWalkable()) {
    next->AddEntity(*this);
    
    return true;
  }

  tile->RemoveEntityByID(this->GetID());
  this->Delete();
  return false;
}

void Wave::Attack(Character* _entity) {
  _entity->Hit(GetHitboxProperties());
}
