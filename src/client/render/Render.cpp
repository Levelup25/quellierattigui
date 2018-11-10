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
    vector<vector < Cell*>> grid = state->getGrid();
    int nb = 2, N = state->getI(), M = state->getJ(), l = 34 * nb, h = 24 * nb, l2 = 32, h2 = 32;

    RenderWindow window(VideoMode(n*l, m * h * 5 / 4), "Jeu");
    window.setFramerateLimit(60);
    View view;
    view.setSize(Vector2f(n*l, m * h));
    View view2;
    view2.setSize(Vector2f(n*l, m * h / 4));
    view2.setCenter(sf::Vector2f(N * l + n * l / 2, M * h + m * h / 4 / 2));
    view.setViewport(sf::FloatRect(0, 0, 1, 4.0 / 5));
    view2.setViewport(sf::FloatRect(0, 4.0 / 5, 1, 1.0 / 5));

    // UIInventory
    IGWindowContainer wcontainer;
    UIInventory inv;

    Character* maincharacter = state->getMainCharacter();
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
    AttackSprite attacks(3 * l, 3 * h);
    CharacterSprite persos(l2, h2);

    RectangleShape zone(Vector2f(l, h));
    zone.setFillColor(Color::Transparent);
    zone.setOutlineThickness(-1);
    zone.setOutlineColor(Color::Black);

    vector<vector<int>> moves;
    vector<vector<int>> targets;
    vector<vector<int>> effects;

    int abilityNumber = 0;

    RectangleShape r(Vector2f(n * l / 4, m * h / 4));
    r.setFillColor(Color::Black);

    while (window.isOpen()) {
        window.clear();

        if (state->isFighting()) {
            chars = state->getFight()->getFightingCharacters();
            if (maincharacter == nullptr)maincharacter = state->getMainCharacter();
        } else {
            chars = state->getMainCharacters();
            maincharacter = state->getMainCharacter();
            abilityNumber = 0;
        }

        vector<Ability*> abs = maincharacter->getWeapon()->getAbilities();
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

        if (state->isFighting() && state->getFight()->getTurn() % 2 == 1) {
            auto posMouseBuff = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            int X = xv + posMouseBuff.x / l, Y = yv + posMouseBuff.y / h;
            if (state->etatCombat == 0) {
                moves = (new MoveCommands(state, engine, maincharacter, X, Y))->getPath();
                zone.setFillColor(Color(0, 255, 0, 128));
                for (vector<int> coord : moves) {
                    zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
                    window.draw(zone);
                }
            } else if (state->etatCombat == 1) {
                AttackCommand* atkcmd = new AttackCommand(state, maincharacter,{X, Y}, abilityNumber);
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
                int animation = (ic - (int) ic + jc - (int) jc) * 4 - 1;
                if (animation == -1)
                    animation = 1;
                sprite = persos.getSprite((*c)->getId(), (int) (*c)->getDirection(),
                        animation);
                sprite.setScale(Vector2f(nb, (float) h / h2));
                sprite.setPosition(Vector2f(l * ic, h * jc));
                window.draw(sprite);
            }
        }

        zone.setOutlineThickness(-2);
        if (state->isFighting()) {
            zone.setOutlineColor(Color::Blue);
            for (auto c : state->getFight()->getTeam(0)->getCharacters()) {
                zone.setPosition(
                        Vector2f(c->getI() * l, c->getJ() * h));

                window.draw(zone);
            }
            zone.setOutlineColor(Color::Red);
            for (auto c : state->getFight()->getTeam(1)->getCharacters()) {
                zone.setPosition(
                        Vector2f(c->getI() * l, c->getJ() * h));

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

            posView = {xv * l, yv * h};
            inv.setPosition(posView + Vector2f{30, 30});
            auto posMouseBuff = sf::Mouse::getPosition(window);
            Vector2f posMouse{(float) posMouseBuff.x, (float) posMouseBuff.y};
            //wcontainer.transmit(event, posMouse + posView);

            if (event.type == sf::Event::MouseButtonPressed) {
                int X = xv + event.mouseButton.x / l, Y = yv + event.mouseButton.y / h;
                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (state->isFighting() && state->getMainTeam()->getCharacter(X, Y) != nullptr) {
                        maincharacter = state->getMainTeam()->getCharacter(X, Y);
                        abilityNumber = 0;
                    }
                } else if (event.mouseButton.button == sf::Mouse::Left) {
                    if (Y >= m) {
                        if (X <= 3 * abs.size()) {
                            if (X / 3 != abilityNumber || state->etatCombat == 0) {
                                abilityNumber = X / 3;
                                state->etatCombat = 1;
                            } else state->etatCombat = 0;
                        }
                    } else {
                        if (state->etatCombat == 0) {
                            if (!state->isFighting())engine->clearCommands();
                            engine->addCommand(new MoveCommands(state, engine, maincharacter, X, Y));
                        } else if (state->etatCombat == 1) {
                            engine->addCommand(new AttackCommand(state, maincharacter,{X, Y}, abilityNumber));
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
                //                    abilityNumber = (abilityNumber - 1) % maincharacter->getWeapon()->getAbilities().size();
                //                } else if (event.key.code == sf::Keyboard::Right) {
                //                    abilityNumber = (abilityNumber + 1) % maincharacter->getWeapon()->getAbilities().size();
                //                } else if (event.key.code == sf::Keyboard::M) {
                //                    state->etatCombat = 0;
                //                } else if (event.key.code == sf::Keyboard::A) {
                //                    state->etatCombat = 1;
                //                }
            }
        }

        window.setView(view2);
        for (int i = 0; i < 4; i++) {
            if (i < abs.size()) {
                sprite = attacks.getSprite((int) abs[i]->getElement(), abs[i]->getLv());
                sprite.setPosition(Vector2f(N * l + i * n * l / 4, M * h));
                window.draw(sprite);
            } else {
                r.setPosition(Vector2f(N * l + i * n * l / 4, M * h));
                window.draw(r);
            }
        }

        // end the current frame
        //window.draw(wcontainer);
        window.display();
    }
}