#pragma once

#include <list>

#include "bnComponent.h"
#include "bnChip.h"
#include "bnChipUseListener.h"

class Character;

class ChipUsePublisher {
private:
  friend class ChipUseListener;

  std::list<ChipUseListener*> listeners;

  void AddListener(ChipUseListener* listener) {
    listeners.push_back(listener);
    std::cout << "listeners: " << listeners.size() << std::endl;
  }

public:
  virtual ~ChipUsePublisher();
  virtual void UseNextChip() = 0;

  void Broadcast(Chip& chip, Character& user) {
    std::list<ChipUseListener*>::iterator iter = listeners.begin();

    while (iter != listeners.end()) {
      (*iter)->OnChipUse(chip, user);
      iter++;
    }
  }
};