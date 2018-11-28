#include "HeuristicAI.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include "engine/AttackCommand.h"
#include "engine/MoveCommands.h"

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

HeuristicAI::HeuristicAI(State* state, Engine* engine)
{
    this->state = state;
    this->engine = engine;
}

bool HeuristicAI::isCharacterAtpos(Character* c, int i, int j)
{
    // vector<Character*> fighters = state->getFight()->getFightingCharacters();
    //if (find(fighters.begin(), fighters.end(), c) != fighters.end())
    return c->getI() == i && c->getJ() == j;
    //return false;
}

int HeuristicAI::getScoreAction(MoveCommands* mv,
                                AttackCommand* atk,
                                Character* character,
                                vector<Character*> allies,
                                vector<Character*> ennemies)
{
    int score = 0;
    int bonusKillEnemy = 300;
    int MalusKillAlly = 300;

    if (atk == nullptr)
        return 0;
    Weapon* weapon = character->getWeapon();
    int dmg = weapon->getAbility(atk->getAbilityNumber())->getDamage();
    auto zoneDmg = atk->getZone(1);

    for (auto coord : zoneDmg)
    {
        if (mv != nullptr)
        {
            coord[0] += mv->getDiff()[0];
            coord[1] += mv->getDiff()[1];
        }
        for (auto pcharacter : ennemies)
        {
            if (isCharacterAtpos(pcharacter, coord[0], coord[1]))
            {
                int pv = pcharacter->getPvCurrent();
                score += dmg < pv ? (100 * dmg / pv) : bonusKillEnemy;
            }
        }
        for (auto pcharacter : allies)
        {
            if (isCharacterAtpos(pcharacter, coord[0], coord[1]))
            {
                int pv = pcharacter->getPvCurrent();
                score -= dmg < pv ? (100 * dmg / pv) : MalusKillAlly;
            }
        }
        score -= 25 * weapon->getAbility(atk->getAbilityNumber())->getPa();
    }
    return score;
}

std::tuple<MoveCommands*, AttackCommand*> HeuristicAI::getBestAction(
                                                                     Character* character)
{
    // get our and enemy fighters
    vector<Character*> allies = state->getFight()->getFightingCharacters(1);
    vector<Character*> ennemies = state->getFight()->getFightingCharacters(0);

    // get possible moves and attacks for the character
    vector<MoveCommands*> listmv;
    vector<AttackCommand*> listatk;
    // complete with no move and no atk
    listmv.push_back(nullptr);
    listatk.push_back(nullptr);

    for (auto pcommand : this->listCommands(character, 0))
    {
        listmv.push_back(static_cast<MoveCommands*> (pcommand));
    }
    for (auto pcommand : this->listCommands(character, 1))
    {
        listatk.push_back(static_cast<AttackCommand*> (pcommand));
    }

    int scoreMax =
            getScoreAction(listmv[0], listatk[0], character, allies, ennemies);
    // search best actions
    int i = 0, j = 0;
    vector<int> imax = {0}, jmax = {0};

    // cout << "score = {";
    for (auto mv : listmv)
    {
        for (auto atk : listatk)
        {
            int score = getScoreAction(mv, atk, character, allies, ennemies);
            //cout << score << ", ";

            if (score > scoreMax)
            {
                imax.clear();
                jmax.clear();
                scoreMax = score;
            }

            if (score == scoreMax)
            {
                imax.push_back(i);
                jmax.push_back(j);
            }
            j++;
        }
        j = 0;
        i++;
    }
    // cout << "}" << endl;
    //cout << "scoreMax = " << scoreMax << endl << endl;

    // Choose one of the best actions randomly
    int r = rand() % imax.size();
    i = imax[r];
    j = jmax[r];
    /*
    cout << "score action "
         << getScoreAction(listmv[i], listatk[j], *character, allies,
                           ennemies)
         << endl
         << endl;
     */
    return std::make_tuple(listmv[i], listatk[j]);
}

void HeuristicAI::run(Character* character)
{
    vector<Command*> listmv = this->listCommands(character, 0);
    vector<Command*> listatk = this->listCommands(character, 1);
    MoveCommands* mv;
    AttackCommand* atk;
    do
    {
        std::tuple<MoveCommands*, AttackCommand*> bestAction = this->getBestAction(character);
        mv = std::get<0>(bestAction);
        if (mv != nullptr)
        {
            engine->addCommand(mv);
            while (engine->getSize() != 0);
            listmv = this->listCommands(character, 0);
        }
        atk = std::get<1>(bestAction);
        if (atk != nullptr)
        {
            engine->addCommand(atk);
            while (engine->getSize() != 0);
            listatk = this->listCommands(character, 1);
        }
    }
    while ((mv != nullptr || atk != nullptr) && state->isFighting() && listmv.size() + listatk.size() > 2 && character->getPvCurrent() > 0);
}
