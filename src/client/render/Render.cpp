#include "Render.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "IGWindow.h"
#include "IGWindowContainer.h"
#include "UIInventory.h"

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
  World* world = state->getWorld();
  vector<vector<Cell*>> grid = world->getGrid();
  int nb = 3, l = 34 * nb, h = 24 * nb, l2 = 32, h2 = 32;

  RenderWindow window(VideoMode(n * l, m * h), "Jeu");
  View view;
  view.setSize(Vector2f(n * l, m * h));

  // UIInventory
  IGWindowContainer wcontainer;
  UIInventory inv;

  Character* maincharacter = world->getMainCharacter();
  Ability* ability = maincharacter->getWeapon()->getAbilities()[0];
  mvcmd = new MoveCommands(state, engine, maincharacter);
  vector<Character*> chars;

  unsigned int x, y, xv, yv;
  x = maincharacter->getI();
  y = maincharacter->getJ();
  xv = (x / n) * n;
  yv = (y / m) * m;
  Vector2f posView = {xv * l, yv * h};
  inv.setPosition(posView + Vector2f{30, 30});
  wcontainer.add(&inv);

  Sprite sprite;
  TileSprite tiles(l, h, nb);
  ContentSprite contents(l, h);
  CharacterSprite persos("res/persos.png", l2, h2);

  RectangleShape zone(Vector2f(l, h));
  zone.setFillColor(Color::Transparent);
  zone.setOutlineThickness(-1);
  zone.setOutlineColor(Color::Black);

  while (window.isOpen()) {
    window.clear();

    if (state->isFighting())
      chars = state->getFight()->getFightingCharacters();
    else
      chars = world->getMainCharacters();

    x = maincharacter->getI();
    y = maincharacter->getJ();
    xv = (x / n) * n;
    yv = (y / m) * m;
    view.setCenter(sf::Vector2f(xv * l + n * l / 2, yv * h + m * h / 2));
    window.setView(view);
    for (unsigned int j = yv; j < yv + m; j++) {
      for (unsigned int i = xv; i < xv + n; i++) {
        ElementType element = grid[i][j]->getElement();
        ContentType content = grid[i][j]->getContent();

        sprite = tiles.getSprite((int)element);
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

    if (state->isFighting()) {
      int ok = 0;
      auto posMouseBuff = sf::Mouse::getPosition(window);
      vector<vector<int>> targets = ability->getTargetZone({x, y});
      zone.setFillColor(Color(0, 0, 255, 128));
      for (vector<int> coord : targets) {
        zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
        window.draw(zone);
        if (coord[0] == (xv + posMouseBuff.x / l) &&
            coord[1] == (yv + posMouseBuff.y / h))
          ok = 1;
      }

      if (ok) {
        vector<vector<int>> effects = ability->getEffectZone(
            {xv + posMouseBuff.x / l, yv + posMouseBuff.y / h});
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

    zone.setPosition(
        Vector2f(maincharacter->getI() * l, maincharacter->getJ() * h));
    zone.setOutlineThickness(-2);
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

      posView = {xv * l, yv * h};
      inv.setPosition(posView + Vector2f{30, 30});
      auto posMouseBuff = sf::Mouse::getPosition(window);
      Vector2f posMouse{(float)posMouseBuff.x, (float)posMouseBuff.y};
      wcontainer.transmit(event, posMouse + posView);

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          mvcmd->addCommands(xv + event.mouseButton.x / l,
                             yv + event.mouseButton.x / h);
          cout << xv + event.mouseButton.x / l << " "
               << yv + event.mouseButton.y / h << endl;
        }
      }
    }

    // end the current frame
    window.draw(wcontainer);
    window.display();
  }
}