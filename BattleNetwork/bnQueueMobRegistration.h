#pragma once
#include "bnMobRegistration.h"

// Register these mobs
#include "bnTwoMettaurMob.h"
#include "bnCanodumbMob.h"
#include "bnStarfishMob.h"
#include "bnMetalManBossFight.h"
#include "bnMetalManBossFight2.h"
#include "bnRandomMettaurMob.h"

/***********************************************************************
************    Register your custom mobs here    *********************
************************************************************************/

void QueueMobRegistration() {
  auto info = MOBS.AddClass<TwoMettaurMob>();  // Create and register mob info object
  info->SetDescription("Tutorial ranked mettaurs, you got this!"); // Set property
  info->SetPlaceholderTexturePath("resources/mobs/mettaur/preview.png");
  info->SetName("Mettaurs");
  info->SetSpeed(1);
  info->SetAttack(10);
  info->SetHP(80);

  info = MOBS.AddClass<StarfishMob>();  // Create and register mob info object
  info->SetDescription("Starfish can trap you in bubbles"); // Set property
  info->SetName("Bubble Battle");
  info->SetPlaceholderTexturePath("resources/mobs/starfish/preview.png");
  info->SetSpeed(0);
  info->SetAttack(20);
  info->SetHP(100);

  info = MOBS.AddClass<CanodumbMob>();  // Create and register mob info object
  info->SetDescription("Family of cannon virii - Watch out!"); // Set property
  info->SetName("Triple Trouble");
  info->SetPlaceholderTexturePath("resources/mobs/canodumb/preview.png");
  info->SetSpeed(0);
  info->SetAttack(20);
  info->SetHP(500);

  info = MOBS.AddClass<ProgsManBossFight>();  // Create and register mob info object
  info->SetDescription("A rogue Mr.Prog! Can you stop it?"); // Set property
  info->SetName("Enter ProgsMan");
  info->SetPlaceholderTexturePath("resources/mobs/progsman/preview.png");
  info->SetSpeed(5);
  info->SetAttack(20);
  info->SetHP(600);

  info = MOBS.AddClass<MetalManBossFight>();  // Create and register mob info object
  info->SetDescription("MetalMan throws blades, shoots missiles, and can shatter the ground."); // Set property
  info->SetName("BN4 MetalMan");
  info->SetPlaceholderTexturePath("resources/mobs/metalman/preview.png");
  info->SetSpeed(6);
  info->SetAttack(20);
  info->SetHP(1000);

  info = MOBS.AddClass<RandomMettaurMob>();  // Create and register mob info object
  info->SetDescription("Randomly generated battle. Anything goes."); // Set property
  info->SetName("Random");
  info->SetPlaceholderTexturePath("resources/mobs/select/random.png");
  info->SetSpeed(999);
  info->SetAttack(999);
  info->SetHP(999);

  info = MOBS.AddClass<MetalManBossFight2>();  // Create and register mob info object
  info->SetDescription("MetalMan - On ice!"); // Set property
  info->SetName("Vengence Served Cold");
  info->SetPlaceholderTexturePath("resources/mobs/metalman/preview2.png");
  info->SetSpeed(6);
  info->SetAttack(20);
  info->SetHP(1000);
}