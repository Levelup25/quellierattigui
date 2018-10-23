#include "DisplayState.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;
using namespace state;
using namespace render;

DisplayState::DisplayState(State* state) {
  this->state = state;
}

void DisplayState::display() {
  World* world = state->getWorld();
  vector<vector<Cell*>> grid = world->getGrid();
  int N = world->getI(), M = world->getJ(), nb = 3, l = 34 * nb, h = 24 * nb,
      l2 = 32, h2 = 32;
  RenderWindow window(VideoMode(N * l, M * h), "Jeu");

  Sprite sprite, perso;
  TileSprite neutre("res/tileset/isometric_pixel_flat_0000.png", l, h, nb);
  TileSprite eau("res/tileset/isometric_pixel_flat_0028.png", l, h, nb);
  TileSprite terre("res/tileset/isometric_pixel_flat_0055.png", l, h, nb);
  TileSprite feu("res/tileset/isometric_pixel_flat_0037.png", l, h, nb);
  TileSprite air("res/tileset/isometric_pixel_flat_0093.png", l, h, nb);
  CharacterSprite persos("res/persos.png", l2, h2);
  RectangleShape zone(Vector2f(nb * l, nb * h));
  zone.setFillColor(Color::Transparent);
  zone.setOutlineThickness(-1);
  zone.setOutlineColor(Color::Black);

  while (window.isOpen()) {
    for (int j = 0; j < M; j++) {
      for (int i = 0; i < N; i++) {
        ElementType element = grid[i][j]->getElement();
        if (element == neutral) {
          sprite = neutre.getSprite();
        } else if (element == water) {
          sprite = eau.getSprite();
        } else if (element == earth) {
          sprite = terre.getSprite();
        } else if (element == fire) {
          sprite = feu.getSprite();
        } else if (element == wind) {
          sprite = air.getSprite();
        }
        sprite.setPosition(Vector2f(i * l, j * h));
        zone.setPosition(Vector2f(i * l, j * h));
        window.draw(sprite);
        window.draw(zone);

        vector<Character*> chars = world->getCharacters();
        for (auto c = chars.begin(); c != chars.end(); ++c) {
          perso = persos.getSprite((*c)->getId(), (int)(*c)->getDirection(), 1);
          perso.setScale(sf::Vector2f(nb, (float)h / h2));
          perso.setPosition(
              Vector2f(nb * (l - l2) / 2 + l * (*c)->getI(), h * (*c)->getJ()));
          window.draw(perso);
        }
      }
    }
    // check all the window's events that were triggered since the last
    // iteration of the loop
    Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == Event::Closed)
        window.close();
    }

    // end the current frame
    window.display();
  }
}