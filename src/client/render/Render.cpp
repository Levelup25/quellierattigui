#include "Render.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Sprites.h"
#include "View.h"
#include "Window.h"
#include "WindowManager.h"
#include "renderTest.h"

using namespace std;
using namespace sf;
using namespace state;
using namespace render;
using namespace engine;

Render::Render(State* state, Engine* engine)
{
    this->state = state;
    this->engine = engine;
    n = state->getN();
    m = state->getM();
}

void Render::display()
{
    vector<vector < Cell*>> grid = state->getGrid();
    int nb = 2, l = 34 * nb, h = 24 * nb; //, N = state->getI(), M = state->getJ();

    render::View worldView;
    worldView.setSizeRelative(sf::Vector2f({(float) n * l, (float) m * h}));

    auto pwm = new Element();
    // pwm = buildRootSprite();
    pwm->setSizeRelative({"100%", "100%"});
    // worldView.add(pwm);

    sf::View abilityView;
    abilityView.setSize(
                        Vector2f(worldView.getSize().x, worldView.getSize().y / 6));
    abilityView.setCenter(
                          sf::Vector2f(abilityView.getSize().x / 2, abilityView.getSize().y / 2));
    worldView.view.setViewport(sf::FloatRect(
                                             0, 0, 1, 1 / (1 + abilityView.getSize().y / worldView.getSize().y)));
    abilityView.setViewport(sf::FloatRect(
                                          0, 1 / (1 + abilityView.getSize().y / worldView.getSize().y), 1,
                                          1 / (1 + worldView.getSize().y / abilityView.getSize().y)));

    RenderWindow window(
                        VideoMode(worldView.getSize().x,
                                  worldView.getSize().y + abilityView.getSize().y),
                        "Jeu", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);

    Character* maincharacter = state->getMainCharacter();
    vector<Character*> chars;

    unsigned int x, y, xv, yv;
    x = maincharacter->getI();
    y = maincharacter->getJ();
    xv = (x / n) * n;
    yv = (y / m) * m;
    Vector2f posView = {(float) xv * l, (float) yv * h};

    sf::Sprite sprite;
    Sprites sprites(nb);

    RectangleShape zone(Vector2f(l, h));
    zone.setFillColor(Color::Transparent);
    zone.setOutlineThickness(-1);
    zone.setOutlineColor(Color::Black);

    vector<vector<int>> moves;
    vector<vector<int>> targets;
    vector<vector<int>> effects;

    int abilityNumber = 0;

    RectangleShape r(
                     Vector2f(abilityView.getSize().x / 6, abilityView.getSize().y));
    r.setFillColor(Color::Black);

    sf::Text text;
    Font font;
    font.loadFromFile("res/font/roboto/Roboto-Regular.ttf");
    text.setFont(font);
    text.setCharacterSize(24);
    vector<Color> colors = {Color::White, Color::Blue, Color::Green, Color::Red,
        Color::Yellow};

    while (window.isOpen())
    {
        window.clear();

        if (state->isFighting())
        {
            Fight* fight = state->getFight();
            chars = fight->getFightingCharacters();
            if (maincharacter->getPvCurrent() <= 0)
            {
                maincharacter = fight->getFightingCharacters(0)[0];
            }
        }
        else
        {
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
        auto MouseWorldView = window.mapPixelToCoords(
                                                      sf::Mouse::getPosition(window), window.getView());
        auto MouseAbilityView =
                window.mapPixelToCoords(sf::Mouse::getPosition(window), abilityView);
        int X = MouseWorldView.x / l, Y = MouseWorldView.y / h;
        int X2 = floor(MouseAbilityView.x / l), Y2 = floor(MouseAbilityView.y / h);

        // check all the window's events that were triggered since the last
        // iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            posView = {(float) xv * l, (float) yv * h};
            Vector2f posMouse{(float) MouseWorldView.x, (float) MouseWorldView.y};
            worldView.reactEvent(event, posMouse);

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    state->etatCombat = 0;
                    if (state->isFighting() &&
                            state->getMainTeam()->getCharacter(X, Y) != nullptr &&
                            state->getCharacter(X, Y)->getPvCurrent() > 0)
                    {
                        maincharacter = state->getMainTeam()->getCharacter(X, Y);
                        abilityNumber = 0;
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (Y2 >= 0)
                    {
                        if ((X2 / 2 - 1) < (int) abs.size())
                        {
                            if ((X2 / 2 - 1) != abilityNumber || state->etatCombat == 0)
                            {
                                abilityNumber = X2 / 2 - 1;
                                if (state->isFighting())
                                    state->etatCombat = 1;
                            }
                            else
                                state->etatCombat = 0;
                        }
                    }
                    else
                    {
                        if (state->etatCombat == 0)
                        {
                            if (!state->isFighting())
                                engine->clearCommands();
                            engine->addCommand(
                                               new MoveCommands(state, engine, maincharacter, X, Y));
                        }
                        else if (state->etatCombat == 1)
                        {
                            engine->addCommand(new AttackCommand(state, engine, maincharacter,{X, Y}, abilityNumber));
                            state->etatCombat = 0;
                        }
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed && state->isFighting() &&
                    state->getFight()->getTurn() % 2 == 1)
            {
                if (event.key.code == sf::Keyboard::Return)
                {
                    engine->addCommand(new FightCommand(state, nullptr, nullptr));
                }
            }
        }

        for (unsigned int j = yv; j < yv + m; j++)
        {
            for (unsigned int i = xv; i < xv + n; i++)
            {
                ElementType element = grid[i][j]->getElement();
                ContentType content = grid[i][j]->getContent();

                sprite = sprites.getTileSprite(l, h, (int) element);
                sprite.setPosition(Vector2f(i * l, j * h));
                window.draw(sprite);

                if ((int) content > 1)
                {
                    sprite = sprites.getContentSprite(l, h, (int) content, (int) element);
                    sprite.setPosition(Vector2f(i * l, j * h));
                    window.draw(sprite);
                }
                zone.setPosition(Vector2f(i * l, j * h));
                window.draw(zone);
            }
        }

        if (state->isFighting() && state->getFight()->getTurn() % 2 == 1)
        {
            if (state->etatCombat == 0)
            {
                moves =
                        (new MoveCommands(state, engine, maincharacter, X, Y))->getPath();
                zone.setFillColor(Color(0, 255, 0, 128));
                if (moves.size() <= maincharacter->getPmCurrent())
                {
                    for (vector<int> coord : moves)
                    {
                        zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
                        window.draw(zone);
                    }
                }
            }
            else if (state->etatCombat == 1)
            {
                AttackCommand* atkcmd = new AttackCommand(state, engine, maincharacter,{X, Y}, abilityNumber);
                atkcmd->setZones();
                targets = atkcmd->getZone(0);
                zone.setFillColor(Color(0, 0, 255, 128));
                for (vector<int> coord : targets)
                {
                    zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
                    window.draw(zone);
                }
                effects = atkcmd->getZone(1);
                zone.setFillColor(Color(255, 0, 0, 128));
                for (vector<int> coord : effects)
                {
                    zone.setPosition(Vector2f(l * coord[0], h * coord[1]));
                    window.draw(zone);
                }
            }
            zone.setFillColor(Color::Transparent);
        }

        for (auto c = chars.begin(); c != chars.end(); ++c)
        {
            float ic = (*c)->getI(), jc = (*c)->getJ();
            if (ic >= xv && ic < xv + n && jc >= yv && jc < yv + m)
            {
                int animation = (ic - (int) ic + jc - (int) jc) * 4 - 1;
                if (animation == -1)
                    animation = 1;
                sprite = sprites.getCharacterSprite(l, h, (*c)->getId(), (int) (*c)->getDirection(), animation);
                sprite.setPosition(Vector2f(l * ic, h * jc));
                window.draw(sprite);
            }
        }

        zone.setOutlineThickness(-2);
        if (state->isFighting())
        {
            zone.setOutlineColor(Color::Blue);
            for (auto c : state->getFight()->getFightingCharacters(0))
            {
                zone.setPosition(Vector2f(c->getI() * l, c->getJ() * h));
                window.draw(zone);
            }
            zone.setOutlineColor(Color::Red);
            for (auto c : state->getFight()->getFightingCharacters(1))
            {
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

        for (auto animation : state->animations)
        {
            sprite = sprites.getAttackSprite(l, h, animation[2], animation[3], animation[4]);
            sprite.setPosition(Vector2f(animation[0] * l, animation[1] * h));
            window.draw(sprite);
        }

        window.draw(worldView);

        window.setView(abilityView);
        for (int i = 0; i < 5; i++)
        {
            if ((i - 1) < (int) abs.size() && i > 0)
            {
                sprite = sprites.getAbilitySprite(2 * l, 2 * h, (int) abs[i - 1]->getElement(), abs[i - 1]->getLv());
                sprite.setPosition(Vector2f(i * abilityView.getSize().x / 6, 0));
                window.draw(sprite);
            }
            else
            {
                r.setPosition(Vector2f(i * abilityView.getSize().x / 6, 0));
                window.draw(r);
            }
        }
        text.setString("pv : " + to_string(maincharacter->getPvCurrent()) +
                       " pa : " + to_string(maincharacter->getPaCurrent()) +
                       "\npm : " + to_string(maincharacter->getPmCurrent()));
        text.setPosition(Vector2f(0, 0));
        text.setColor(colors[maincharacter->getWeapon()->getElement()]);
        window.draw(text);

        text.setString("pa : " + to_string(a->getPa()) +
                       " atk : " + to_string(a->getDamage()));
        text.setPosition(Vector2f(abilityView.getSize().x * 5 / 6, 0));
        text.setColor(colors[a->getElement()]);
        window.draw(text);

        Character* c = state->getCharacter(X, Y);
        for (auto ch : chars)
        {
            if (c == ch)
            {
                text.setString("\npv : " + to_string(c->getPvCurrent()) +
                               " pa : " + to_string(c->getPaCurrent()) +
                               "\npm : " + to_string(c->getPmCurrent()));
                text.setPosition(Vector2f(abilityView.getSize().x * 5 / 6, 0));
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
