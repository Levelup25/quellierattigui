#include "MoveCommands.h"
#include "MoveCommand.h"
#include "DirectionCommand.h"
#include "FightCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;

MoveCommands::MoveCommands(State *state, Engine *engine, Character *character, size_t i, size_t j) {
    this->state = state;
    this->engine = engine;
    this->character = character;
    this->i = i;
    this->j = j;
    n = state->getN();
    m = state->getM();
    generator.setWorldSize({n, m});
}

void MoveCommands::setGenerator() {
    state->resetContents();
    int i0 = character->getI(), j0 = character->getJ();
    int xv = (i0 / n) * n, yv = (j0 / m) * m;
    generator.clearCollisions();
    for (int l = 0; l < m; l++) {
        for (int k = 0; k < n; k++) {
            if (state->getCell(k + xv, l + yv)->getContent() != nothing)
                generator.addCollision({k, l});
        }
    }
}

vector<vector<int>> MoveCommands::getPath() {
    vector<vector<int>> coords;
    if (character->getPm() > 0) {
        this->character = character;
        this->setGenerator();
        int i0 = character->getI(), j0 = character->getJ();
        int xv = (i0 / n) * n, yv = (j0 / m) * m;
        generator.removeCollision({i0 - xv, j0 - yv});
        generator.removeCollision({i - xv, j - yv});
        auto path = generator.findPath({(int) (i - xv), int(j - yv)},
        {
            (int) (i0 - xv), (int) (j0 - yv)
        });
        path.erase(path.begin());

        if (i >= 0 && j >= 0 && i < state->getI() && j < state->getJ() && state->getCell(i, j)->getContent() > 0 && path.size() > 0)
            path.pop_back();
        if (path.size() <= character->getPm()) {
            for (auto coord : path) {
                coords.push_back({xv + coord.x, yv + coord.y});
            }
        }
    }
    return coords;
}

void MoveCommands::execute() {
    this->character = character;
    this->setGenerator();
    int i0 = character->getI(), j0 = character->getJ();
    int xv = (i0 / n) * n, yv = (j0 / m) * m;
    generator.removeCollision({i0 - xv, j0 - yv});
    generator.removeCollision({i - xv, j - yv});
    auto path = generator.findPath({(int) (i - xv), int(j - yv)},
    {
        (int) (i0 - xv), (int) (j0 - yv)
    });
    path.erase(path.begin());
    if ((int) state->getCell(i, j)->getContent() > 0 && path.size() > 0)
        path.pop_back();

    float step = 1.0 / 12;

    if (path.size() > 0 && (!state->isFighting() || (state->isFighting() && path.size() <= character->getPm()))) {
        if (state->isFighting())
            character->removePm(path.size());
        for (auto coord = path.begin(); coord != path.end(); coord++) {
            float k = (*coord).x + xv, l = (*coord).y + yv;
            if (l > j0)
                engine->addCommand(new DirectionCommand(character, 0));
            else if (k < i0)
                engine->addCommand(new DirectionCommand(character, 1));
            else if (k > i0)
                engine->addCommand(new DirectionCommand(character, 2));
            else if (l < j0)
                engine->addCommand(new DirectionCommand(character, 3));
            for (float f = 1 - step; f >= 0; f = f - step) {
                //cout << (i0 - k) * f + k << " " << (j0 - l) * f + l << endl;
                engine->addCommand(new MoveCommand(state, character, (i0 - k) * f + k, (j0 - l) * f + l));
                if (f < step)engine->addCommand(new MoveCommand(state, character, k, l));
            }
            i0 = k;
            j0 = l;
        }

    }
    if (state->getCharacter(i, j) != character && (int) state->getCell(i, j)->getContent() == 1 && !state->isFighting())
        engine->addCommand(new FightCommand(state, state->getTeam(character), state->getTeam(state->getCharacter(i, j))));
    else if (!state->isFighting()) {
        if (i % n == 0 && i > 0) {
            i--;
            engine->addCommand(new DirectionCommand(character, 1));
            if (state->getCell(i, j)->getContent() == 0)
                engine->addCommand(new MoveCommand(state, character, i, j));
        } else if ((i + 1) % n == 0 && i < state->getI() - 1) {
            i++;
            engine->addCommand(new DirectionCommand(character, 2));
            if (state->getCell(i, j)->getContent() == 0)
                engine->addCommand(new MoveCommand(state, character, i, j));
        } else if (j % m == 0 && j > 0) {
            j--;
            engine->addCommand(new DirectionCommand(character, 3));
            if (state->getCell(i, j)->getContent() == 0)
                engine->addCommand(new MoveCommand(state, character, i, j));
        } else if ((j + 1) % m == 0 && j < state->getJ() - 1) {
            j++;
            engine->addCommand(new DirectionCommand(character, 0));
            if (state->getCell(i, j)->getContent() == 0)
                engine->addCommand(new MoveCommand(state, character, i, j));
        }
    }
}