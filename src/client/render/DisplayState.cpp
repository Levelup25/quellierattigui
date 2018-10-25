#include "DisplayState.h"
#include <SFML/Graphics.hpp>
#include <math.h>

using namespace std;
using namespace sf;
using namespace state;
using namespace render;

DisplayState::DisplayState(State* state) {
    this->state = state;
}

void DisplayState::display() {
    World* world = state->getWorld();
    vector<vector < Cell*>> grid = world->getGrid();
    int N = world->getI(), M = world->getJ(), nb = 3, l = 34 * nb, h = 24 * nb,
            l2 = 32, h2 = 32, n = sqrt(N), m = sqrt(M);

    RenderWindow window(VideoMode(n * l, m * h), "Jeu");
    View view;
    view.setSize(Vector2f(n*l, m * h));

    Sprite tile, obstacle, perso;
    TileSprite neutre("res/tileset/isometric_pixel_flat_0000.png", l, h, nb);
    TileSprite eau("res/tileset/isometric_pixel_flat_0028.png", l, h, nb);
    TileSprite terre("res/tileset/isometric_pixel_flat_0055.png", l, h, nb);
    TileSprite feu("res/tileset/isometric_pixel_flat_0037.png", l, h, nb);
    TileSprite air("res/tileset/isometric_pixel_flat_0093.png", l, h, nb);
    CharacterSprite persos("res/persos.png", l2, h2);

    vector<int> offsetI{160, 20, 160, 20, 160};
    vector<int> offsetJ{10, 10, 10, 165, 165};
    ContentSprite trees("res/trees.png", 105, 135, offsetI, offsetJ);

    offsetI = {195, 195, 195, 195, 195};
    offsetJ = {15, 205, 15, 300, 105};
    ContentSprite rocks("res/rocks.png", 85, 75, offsetI, offsetJ);

    RectangleShape zone(Vector2f(l, h));
    zone.setFillColor(Color::Transparent);
    zone.setOutlineThickness(-1);
    zone.setOutlineColor(Color::Black);

    Character* maincharacter = (world->getMainCharacters())[0];
    unsigned int x, y, xv, yv;
    while (window.isOpen()) {
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
                if (element == neutral) {
                    tile = neutre.getSprite();
                } else if (element == water) {
                    tile = eau.getSprite();
                } else if (element == earth) {
                    tile = terre.getSprite();
                } else if (element == fire) {
                    tile = feu.getSprite();
                } else if (element == wind) {
                    tile = air.getSprite();
                }
                tile.setPosition(Vector2f(i * l, j * h));
                window.draw(tile);

                if (content == tree) {
                    obstacle = trees.getSprite((int) element);
                    obstacle.setScale(Vector2f((float) l / 105, (float) h / 135));
                    obstacle.setPosition(Vector2f(i * l, j * h));
                    window.draw(obstacle);
                } else if (content == rock) {
                    obstacle = rocks.getSprite((int) element);
                    obstacle.setScale(Vector2f((float) l / 85, (float) h / 75));
                    obstacle.setPosition(Vector2f(i * l, j * h));
                    window.draw(obstacle);
                }

                zone.setPosition(Vector2f(i * l, j * h));
                window.draw(zone);
            }
        }
        zone.setPosition(Vector2f(maincharacter->getI() * l, maincharacter->getJ() * h));
        zone.setOutlineThickness(-2);
        zone.setOutlineColor(Color::White);
        window.draw(zone);
        zone.setOutlineThickness(-1);
        zone.setOutlineColor(Color::Black);
        vector<Character*> chars = world->getMainCharacters();
        for (auto c = chars.begin(); c != chars.end(); ++c) {
            if ((*c)->getI() >= xv && (*c)->getI() < xv + n && (*c)->getJ() >= yv && (*c)->getJ() < yv + m) {
                perso = persos.getSprite((*c)->getId(), (int) (*c)->getDirection(), 1);
                perso.setScale(Vector2f(nb, (float) h / h2));
                perso.setPosition(
                        Vector2f(l * (*c)->getI(), h * (*c)->getJ()));
                window.draw(perso);
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