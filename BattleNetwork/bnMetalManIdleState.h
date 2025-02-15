#pragma once
#include "bnAIState.h"
class MetalMan;

class MetalManIdleState : public AIState<MetalMan>
{
private:
  float cooldown;
public:
  MetalManIdleState(float cooldown = 0.8f);
  ~MetalManIdleState();

  void OnEnter(MetalMan& p);
  void OnUpdate(float _elapsed, MetalMan& p);
  void OnLeave(MetalMan& p);
};

#pragma once
