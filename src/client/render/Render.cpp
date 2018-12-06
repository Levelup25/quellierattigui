#include "Render.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Sprites.h"
#include "View.h"
#include "Window.h"
#include "WindowManager.h"
#include "customElements/CharacterStat.h"
#include "renderTest.h"

using namespace std;
using namespace sf;
using namespace state;
using namespace render;
using namespace engine;

Render::Render(State* state, Engine* engine, int nb, int l, int h) {
  this->state = state;
  this->engine = engine;
  this->nb = nb;
  this->l = nb * l;
  this->h = nb * h;
  n = state->getN();
  m = state->getM();
}

void Render::drawMap(RenderWindow& window, render::View& v, Sprites& sprites) {
  sf::View view = v.view;
  sf::Sprite sprite;
  RectangleShape zone(Vector2f(l, h));
  zone.setFillColor(Color::Transparent);
  zone.setOutlineThickness(-1);
  zone.setOutlineColor(Color::Black);
  window.setView(view);
  for (unsigned int j = yv; j < yv + m; j++) {
    for (unsigned int i = xv; i < xv + n; i++) {
      Cell* cell = state->getCell(i, j);
      ElementType element = cell->getElement();
      ContentType content = cell->getContent();

      sprite = sprites.getTileSprite(l, h, (int)element);
      sprite.setPosition(Vector2f(i * l, j * h));
      window.draw(sprite);

      if ((int)content > 1) {
        sprite = sprites.getContentSprite(l, h, (int)content, (int)element);
        sprite.setPosition(Vector2f(i * l, j * h));
        window.draw(sprite);
      }
      zone.setPosition(Vector2f(i * l, j * h));
      window.draw(zone);
    }
  }
}

void Render::drawZones(RenderWindow& window, render::View& v) {
  sf::View view = v.view;
  Vector2f mouse =
      window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
  int X = mouse.x / l, Y = mouse.y / h;
  RectangleShape zone(Vector2f(l, h));
  window.setView(view);
  if (state->isFighting() && state->getFight()->getTurn() % 2 == 1 &&
      v.getChildren().size() == 0) {
    if (state->etatCombat == 0) {
      vector<vector<int>> moves =
          (new MoveCommands(state, engine, selectedcharacter, X, Y))->getPath();
      zone.setFillColor(Color(0, 255, 0, 128));
      if ((int)moves.size() <= selectedcharacter->getPmCurrent()) {
        for (vector<int> coord : moves) {
          zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
          window.draw(zone);
        }
      }
    } else if (state->etatCombat == 1) {
      AttackCommand* atkcmd =
          new AttackCommand(state, engine, selectedcharacter,
                            {X - (int)selectedcharacter->getI(),
                             Y - (int)selectedcharacter->getJ()},
                            abilityNumber);
      atkcmd->setZones();
      vector<vector<int>> targets = atkcmd->getZone(0);
      zone.setFillColor(Color(0, 0, 255, 128));
      for (vector<int> coord : targets) {
        zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
        window.draw(zone);
      }
      vector<vector<int>> effects = atkcmd->getZone(1);
      zone.setFillColor(Color(255, 0, 0, 128));
      for (vector<int> coord : effects) {
        zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
        window.draw(zone);
      }
    }
    zone.setFillColor(Color::Transparent);
  }
}

void Render::drawCharacters(RenderWindow& window,
                            render::View& v,
                            Sprites& sprites,
                            vector<Character*> chars) {
  sf::View view = v.view;
  sf::Sprite sprite;
  RectangleShape zone(Vector2f(l, h));
  zone.setFillColor(Color::Transparent);
  zone.setOutlineThickness(-2);
  window.setView(view);
  for (auto c = chars.begin(); c != chars.end(); ++c) {
    float ic = (*c)->getI(), jc = (*c)->getJ();
    if (ic >= xv && ic < xv + n && jc >= yv && jc < yv + m) {
      int animation = (ic - (int)ic + jc - (int)jc) * 4 - 1;
      if (animation == -1)
        animation = 1;
      sprite = sprites.getCharacterSprite(l, h, (*c)->getId(),
                                          (int)(*c)->getDirection(), animation);
      sprite.setPosition(Vector2f(l * ic, h * jc));
      window.draw(sprite);
    }
  }

  if (state->isFighting()) {
    zone.setOutlineColor(Color::Blue);
    for (auto c : state->getFight()->getFightingCharacters(0)) {
      zone.setPosition(Vector2f(c->getI() * l, c->getJ() * h));
      window.draw(zone);
    }
    zone.setOutlineColor(Color::Red);
    for (auto c : state->getFight()->getFightingCharacters(1)) {
      zone.setPosition(Vector2f(c->getI() * l, c->getJ() * h));
      window.draw(zone);
    }
  }

  zone.setOutlineColor(Color::White);
  zone.setPosition(
      Vector2f(selectedcharacter->getI() * l, selectedcharacter->getJ() * h));
  window.draw(zone);
}

void Render::drawAnimations(RenderWindow& window,
                            render::View& v,
                            Sprites& sprites) {
  sf::View view = v.view;
  sf::Sprite sprite;
  window.setView(view);
  for (auto animation : state->animations) {
    sprite =
        sprites.getAttackSprite(l, h, animation[2], animation[3], animation[4]);
    sprite.setPosition(Vector2f(animation[0] * l, animation[1] * h));
    window.draw(sprite);
  }
}

void Render::drawInformations(RenderWindow& window,
                              render::View& v,
                              Sprites& sprites,
                              vector<Character*> chars) {
  sf::View view = v.view;
  sf::Sprite sprite;
  Vector2f mouse =
      window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
  int X = mouse.x / l, Y = mouse.y / h;
  vector<Ability*> abs = selectedcharacter->getWeapon()->getAbilities();
  Ability* a = abs[abilityNumber];
  window.setView(view);
  sf::Text text;
  Font font;
  font.loadFromFile("res/font/roboto/Roboto-Regular.ttf");
  text.setFont(font);
  text.setCharacterSize(24);
  RectangleShape r(Vector2f(view.getSize().x / 6, view.getSize().y));
  r.setFillColor(Color::Black);
  vector<Color> colors = {Color::White, Color::Blue, Color::Green, Color::Red,
                          Color::Yellow};
  for (int i = 0; i < 5; i++) {
    if ((i - 1) < (int)abs.size() && i > 0) {
      sprite = sprites.getAbilitySprite(
          2 * l, 2 * h, (int)abs[i - 1]->getElement(), abs[i - 1]->getLv());
      sprite.setPosition(Vector2f(i * view.getSize().x / 6, 0));
      window.draw(sprite);
    } else {
      r.setPosition(Vector2f(i * view.getSize().x / 6, 0));
      window.draw(r);
    }
  }
  Element* selectedCharStat = CharacterStats(selectedcharacter);
  selectedCharStat->setPosRelative({2, 2});
  window.draw(*selectedCharStat);
  delete selectedCharStat;

  text.setString("pa : " + to_string(a->getPa()) +
                 " atk : " + to_string(a->getDamage()));
  text.setPosition(Vector2f(view.getSize().x * 5 / 6, 0));
  text.setColor(colors[a->getElement()]);
  window.draw(text);

  Character* c = state->getCharacter(X, Y);
  for (auto ch : chars) {
    if (c == ch) {
      text.setString("\npv : " + to_string(c->getPvCurrent()) +
                     " pa : " + to_string(c->getPaCurrent()) +
                     "\npm : " + to_string(c->getPmCurrent()));
      text.setPosition(Vector2f(view.getSize().x * 5 / 6, 0));
      text.setColor(colors[c->getWeapon()->getElement()]);
      window.draw(text);
    }
  }
}

void Render::display() {
  SpriteGenerator::init();

  vector<vector<Cell*>> grid = state->getGrid();
  int nb = 2, l = 34 * nb,
      h = 24 * nb;  //, N = state->getI(), M = state->getJ();

  // Create view
  render::View worldView, abilityView;
  worldView.setSizeRelative(sf::Vector2f({(float)n * l, (float)m * h}));
  abilityView.setSizeRelative(
      {worldView.getSizeAbs().x, worldView.getSizeAbs().y / 6});
  abilityView.view.setCenter(
      {abilityView.getSizeAbs().x / 2, abilityView.getSizeAbs().y / 2});

  // set view port
  worldView.view.setViewport(sf::FloatRect(
      0, 0, 1,
      1 / (1 + abilityView.getSizeAbs().y / worldView.getSizeAbs().y)));
  abilityView.view.setViewport(sf::FloatRect(
      0, 1 / (1 + abilityView.getSizeAbs().y / worldView.getSizeAbs().y), 1,
      1 / (1 + worldView.getSizeAbs().y / abilityView.getSizeAbs().y)));

  // Create window
  RenderWindow window(
      VideoMode(worldView.getSizeAbs().x,
                worldView.getSizeAbs().y + abilityView.getSizeAbs().y),
      "Jeu", Style::Titlebar | Style::Close);
  window.setFramerateLimit(60);

  selectedcharacter = state->getMainCharacter();
  vector<Character*> chars;

  int x, y;
  x = selectedcharacter->getI();
  y = selectedcharacter->getJ();
  xv = (x / n) * n;
  yv = (y / m) * m;
  Vector2f posView = {(float)xv * l, (float)yv * h};

  sf::Sprite sprite;
  Sprites sprites(nb);

  Element* csheet = nullptr;
  Element* wsheet = nullptr;
  Character* prev = selectedcharacter;
  auto OpenedWindows = worldView.getChildren();

  while (window.isOpen()) {
    window.clear();

    if (state->isFighting()) {
      shared_ptr<Fight> fight = state->getFight();
      chars = fight->getFightingCharacters();
      if (selectedcharacter->getPvCurrent() <= 0) {
        abilityNumber = 0;
        selectedcharacter = fight->getFightingCharacters(0)[0];
      }
    } else {
      chars = state->getMainCharacters();
      selectedcharacter = state->getMainCharacter();
    }

    vector<Ability*> abs = selectedcharacter->getWeapon()->getAbilities();
    // Ability* a = abs[abilityNumber];
    x = selectedcharacter->getI();
    y = selectedcharacter->getJ();
    xv = (x / n) * n;
    yv = (y / m) * m;
    worldView.setPosRelative(sf::Vector2f(xv * l, yv * h));
    window.setView(worldView.view);
    Vector2f MouseWorldView = window.mapPixelToCoords(
        sf::Mouse::getPosition(window), window.getView());
    Vector2f MouseAbilityView = window.mapPixelToCoords(
        sf::Mouse::getPosition(window), abilityView.view);
    int X = MouseWorldView.x / l, Y = MouseWorldView.y / h;
    int X2 = floor(MouseAbilityView.x / l), Y2 = floor(MouseAbilityView.y / h);

    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
        window.close();

      posView = {(float)xv * l, (float)yv * h};
      Vector2f posMouse{(float)MouseWorldView.x, (float)MouseWorldView.y};
      worldView.reactEvent(event, posMouse);

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
          state->etatCombat = 0;
          if (state->isFighting() &&
              state->getMainTeam()->getCharacter(X, Y) != nullptr &&
              state->getCharacter(X, Y)->getPvCurrent() > 0) {
            selectedcharacter = state->getMainTeam()->getCharacter(X, Y);
            abilityNumber = 0;
          }
        } else if (event.mouseButton.button == sf::Mouse::Left) {
          if (Y2 >= 0) {
            if ((X2 / 2 - 1) < (int)abs.size() && (X2 / 2 - 1) >= 0) {
              if ((X2 / 2 - 1) != abilityNumber || state->etatCombat == 0) {
                abilityNumber = X2 / 2 - 1;
                if (state->isFighting())
                  state->etatCombat = 1;
              } else
                state->etatCombat = 0;
            }
          } else if (worldView.getChildren().size() == 0) {
            if (state->etatCombat == 0) {
              if (!state->isFighting()) {
                while (x + y - (int)x - (int)y != 0)
                  ;
                engine->clearCommands();
              }
              engine->addCommand(
                  new MoveCommands(state, engine, selectedcharacter, X, Y));
            } else if (state->etatCombat == 1) {
              engine->addCommand(
                  new AttackCommand(state, engine, selectedcharacter,
                                    {X - (int)x, Y - (int)y}, abilityNumber));
              state->etatCombat = 0;
            }
          }
        }
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return && state->isFighting() &&
            state->getFight()->getTurn() % 2 == 1) {
          engine->addCommand(new FightCommand(state,
                                              state->getFight()->getTeam(0),
                                              state->getFight()->getTeam(1)));
        } else if (event.key.code == sf::Keyboard::R) {
          engine->reverse = !engine->reverse;
        } else if (event.key.code == sf::Keyboard::T) {
          cout << worldView.getTreeView();
        } else if (event.key.code == sf::Keyboard::W) {
          state->etatCombat = 0;
          if (!worldView.getChild(wsheet)) {
            wsheet = WeaponSheet(selectedcharacter->getWeapon());
            worldView.add(wsheet);
            wsheet->setPosRelative({"50%", "25%"});
          } else {
            worldView.remove(wsheet);
            wsheet = nullptr;
          }
        } else if (event.key.code == sf::Keyboard::C) {
          state->etatCombat = 0;
          if (!worldView.getChild(csheet)) {
            csheet = CharacterSheet(selectedcharacter);
            worldView.add(csheet);
            csheet->setPosRelative({"25%", "25%"});
          } else {
            worldView.remove(csheet);
            csheet = nullptr;
          }
        }
      }
    }
    this->drawInformations(window, abilityView, sprites, chars);

    this->drawMap(window, worldView, sprites);

    this->drawZones(window, worldView);

    this->drawCharacters(window, worldView, sprites, chars);

    this->drawAnimations(window, worldView, sprites);

    if (OpenedWindows != worldView.getChildren()) {
      bool c = false, w = false;
      for (auto child : worldView.getChildren()) {
        if (child != csheet && child != wsheet) {
          if (!wsheet && child == csheet->getLinks()[0]) {
            wsheet = child->getCopy();
            worldView.add(wsheet);
            csheet->getLinked()[0]->setLink(wsheet->getCopy());
          }
          worldView.remove(child);
        }
      }
      for (auto child : worldView.getChildren()) {
        if (child == csheet)
          c = true;
        else if (child == wsheet)
          w = true;
      }
      if (!c)
        csheet = nullptr;
      if (!w)
        wsheet = nullptr;
      OpenedWindows = worldView.getChildren();
    }

    if (prev != selectedcharacter) {
      if (worldView.getChild(csheet)) {
        Relatif2 r = csheet->getPosRelative();
        worldView.remove(csheet);
        csheet = CharacterSheet(selectedcharacter);
        worldView.add(csheet);
        csheet->setPosRelative(r);
      }
      if (worldView.getChild(wsheet)) {
        Relatif2 r = wsheet->getPosRelative();
        worldView.remove(wsheet);
        wsheet = WeaponSheet(selectedcharacter->getWeapon());
        worldView.add(wsheet);
        wsheet->setPosRelative(r);
      }
      prev = selectedcharacter;
    }

    window.draw(worldView);

    // end the current frame
    window.display();
  }
}
