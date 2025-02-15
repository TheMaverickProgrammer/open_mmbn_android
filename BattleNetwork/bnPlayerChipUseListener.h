#pragma once

#include "bnAudioResourceManager.h"
#include "bnChipUseListener.h"
#include "bnPlayer.h"
#include "bnFishy.h"
#include "bnTile.h"
#include "bnAirShot.h"
#include "bnCannon.h"
#include "bnBasicSword.h"
#include "bnThunder.h"
#include "bnInvis.h"
#include "bnElecpulse.h"
#include "bnReflectShield.h"
#include "bnHideUntil.h"

class PlayerChipUseListener : public ChipUseListener {
private:
  Player * player;

public:
  PlayerChipUseListener(Player* _player) : ChipUseListener() { player = _player; }
  PlayerChipUseListener(Player& _player) : ChipUseListener() { player = &_player;  }

  void OnChipUse(Chip& chip, Character& character) {
    player->SetCharging(false);

    std::string name = chip.GetShortName();

    if (name.substr(0, 5) == "Recov") {
      player->SetHealth(player->GetHealth() + chip.GetDamage());
      AUDIO.Play(AudioType::RECOVER);

      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };

      player->SetAnimation(PLAYER_HEAL, onFinish);

    }
    else if (name == "CrckPanel") {
      Battle::Tile* top = player->GetField()->GetAt(player->GetTile()->GetX() + 1, 1);
      Battle::Tile* mid = player->GetField()->GetAt(player->GetTile()->GetX() + 1, 2);
      Battle::Tile* low = player->GetField()->GetAt(player->GetTile()->GetX() + 1, 3);

      if (top) { top->SetState(TileState::CRACKED); }
      if (mid) { mid->SetState(TileState::CRACKED); }
      if (low) { low->SetState(TileState::CRACKED); }

      AUDIO.Play(AudioType::PANEL_CRACK);
    }
    else if (name == "Invis") {
      Component* invis = new Invis(player);
      player->RegisterComponent(invis);
    }
    else if (name == "Rflector") {
      ReflectShield* reflect = new ReflectShield(player);
      player->RegisterComponent(reflect);

      AUDIO.Play(AudioType::APPEAR);

      Battle::Tile* tile = player->GetTile();

      if (tile) {
        this->player->GetField()->AddEntity(*reflect, tile->GetX(), tile->GetY());
      }
    }
    else if (name == "Fishy") {
      Fishy* fishy = new Fishy(player->GetField(), player->GetTeam(), 1.0);
      fishy->SetDirection(Direction::RIGHT);
      HideUntil::Callback until = [fishy]() { return fishy->IsDeleted(); };
      HideUntil* fishyStatus = new HideUntil(player, until);
      player->RegisterComponent(fishyStatus);

      Battle::Tile* tile = player->GetTile();

      if (tile) {
        this->player->GetField()->AddEntity(*fishy, tile->GetX(), tile->GetY());
      }
    }
    else if (name == "XtrmeCnnon") {
      Cannon* xtreme1 = new Cannon(player->GetField(), player->GetTeam(), chip.GetDamage());
      Cannon* xtreme2 = new Cannon(player->GetField(), player->GetTeam(), chip.GetDamage());
      Cannon* xtreme3 = new Cannon(player->GetField(), player->GetTeam(), chip.GetDamage());


      ENGINE.GetCamera()->ShakeCamera(25, sf::seconds(1));

      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };
      player->SetAnimation(PLAYER_CANNON, onFinish);
      AUDIO.Play(AudioType::CANNON);

      xtreme1->SetDirection(Direction::RIGHT);
      xtreme2->SetDirection(Direction::RIGHT);
      xtreme3->SetDirection(Direction::RIGHT);

      player->GetField()->AddEntity(*xtreme1, player->GetTile()->GetX(), 1);
      player->GetField()->AddEntity(*xtreme2, player->GetTile()->GetX(), 2);
      player->GetField()->AddEntity(*xtreme3, player->GetTile()->GetX(), 3);
    }
    else if (name == "Cannon") {
      Cannon* cannon = new Cannon(player->GetField(), player->GetTeam(), chip.GetDamage());
      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };
      player->SetAnimation(PLAYER_CANNON, onFinish);
      AUDIO.Play(AudioType::CANNON);

      cannon->SetDirection(Direction::RIGHT);

      player->GetField()->AddEntity(*cannon, player->GetTile()->GetX() + 1, player->GetTile()->GetY());
    }
    else if (name == "Swrd") {
      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };

      player->SetAnimation(PLAYER_SWORD, onFinish);

      BasicSword* sword = new BasicSword(player->GetField(), player->GetTeam(), chip.GetDamage());

      AUDIO.Play(AudioType::SWORD_SWING);

      player->GetField()->AddEntity(*sword, player->GetTile()->GetX() + 1, player->GetTile()->GetY());
    }
    else if (name == "Elecplse") {
      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };

      player->SetAnimation(PLAYER_SHOOTING, onFinish);

      Elecpulse* pulse = new Elecpulse(player->GetField(), player->GetTeam(), chip.GetDamage());
      Elecpulse* pulse2 = new Elecpulse(player->GetField(), player->GetTeam(), chip.GetDamage());
      pulse2->setScale(0, 0);

      AUDIO.Play(AudioType::ELECPULSE);

      player->GetField()->AddEntity(*pulse, player->GetTile()->GetX() + 1, player->GetTile()->GetY());
      player->GetField()->AddEntity(*pulse2, player->GetTile()->GetX() + 2, player->GetTile()->GetY());

    }
    else if (name == "LongSwrd") {
      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };

      player->SetAnimation(PLAYER_SWORD, onFinish);

      BasicSword* sword = new BasicSword(player->GetField(), player->GetTeam(), chip.GetDamage());
      BasicSword* sword2 = new BasicSword(player->GetField(), player->GetTeam(), chip.GetDamage());

      AUDIO.Play(AudioType::SWORD_SWING);

      if (player->GetField()->GetAt(player->GetTile()->GetX() + 1, player->GetTile()->GetY())) {
        player->GetField()->AddEntity(*sword, player->GetTile()->GetX() + 1, player->GetTile()->GetY());
      }

      if (player->GetField()->GetAt(player->GetTile()->GetX() + 2, player->GetTile()->GetY())) {
        player->GetField()->AddEntity(*sword2, player->GetTile()->GetX() + 2, player->GetTile()->GetY());
      }
    }
    else if (name == "WideSwrd") {
      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };

      player->SetAnimation(PLAYER_SWORD, onFinish);

      BasicSword* sword = new BasicSword(player->GetField(), player->GetTeam(), chip.GetDamage());
      BasicSword* sword2 = new BasicSword(player->GetField(), player->GetTeam(), chip.GetDamage());

      AUDIO.Play(AudioType::SWORD_SWING);

      if (player->GetField()->GetAt(player->GetTile()->GetX() + 1, player->GetTile()->GetY())) {
        player->GetField()->AddEntity(*sword, player->GetTile()->GetX() + 1, player->GetTile()->GetY());
      }

      if (player->GetField()->GetAt(player->GetTile()->GetX() + 1, player->GetTile()->GetY() + 1)) {
        player->GetField()->AddEntity(*sword2, player->GetTile()->GetX() + 1, player->GetTile()->GetY() + 1);
      }
    }
    else if (name == "AirShot1") {
      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };

      player->SetAnimation(PLAYER_SHOOTING, onFinish);

      AUDIO.Play(AudioType::SPREADER);

      AirShot* airshot = new AirShot(player->GetField(), player->GetTeam(), chip.GetDamage());
      airshot->SetDirection(Direction::RIGHT);

      player->GetField()->AddEntity(*airshot, player->GetTile()->GetX() + 1, player->GetTile()->GetY());
    }
    else if (name == "Thunder") {
      auto onFinish = [this]() { this->player->SetAnimation(PLAYER_IDLE);  };

      player->SetAnimation(PLAYER_SHOOTING, onFinish);

      //AUDIO.Play(AudioType::);

      Thunder* thunder = new Thunder(player->GetField(), player->GetTeam());
      player->GetField()->AddEntity(*thunder, player->GetTile()->GetX() + 1, player->GetTile()->GetY());
    }
  }
};
