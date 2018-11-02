#include "Render.h"
#include <math.h>
#include <SFML/Graphics.hpp>
#include "IGWindow.h"
#include "IGWindowContainer.h"
#include "UIInventory.h"

using namespace std;
using namespace sf;
using namespace state;
using namespace render;

Render::Render(State* state) {
    this->state = state;
}

void Render::display() {
    World* world = state->getWorld();
    vector<vector < Cell*>> grid = world->getGrid();
    int N = world->getI(), M = world->getJ(), nb = 3, l = 34 * nb, h = 24 * nb,
            l2 = 32, h2 = 32, n = sqrt(N), m = sqrt(M);

    RenderWindow window(VideoMode(n * l, m * h), "Jeu");
    View view;
    view.setSize(Vector2f(n * l, m * h));

    // UIInventory
    IGWindowContainer wcontainer;
    UIInventory inv;

    Character* maincharacter = world->getMainCharacter();
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

                sprite = tiles.getSprite((int) element);
                sprite.setPosition(Vector2f(i * l, j * h));
                window.draw(sprite);
                if ((int) content > 1) {
                    sprite = contents.getSprite((int) content, (int) element);
                    sprite.setPosition(Vector2f(i * l, j * h));
                    window.draw(sprite);
                }
                zone.setPosition(Vector2f(i * l, j * h));
                window.draw(zone);
            }
        }

        zone.setPosition(
                Vector2f(maincharacter->getI() * l, maincharacter->getJ() * h));
        zone.setOutlineThickness(-2);
        zone.setOutlineColor(Color::White);
        window.draw(zone);
        zone.setOutlineThickness(-1);
        zone.setOutlineColor(Color::Black);

        vector<Character*> chars = world->getMainCharacters();
        for (auto c = chars.begin(); c != chars.end(); ++c) {
            float ic = (*c)->getI(), jc = (*c)->getJ();
            if (ic >= xv && ic < xv + n && jc >= yv && jc < yv + m) {
                int animation = (ic - (int) ic + jc - (int) jc)*4 - 1;
                if (animation == -1) animation = 1;
                sprite = persos.getSprite((*c)->getId(), (int) (*c)->getDirection(), animation);
                sprite.setScale(Vector2f(nb, (float) h / h2));
                sprite.setPosition(Vector2f(l * ic, h * jc));
                window.draw(sprite);
            }
        }

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
            Vector2f posMouse{(float) posMouseBuff.x, (float) posMouseBuff.y};
            wcontainer.transmit(event, posMouse + posView);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                world->moveCharacter(maincharacter, xv + posMouseBuff.x / l, yv + posMouseBuff.y / h);
            }
        }

        // end the current frame
        window.draw(wcontainer);
        window.display();
    }
}