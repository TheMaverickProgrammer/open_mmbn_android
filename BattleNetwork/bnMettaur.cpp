#include "bnMettaur.h"
#include "bnMettaurIdleState.h"
#include "bnMettaurAttackState.h"
#include "bnMettaurMoveState.h"
#include "bnExplodeState.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnWave.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"

#define RESOURCE_NAME "mettaur"
#define RESOURCE_PATH "resources/mobs/mettaur/mettaur.animation"

vector<int> Mettaur::metIDs = vector<int>();
int Mettaur::currMetIndex = 0;

Mettaur::Mettaur(Rank _rank)
  :  AI<Mettaur>(this), AnimatedCharacter(_rank) {
  name = "Mettaur";
  Entity::team = Team::BLUE;

  textureType = TextureType::MOB_METTAUR;

  animationComponent.Setup(RESOURCE_PATH);
  animationComponent.Reload();

  if (GetRank() == Rank::SP) {
    SetHealth(200);
    animationComponent.SetPlaybackSpeed(1.2);
    animationComponent.SetAnimation("SP_IDLE");
  }
  else {
	  SetHealth(40);
    //Components setup and load
    animationComponent.SetAnimation("IDLE");
  }

  hitHeight = 0;

  setTexture(*TEXTURES.GetTexture(textureType));
  setScale(2.f, 2.f);

  this->SetHealth(health);

  whiteout = SHADERS.GetShader(ShaderType::WHITE);
  stun = SHADERS.GetShader(ShaderType::YELLOW);

  metID = (int)Mettaur::metIDs.size();
  Mettaur::metIDs.push_back((int)Mettaur::metIDs.size());

  animationComponent.Update(0);
}

Mettaur::~Mettaur() {

}

int* Mettaur::GetAnimOffset() {
  Mettaur* mob = this;

  int* res = new int[2];
  res[0] = 10;  res[1] = 0;

  return res;
}

void Mettaur::Update(float _elapsed) {
  this->SetShader(nullptr);
  this->RefreshTexture();

  if (stunCooldown > 0) {
    stunCooldown -= _elapsed;
    Character::Update(_elapsed);

    if (stunCooldown <= 0) {
      stunCooldown = 0;
      animationComponent.Update(_elapsed);
    }

    if ((((int)(stunCooldown * 15))) % 2 == 0) {
      this->SetShader(stun);
    }

    if (GetHealth() > 0) {
      return;
    }
  }

  this->AI<Mettaur>::Update(_elapsed);

  // Explode if health depleted
  if (GetHealth() <= 0) {
    this->ChangeState<ExplodeState<Mettaur>>();
    
    if (Mettaur::metIDs.size() > 0) {
      vector<int>::iterator it = find(Mettaur::metIDs.begin(), Mettaur::metIDs.end(), metID);

      if (it != Mettaur::metIDs.end()) {
        // Remove this mettaur out of rotation...
        Mettaur::currMetIndex++;

        Mettaur::metIDs.erase(it);
        if (Mettaur::currMetIndex >= Mettaur::metIDs.size()) {
          Mettaur::currMetIndex = 0;
        }
      }
    }

    this->LockState();
  } else {
    animationComponent.Update(_elapsed);
  }

  Character::Update(_elapsed);
}

void Mettaur::RefreshTexture() {
  setPosition(tile->getPosition().x, tile->getPosition().y);

  setPosition(getPosition() + tileOffset);
}

/*void Mettaur::SetAnimation(string _state, std::function<void()> onFinish) {
  state = _state;
  animationComponent.SetAnimation(_state, onFinish);
  animationComponent.Update(0);
}

void Mettaur::SetCounterFrame(int frame)
{
  auto onFinish = [&]() { this->ToggleCounter(); };
  auto onNext = [&]() { this->ToggleCounter(false); };
  animationComponent.AddCallback(frame, onFinish, onNext);
}*/

TextureType Mettaur::GetTextureType() const {
  return textureType;
}

int Mettaur::GetHealth() const {
  return health;
}

void Mettaur::SetHealth(int _health) {
  health = _health;
}

const bool Mettaur::Hit(Hit::Properties props) {
  /*if (Character::Hit(_damage, props)) {
    SetShader(whiteout);
    return true;
  }

  return false;*/

  bool result = true;

  if (health - props.damage < 0) {
    health = 0;
  }
  else {
    health -= props.damage;

    if ((props.flags & Hit::stun) == Hit::stun) {
      SetShader(stun);
      this->stunCooldown = props.secs;
    }
    else {
      SetShader(whiteout);
    }
  }

  return result;
}

const float Mettaur::GetHitHeight() const {
  return hitHeight;
}

const bool Mettaur::IsMettaurTurn() const
{
  if(Mettaur::metIDs.size() > 0)
    return (Mettaur::metIDs.at(Mettaur::currMetIndex) == this->metID);

  return false;
}

void Mettaur::NextMettaurTurn() {
  Mettaur::currMetIndex++;

  if (Mettaur::currMetIndex >= Mettaur::metIDs.size()) {
    Mettaur::currMetIndex = 0;
  }
}
