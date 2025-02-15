#pragma once
#include "bnArtifact.h"
#include "bnComponent.h"
#include "bnField.h"

class BubbleTrap : virtual public Artifact, virtual public Component
{
private:
  Animation animation;
  sf::Sprite bubble;
  double duration;
  DefenseRule* defense;
public:
  BubbleTrap(Character* owner);
  ~BubbleTrap();

  virtual void Inject(BattleScene&);
  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction) { return false; }

  void Pop();
};
