#pragma once
#include "bnSpell.h"
#include "bnAnimate.h"

class Cannon : public Spell {
public:
  Cannon(Field* _field, Team _team, int damage);
  virtual ~Cannon(void);

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual void Attack(Character* _entity);

private:
  int damage;
  int random;
  float cooldown;
  FrameList animation;
  Animate animator;
};