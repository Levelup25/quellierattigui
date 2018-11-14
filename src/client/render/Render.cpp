#include "Render.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SpriteGenerator.h"
#include "View.h"
#include "Window.h"
#include "WindowManager.h"
#include "renderTest.h"

using namespace std;
using namespace sf;
using namespace state;
using namespace render;
using namespace engine;

Render::Render(State* state, Engine* engine) {
  this->state = state;
  this->engine = engine;
}

void Render::display() {
  SpriteGenerator::init();

  vector<vector<Cell*>> grid = state->getGrid();
  int nb = 2, N = state->getI(), M = state->getJ(), l = 34 * nb, h = 24 * nb,
      l2 = 32, h2 = 32;

  RenderWindow window(VideoMode(n * l, m * h * 7 / 6), "Jeu");
  window.setFramerateLimit(60);

  render::View worldView;
  worldView.setSizeRelative(sf::Vector2f({n * l, m * h}));

  auto pwm = buildRootSprite();
  pwm->setSizeRelative({"100%", "100%"});
  worldView.add(pwm);

  sf::View view2;
  view2.setSize(Vector2f(n * l, m * h / 6));
  view2.setCenter(sf::Vector2f(N * l + n * l / 2, M * h + m * h / 6 / 2));
  worldView.view.setViewport(sf::FloatRect(0, 0, 1, 6.0 / 7));
  view2.setViewport(sf::FloatRect(0, 6.0 / 7, 1, 1.0 / 7));

  Character* maincharacter = state->getMainCharacter();
  vector<Character*> chars;

  unsigned int x, y, xv, yv;
  x = maincharacter->getI();
  y = maincharacter->getJ();
  xv = (x / n) * n;
  yv = (y / m) * m;
  Vector2f posView = {xv * l, yv * h};

  sf::Sprite sprite;
  // TileSprite tiles(l, h, nb);
  std::vector<SpriteGeneratorById*> TileGenerators = {
      SpriteGenerator::Tile::pdefault, SpriteGenerator::Tile::palt1,
      SpriteGenerator::Tile::palt2};
  size_t offsetTileGenerator = 0;
  auto TileGenerator = TileGenerators[offsetTileGenerator];

  ContentSprite contents(l, h);
  AttackSprite attacks(2 * l, 2 * h);
  CharacterSprite persos(l2, h2);

  RectangleShape zone(Vector2f(l, h));
  zone.setFillColor(Color::Transparent);
  zone.setOutlineThickness(-1);
  zone.setOutlineColor(Color::Black);

  vector<vector<int>> moves;
  vector<vector<int>> targets;
  vector<vector<int>> effects;

  int abilityNumber = 0;

  RectangleShape r(Vector2f(n * l / 6, m * h / 6));
  r.setFillColor(Color::Black);

  sf::Text text;
  Font font;
  font.loadFromFile("res/font/roboto/Roboto-Regular.ttf");
  text.setFont(font);
  text.setCharacterSize(24);
  text.setColor(sf::Color::White);

  while (window.isOpen()) {
    window.clear();

    if (state->isFighting()) {
      chars = state->getFight()->getFightingCharacters();
      if (maincharacter == nullptr)
        maincharacter = state->getMainCharacter();
    } else {
      chars = state->getMainCharacters();
      maincharacter = state->getMainCharacter();
    }

    vector<Ability*> abs = maincharacter->getWeapon()->getAbilities();
    x = maincharacter->getI();
    y = maincharacter->getJ();
    xv = (x / n) * n;
    yv = (y / m) * m;
    worldView.setPosRelative(sf::Vector2f(xv * l, yv * h));
    window.setView(worldView.view);
    auto posMouseBuff = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    int X = xv + posMouseBuff.x / l, Y = yv + posMouseBuff.y / h;

    for (unsigned int j = yv; j < yv + m; j++) {
      for (unsigned int i = xv; i < xv + n; i++) {
        ElementType element = grid[i][j]->getElement();
        ContentType content = grid[i][j]->getContent();

        // sprite = tiles.getSprite((int)element);
        sprite = TileGenerator->getSpriteRepeated((int)element, {2, 2});
        sprite.setPosition(Vector2f(i * l, j * h));
        window.draw(sprite);

        if ((int)content > 1) {
          sprite = contents.getSprite((int)content, (int)element);
          sprite.setPosition(Vector2f(i * l, j * h));
          window.draw(sprite);
        }
        zone.setPosition(Vector2f(i * l, j * h));
        window.draw(zone);
      }
    }

    if (state->isFighting() && state->getFight()->getTurn() % 2 == 1) {
      if (state->etatCombat == 0) {
        moves =
            (new MoveCommands(state, engine, maincharacter, X, Y))->getPath();
        zone.setFillColor(Color(0, 255, 0, 128));
        for (vector<int> coord : moves) {
          zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
          window.draw(zone);
        }
      } else if (state->etatCombat == 1) {
        AttackCommand* atkcmd =
            new AttackCommand(state, maincharacter, {X, Y}, abilityNumber);
        atkcmd->setZones();
        targets = atkcmd->getZone(0);
        zone.setFillColor(Color(0, 0, 255, 128));
        for (vector<int> coord : targets) {
          zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
          window.draw(zone);
        }
        if (Y < m) {
          effects = atkcmd->getZone(1);
          zone.setFillColor(Color(255, 0, 0, 128));
          for (vector<int> coord : effects) {
            zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
            window.draw(zone);
          }
        }
      }
      zone.setFillColor(Color::Transparent);
    }

    for (auto c = chars.begin(); c != chars.end(); ++c) {
      float ic = (*c)->getI(), jc = (*c)->getJ();
      if (ic >= xv && ic < xv + n && jc >= yv && jc < yv + m) {
        int animation = (ic - (int)ic + jc - (int)jc) * 4 - 1;
        if (animation == -1)
          animation = 1;
        sprite = persos.getSprite((*c)->getId(), (int)(*c)->getDirection(),
                                  animation);
        sprite.setScale(Vector2f(nb, (float)h / h2));
        sprite.setPosition(Vector2f(l * ic, h * jc));
        window.draw(sprite);
      }
    }

    zone.setOutlineThickness(-2);
    if (state->isFighting()) {
      zone.setOutlineColor(Color::Blue);
      for (auto c : state->getFight()->getTeam(0)->getCharacters()) {
        zone.setPosition(Vector2f(c->getI() * l, c->getJ() * h));

        window.draw(zone);
      }
      zone.setOutlineColor(Color::Red);
      for (auto c : state->getFight()->getTeam(1)->getCharacters()) {
        zone.setPosition(Vector2f(c->getI() * l, c->getJ() * h));

        window.draw(zone);
      }
    }

    zone.setPosition(
        Vector2f(maincharacter->getI() * l, maincharacter->getJ() * h));
    zone.setOutlineColor(Color::White);
    window.draw(zone);
    zone.setOutlineThickness(-1);
    zone.setOutlineColor(Color::Black);

    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Add) {
        offsetTileGenerator += 1;
        offsetTileGenerator %= TileGenerators.size();
        TileGenerator = TileGenerators[offsetTileGenerator];
      }
      posView = {xv * l, yv * h};
      Vector2f posMouse{(float)posMouseBuff.x, (float)posMouseBuff.y};
      worldView.reactEvent(event, posMouse);

      if (event.type == sf::Event::MouseButtonPressed) {
        int X = xv + event.mouseButton.x / l, Y = yv + event.mouseButton.y / h;
        if (event.mouseButton.button == sf::Mouse::Right) {
          if (state->isFighting() &&
              state->getMainTeam()->getCharacter(X, Y) != nullptr) {
            maincharacter = state->getMainTeam()->getCharacter(X, Y);
            abilityNumber = 0;
          }
        } else if (event.mouseButton.button == sf::Mouse::Left) {
          if (Y >= m) {
            if ((X / 2 - 1) < abs.size()) {
              if ((X / 2 - 1) != abilityNumber || state->etatCombat == 0) {
                abilityNumber = X / 2 - 1;
                if (state->isFighting())
                  state->etatCombat = 1;
              } else
                state->etatCombat = 0;
            }
          } else {
            if (state->etatCombat == 0) {
              if (!state->isFighting())
                engine->clearCommands();
              engine->addCommand(
                  new MoveCommands(state, engine, maincharacter, X, Y));
            } else if (state->etatCombat == 1) {
              engine->addCommand(new AttackCommand(state, maincharacter, {X, Y},
                                                   abilityNumber));
              state->etatCombat = 0;
            }
          }
        }
      }
      if (event.type == sf::Event::KeyPressed && state->isFighting()) {
        if (event.key.code == sf::Keyboard::Return) {
          state->getFight()->endTurn();
          state->getFight()->endTurn();
          state->etatCombat = 0;
        }
        //                else if (event.key.code == sf::Keyboard::Left) {
        //                    abilityNumber = (abilityNumber - 1) %
        //                    maincharacter->getWeapon()->getAbilities().size();
        //                } else if (event.key.code == sf::Keyboard::Right) {
        //                    abilityNumber = (abilityNumber + 1) %
        //                    maincharacter->getWeapon()->getAbilities().size();
        //                } else if (event.key.code == sf::Keyboard::M) {
        //                    state->etatCombat = 0;
        //                } else if (event.key.code == sf::Keyboard::A) {
        //                    state->etatCombat = 1;
        //                }
      }
    }

    window.draw(worldView);

    window.setView(view2);
    for (int i = 0; i < 5; i++) {
      if ((i - 1) < abs.size() && i > 0) {
        sprite = attacks.getSprite((int)abs[i - 1]->getElement(),
                                   abs[i - 1]->getLv());
        sprite.setPosition(Vector2f(N * l + i * n * l / 6, M * h));
        window.draw(sprite);
      } else {
        r.setPosition(Vector2f(N * l + i * n * l / 6, M * h));
        window.draw(r);
      }
    }
    text.setString("pv : " + to_string(maincharacter->getPv()) +
                   " pa : " + to_string(maincharacter->getPa()) +
                   "\npm : " + to_string(maincharacter->getPm()));
    text.setPosition(Vector2f(N * l, M * h));
    window.draw(text);
    if (state->getCharacter(X, Y) != nullptr) {
      Character* c = state->getCharacter(X, Y);
      text.setString("pa : " + to_string(abs[abilityNumber]->getPa()) +
                     " atk : " + to_string(abs[abilityNumber]->getDamage()) +
                     "\npv : " + to_string(c->getPv()) + " pa : " +
                     to_string(c->getPa()) + "\npm : " + to_string(c->getPm()));
    } else
      text.setString("pa : " + to_string(abs[abilityNumber]->getPa()) +
                     " atk : " + to_string(abs[abilityNumber]->getDamage()));
    text.setPosition(Vector2f((N + n - 2) * l, M * h));
    window.draw(text);

    // end the current frame
    window.display();
  }
}