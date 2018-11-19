#include "Render.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SpriteGenerator.h"
#include "View.h"
#include "Window.h"
#include "WindowManager.h"
#include "ai/RandomAI.h"
#include "renderTest.h"

using namespace std;
using namespace sf;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;

Render::Render(State* state, Engine* engine) {
  this->state = state;
  this->engine = engine;
  n = state->getN();
  m = state->getM();
}

void Render::display() {
  SpriteGenerator::init();

  vector<vector<Cell*>> grid = state->getGrid();
  int nb = 2, N = state->getI(), M = state->getJ(), l = 34 * nb, h = 24 * nb,
      l2 = 32, h2 = 32;

  RenderWindow window(VideoMode(n * l, m * h * 7 / 6), "Jeu",
                      Style::Titlebar | Style::Close);
  window.setFramerateLimit(60);

  render::View worldView;
  worldView.setSizeRelative(sf::Vector2f({n * l, m * h}));

  auto pwm = new Element();
  pwm = buildRootSprite();
  pwm->setSizeRelative({"100%", "100%"});
  // worldView.add(pwm);

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

  std::vector<SpriteGeneratorById*> TileGenerators = {
      SpriteGenerator::Tile::pdefault, SpriteGenerator::Tile::palt1,
      SpriteGenerator::Tile::palt2};
  size_t offsetTileGenerator = 0;
  auto TileGenerator = TileGenerators[offsetTileGenerator];

  AttackSprite attacks(l, h);
  ContentSprite contents(l, h);
  AbilitySprite abilities(2 * l, 2 * h);
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
  vector<Color> colors = {Color::White, Color::Blue, Color::Green, Color::Red,
                          Color::Yellow};

  while (window.isOpen()) {
    window.clear();

    if (state->isFighting()) {
      Fight* fight = state->getFight();
      chars = fight->getFightingCharacters();
      if (maincharacter->getPvCurrent() <= 0) {
        maincharacter = fight->getFightingCharacters(0)[0];
        //                for (auto c : fight->getFightingCharacters(0)) {
        //                    if (c->getPvCurrent() > 0) {
        //                        maincharacter = c;
        //                        break;
        //                    }
        //                }
      }
    } else {
      chars = state->getMainCharacters();
      maincharacter = state->getMainCharacter();
    }

    vector<Ability*> abs = maincharacter->getWeapon()->getAbilities();
    Ability* a = abs[abilityNumber];
    x = maincharacter->getI();
    y = maincharacter->getJ();
    xv = (x / n) * n;
    yv = (y / m) * m;
    worldView.setPosRelative(sf::Vector2f(xv * l, yv * h));
    window.setView(worldView.view);
    auto posMouseBuff = sf::Mouse::getPosition(window);
    int X = xv + posMouseBuff.x / l, Y = yv + posMouseBuff.y / h;

    for (unsigned int j = yv; j < yv + m; j++) {
      for (unsigned int i = xv; i < xv + n; i++) {
        ElementType element = grid[i][j]->getElement();
        ContentType content = grid[i][j]->getContent();

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
        AttackCommand* atkcmd = new AttackCommand(state, engine, maincharacter,
                                                  {X, Y}, abilityNumber);
        atkcmd->setZones();
        targets = atkcmd->getZone(0);
        zone.setFillColor(Color(0, 0, 255, 128));
        for (vector<int> coord : targets) {
          zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
          window.draw(zone);
        }
        effects = atkcmd->getZone(1);
        zone.setFillColor(Color(255, 0, 0, 128));
        for (vector<int> coord : effects) {
          zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
          window.draw(zone);
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
        Vector2f(maincharacter->getI() * l, maincharacter->getJ() * h));
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
          state->etatCombat = 0;
          if (state->isFighting() &&
              state->getMainTeam()->getCharacter(X, Y) != nullptr &&
              state->getCharacter(X, Y)->getPvCurrent() > 0) {
            maincharacter = state->getMainTeam()->getCharacter(X, Y);
            abilityNumber = 0;
          }
        } else if (event.mouseButton.button == sf::Mouse::Left) {
          if (Y - yv >= m) {
            int X2 = X - xv;
            if ((X2 / 2 - 1) < abs.size()) {
              if ((X2 / 2 - 1) != abilityNumber || state->etatCombat == 0) {
                abilityNumber = X2 / 2 - 1;
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
              engine->addCommand(new AttackCommand(state, engine, maincharacter,
                                                   {X, Y}, abilityNumber));
              state->etatCombat = 0;
            }
          }
        }
      }
      if (event.type == sf::Event::KeyPressed && state->isFighting() &&
          state->getFight()->getTurn() % 2 == 1) {
        if (event.key.code == sf::Keyboard::Return) {
          engine->addCommand(new FightCommand(state, nullptr, nullptr));
          //                    for (auto c :
          //                    state->getFight()->getFightingCharacters(1))
          //                        ai->run(c);
          //                    engine->addCommand(new FightCommand(state,
          //                    nullptr, nullptr));
        }
      }
    }

    for (auto animation : state->animations) {
      sprite = attacks.getSprite(animation[2], animation[3],
                                 animation[4]);  // direction element lv
      sprite.setPosition(Vector2f(animation[0] * l, animation[1] * h));
      window.draw(sprite);
    }

    window.draw(worldView);

    window.setView(view2);
    for (int i = 0; i < 5; i++) {
      if ((i - 1) < abs.size() && i > 0) {
        sprite = abilities.getSprite((int)abs[i - 1]->getElement(),
                                     abs[i - 1]->getLv());
        sprite.setPosition(Vector2f(N * l + i * n * l / 6, M * h));
        window.draw(sprite);
      } else {
        r.setPosition(Vector2f(N * l + i * n * l / 6, M * h));
        window.draw(r);
      }
    }
    text.setString("pv : " + to_string(maincharacter->getPvCurrent()) +
                   " pa : " + to_string(maincharacter->getPaCurrent()) +
                   "\npm : " + to_string(maincharacter->getPmCurrent()));
    text.setPosition(Vector2f(N * l, M * h));
    text.setColor(colors[maincharacter->getWeapon()->getElement()]);
    window.draw(text);

    text.setString("pa : " + to_string(a->getPa()) +
                   " atk : " + to_string(a->getDamage()));
    text.setPosition(Vector2f((N + n - 2) * l, M * h));
    text.setColor(colors[a->getElement()]);
    window.draw(text);

    Character* c = state->getCharacter(X, Y);
    for (auto ch : chars) {
      if (c == ch) {
        text.setString("\npv : " + to_string(c->getPvCurrent()) +
                       " pa : " + to_string(c->getPaCurrent()) +
                       "\npm : " + to_string(c->getPmCurrent()));
        text.setPosition(Vector2f((N + n - 2) * l, M * h));
        text.setColor(colors[c->getWeapon()->getElement()]);
        window.draw(text);
      }
    }

    //        for (int b = yv; b < yv + m; b++) {
    //            for (int a = xv; a < xv + n; a++) {
    //                cout << state->getCell(a, b)->getContent() << " ";
    //            }
    //            cout << endl;
    //        }
    //        cout << endl;

    // end the current frame
    window.display();
  }
}
