#pragma once
#include "bnSpell.h"
#include "bnAnimationComponent.h"

class Buster : public Spell {
public:
  Buster(Field* _field, Team _team, bool _charged);
  virtual ~Buster(void);

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual void Attack(Character* _entity);
private:
  bool isCharged;
  bool spawnGuard;
  Character* contact;
  int damage;
  float cooldown;
  float random; // offset
};