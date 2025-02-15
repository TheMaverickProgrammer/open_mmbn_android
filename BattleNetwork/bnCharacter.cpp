#include "bnCharacter.h"
#include "bnDefenseRule.h"
#include "bnSpell.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnExplosion.h"

Character::Character(Rank _rank) : 
  health(0),
  maxHealth(0),
  counterable(false),
  draggable(true),
  canShareTile(false),
  stunCooldown(0),
  name("unnamed"),
  rank(_rank),
  frameDamageTaken(0),
  frameElementalModifier(false),
  invokeDeletion(false),
  CounterHitPublisher() {
  burnCycle = sf::milliseconds(150);
  elapsedBurnTime = burnCycle.asSeconds();
  team = Team::UNKNOWN;
  isBattleActive = false;
  deleted = false;
  passthrough = false;
  ownedByField = false;
  isSliding = false;
  floatShoe = false;
  airShoe = false;
}

Character::~Character() {
}

const Character::Rank Character::GetRank() const {
  return rank;
}

void Character::ShareTileSpace(bool enabled)
{
  canShareTile = enabled;
}

const bool Character::CanShareTileSpace() const
{
  return this->canShareTile;
}

void Character::SetDraggable(bool enabled)
{
  this->draggable = enabled;
}

void Character::Update(float _elapsed) {
  elapsedBurnTime -= _elapsed;

  if (this->IsBattleActive() && !this->HasFloatShoe()) {
    if (this->GetTile()) {
      if (this->GetTile()->GetState() == TileState::POISON) {
        if (elapsedBurnTime <= 0) {
          if (this->Hit(Hit::Properties({ 1, 0x00, Element::NONE, 0, nullptr }))) {
            elapsedBurnTime = burnCycle.asSeconds();
          }
        }
      }
      else {
        elapsedBurnTime = 0;
      }

      if (this->GetTile()->GetState() == TileState::LAVA) {
        if (this->Hit(Hit::Properties({ 50, Hit::pierce, Element::FIRE, 0, nullptr }))) {
          Field* field = GetField();
          Artifact* explosion = new Explosion(field, this->GetTeam(), 1);
          field->AddEntity(*explosion, tile->GetX(), tile->GetY());
          this->GetTile()->SetState(TileState::NORMAL);
        }
      }
    }
  }

  if (this->GetHealth() <= 0 && this->isSliding) {
    this->isSliding = false;
  }

  Entity::Update(_elapsed);
}

bool Character::CanMoveTo(Battle::Tile * next)
{
  auto occupied = [this](Entity* in) {
    Character* c = dynamic_cast<Character*>(in);

    return c && c != this && !c->CanShareTileSpace();
  };

  return (Entity::CanMoveTo(next) && next->FindEntities(occupied).size() == 0);
}

void Character::AddAnimation(string _state, FrameList _frameList, float _duration) {
  assert(false && "AddAnimation shouldn't be called directly from Character");
}

void Character::SetAnimation(string _state) {
  assert(false && "SetAnimation shouldn't be called directly from Character");
}

void Character::SetCounterFrame(int frame)
{
  assert(false && "SetCounterFrame shouldn't be called directly from Character");
}

void Character::OnFrameCallback(int frame, std::function<void()> onEnter, std::function<void()> onLeave, bool doOnce)
{
  assert(false && "OnFrameCallback shouldn't be called directly from Character");
}

int Character::GetHealth() const {
  return health;
}

const int Character::GetMaxHealth() const
{
  return this->maxHealth;
}

const bool Character::Hit(Hit::Properties props) {
  this->frameHitProps |= props.flags;
  this->frameDamageTaken += props.damage;

  return (health != 0);
}

void Character::ResolveFrameBattleDamage()
{
  (health - this->frameDamageTaken < 0) ? this->SetHealth(0) : this->SetHealth(health - this->frameDamageTaken);

  if (this->IsCountered() && (this->frameHitProps & Hit::recoil) == Hit::recoil) {
    this->Stun(3.0);

    if (this->GetHealth() == 0) {
      // Slide entity back a few pixels
      this->tileOffset = sf::Vector2f(50.f, 0.0f);
    }

    //this->Broadcast(*this, *props.aggressor);
  }

  if (this->GetHealth() == 0 && !this->invokeDeletion) {
    this->OnDelete();
    this->invokeDeletion = true;
  }

  this->frameDamageTaken = 0;
  this->frameHitProps = Hit::none;
}

int* Character::GetAnimOffset() {
  return nullptr;
}

void Character::SetHealth(const int _health) {
  health = _health;

  if (maxHealth == 0) {
    maxHealth = health;
  }

  if (health < 0) health = 0;
  if (health > maxHealth) health = maxHealth;
}

void Character::AdoptTile(Battle::Tile * tile)
{
  tile->AddEntity(*this);

  if (!isSliding) {
    this->setPosition(tile->getPosition());
  }
}

void Character::TryDelete() {
  deleted = (health <= 0);
}

void Character::ToggleCounter(bool on)
{
  counterable = on;
}

void Character::Stun(double maxCooldown)
{
  stunCooldown = maxCooldown;
}

bool Character::IsCountered()
{
  return (counterable && stunCooldown <= 0);
}

void Character::SetName(std::string name)
{
  this->name = name;
}

const std::string Character::GetName() const
{
  return name;
}

void Character::AddDefenseRule(DefenseRule * rule)
{
  if (rule) {
    defenses.push_back(rule);
    std::sort(defenses.begin(), defenses.end(), [](DefenseRule* first, DefenseRule* second) { return first->GetPriorityLevel() < second->GetPriorityLevel(); });
  }
}

void Character::RemoveDefenseRule(DefenseRule * rule)
{
  auto iter = std::remove_if(defenses.begin(), defenses.end(), [&rule](DefenseRule * in) { return in == rule; });

  //if (iter != defenses.end()) {
    defenses.erase(iter);
  //}
}

const bool Character::CheckDefenses(Spell* in)
{
  for (int i = 0; i < defenses.size(); i++) {
    if (defenses[i]->Check(in, this)) {
      return true;
    }
  }

  return false;
}
