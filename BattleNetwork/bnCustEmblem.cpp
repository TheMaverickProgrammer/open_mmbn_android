#include "bnCustEmblem.h"

CustEmblem::CustEmblem() {
  numWires = 12;
  wireShader = &LOAD_SHADER(BADGE_WIRE);
  wireShader->setUniform("texture", sf::Shader::CurrentTexture);
  wireShader->setUniform("numOfWires", numWires);

  emblem.setTexture(LOAD_TEXTURE(CUST_BADGE));
  emblemWireMask.setTexture(LOAD_TEXTURE(CUST_BADGE_MASK));

  emblemWireMask.setPosition(-9.0f, -7.0f);
}

void CustEmblem::CreateWireEffect() {

  sf::Color mix = sf::Color::White;

  WireEffect w;
  w.color = sf::Color::White; // todo make random

  // Original concept was to have each wire operate independantly
  // Incorrect shader math made multiple happen at once which looked cooler
  // when index is equal to the following, the shader breaks and looks ugly. 
  // Skip those values.
  do {
    w.index = rand() % numWires;
  } while (w.index == 2 || w.index == 9 || w.index == 10);

  std::cout << "wire index: " << w.index << std::endl;

  w.progress = 0;

  coming.push_front(w);
}

void CustEmblem::UndoWireEffect() {
  bool found = false;

  sf::Color red = sf::Color(123, 239, 178, 255);

  if (coming.size() > 0) {
    // find a wire going forward and reverse it
    auto top = coming.front();
    coming.pop_front();

    top.color = red;
    top.progress = 1.0;

    leaving.push_front(top);
  }
}

void CustEmblem::Update(double elapsed) {
  for (auto iter = coming.begin(); iter != coming.end(); iter++) {
    iter->progress += elapsed * 2.0;
  }

  for (auto iter = leaving.begin(); iter != leaving.end();) {
    iter->progress -= elapsed * 2.0;

    if (iter->progress < 0.0) {
      iter = leaving.erase(iter);
      continue;
    }

    iter++;
  }
}

void CustEmblem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(emblem, states);

  states.shader = wireShader;

  std::deque<WireEffect> effects;
  effects.resize(coming.size() + leaving.size());
  effects.insert(effects.begin(), coming.begin(), coming.end());
  effects.insert(effects.begin(), leaving.begin(), leaving.end());

  for (auto& e : effects) {
    if (e.progress <= 1.0f) {
      wireShader->setUniform("progress", (float)e.progress);
      wireShader->setUniform("index", e.index);
      wireShader->setUniform("inColor", sf::Glsl::Vec4(e.color));
      target.draw(emblemWireMask, states);
    }
  }
}