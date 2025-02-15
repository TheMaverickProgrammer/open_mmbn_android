#include "bnMetalManBossFight.h"
#include "bnMetalMan.h"
#include "bnBattleItem.h"
#include "bnStringEncoder.h"
#include "bnChip.h"
#include "bnField.h"
#include "bnTile.h"
#include "bnSpawnPolicy.h"
#include "bnGear.h"
#include "bnBattleOverTrigger.h"

#include "bnUndernetBackground.h"

MetalManBossFight::MetalManBossFight(Field* field) : MobFactory(field)
{
}


MetalManBossFight::~MetalManBossFight()
{
}

Mob* MetalManBossFight::Build() {
  Mob* mob = new Mob(field);
  mob->SetBackground(new UndernetBackground());
  mob->StreamCustomMusic("resources/loops/loop_boss_battle.ogg");

  mob->RegisterRankedReward(1, BattleItem(Chip(100, 139, 'C', 0, Element::NONE, "RockCube", "Places a cube in front", "", 0)));
  mob->RegisterRankedReward(5, BattleItem(Chip(100, 139, '*', 0, Element::NONE, "RockCube", "Places a cube in front", "", 0)));

  field->AddEntity(*new Gear(field, Team::BLUE, Direction::LEFT), 3, 2);
  field->AddEntity(*new Gear(field, Team::BLUE, Direction::RIGHT), 4, 2);

  mob->Spawn<Rank1<MetalMan, MetalManIdleState>>(6, 2);

  mob->ToggleBossFlag();

  for (int i = 0; i < field->GetWidth(); i++) {
    for (int j = 0; j < field->GetHeight(); j++) {
      Battle::Tile* tile = field->GetAt(i + 1, j + 1);
    }
  }

  return mob;
}
