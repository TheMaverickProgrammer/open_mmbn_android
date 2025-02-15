#pragma once
#include "bnTile.h"
#include "bnField.h"
class ProgsMan;

class ProgsManThrowState : public AIState<ProgsMan>
{
private:
    Battle::Tile* lastTargetPos;

public:
  ProgsManThrowState();
  ~ProgsManThrowState();

  void OnEnter(ProgsMan& p);
  void OnUpdate(float _elapsed, ProgsMan& p);
  void OnLeave(ProgsMan& p);
};

