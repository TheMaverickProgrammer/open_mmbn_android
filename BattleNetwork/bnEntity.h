#pragma once
#include <string>
#include <vector>
using std::string;

#include "bnAnimation.h"
#include "bnDirection.h"
#include "bnTeam.h"
#include "bnMemory.h"
#include "bnEngine.h"
#include "bnTextureType.h"
#include "bnElements.h"

namespace Battle {
  class Tile;
}

class Field;
class Character;   // forward decl
class Component;   // forward decl
class BattleScene; // forward decl

class Entity : public LayeredDrawable {
  friend class Field;
  friend class Component;
  friend class BattleScene;

private:
  long ID; // used for tagging and later, in scripting
  static long numOfIDs;
  int alpha;
  long lastComponentID; // To run through scene injection later

public:
  Entity();
  virtual ~Entity();

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual bool Teleport(int x, int y);
  virtual bool CanMoveTo(Battle::Tile* next);

  const long GetID() const;

  bool Teammate(Team _team);

  void SetTile(Battle::Tile* _tile);
  Battle::Tile* GetTile() const;
  const Battle::Tile * GetNextTile() const;
  void SlideToTile(bool );
  const bool IsSliding() const;

  void SetField(Field* _field);

  Field* GetField() const;

  Team GetTeam() const;
  void SetTeam(Team _team);

  void SetPassthrough(bool state);
  bool IsPassthrough();

  void SetAlpha(int value);

  void SetFloatShoe(bool state);
  void SetAirShoe(bool state);
  bool HasFloatShoe();
  bool HasAirShoe();

  void SetDirection(Direction direction);
  Direction GetDirection();
  Direction GetPreviousDirection();

  void Delete();
  bool IsDeleted() const;

  void SetElement(Element _elem);
  const Element GetElement() const;

  void AdoptNextTile();
  virtual void AdoptTile(Battle::Tile* tile) = 0;

  void SetBattleActive(bool state);
  const bool IsBattleActive();

  // TODO: GetComponents<>() and GetFirstComponent<>()
  template<typename Type>
  Type* GetComponent();

  Component* RegisterComponent(Component* c);
  void FreeAllComponents();
  void FreeComponentByID(long ID);

protected:
  // used to toggle some effects inbetween paused scene moments
  bool isBattleActive; 

  bool ownedByField;
  Battle::Tile* next;
  Battle::Tile* tile;
  Battle::Tile* previous;
  sf::Vector2f tileOffset;
  sf::Vector2f slideStartPosition;
  Field* field;
  Team team;
  Element element;
  bool passthrough;
  bool floatShoe;
  bool airShoe;
  bool isSliding; // If sliding/gliding to a tile
  bool deleted;
  int moveCount;
  sf::Time slideTime; // how long slide behavior lasts
  sf::Time defaultSlideTime;
  double elapsedSlideTime; // in seconds
  Direction direction;
  Direction previousDirection;

  std::vector<Component*> shared;

private:
  void UpdateSlideStartPosition();
};

// todo use typid for exact types
template<typename Type>
inline Type* Entity::GetComponent()
{
  for (vector<Component*>::iterator it = shared.begin(); it != shared.end(); ++it) {
    Type* to_type = dynamic_cast<Type*>(*it);

    if (to_type != nullptr) {
      return to_type;
    }
  }

  return nullptr;
}

