// Generated by dia2code
#ifndef STATE__STATE__H
#define STATE__STATE__H

#include <vector>
#include <stdlib.h>

namespace state {
  class Team;
  class MainQuest;
  class Fight;
  class Cell;
  class Character;
}

#include "Direction.h"
#include "Fight.h"
#include "Team.h"
#include "Cell.h"
#include "MainQuest.h"

namespace state {

  /// class State - 
  class State {
    // Associations
    // Attributes
  public:
    int etatCombat     = 0;
    bool isInventoryOpened     = false;
    std::vector<std::vector<int>> animations;
  private:
    std::size_t I     = 144;
    std::size_t J     = 144;
    std::size_t n     = 12;
    std::size_t m     = 12;
    std::vector<std::vector<Cell*>> grid;
    std::vector<Team*> teams;
    std::vector<MainQuest*> quests;
    Fight* fight;
  protected:
    int epoch     = 0;
    int epochRate     = 0;
    // Operations
  public:
    State (std::size_t I = 144, std::size_t J = 144);
    void resetContents ();
    std::size_t getI ();
    std::size_t getJ ();
    std::size_t getN ();
    std::size_t getM ();
    std::vector<std::vector<Cell*>> getGrid ();
    Cell* getCell (std::size_t i, std::size_t j);
    void setCell (std::size_t i, std::size_t j, Cell* cell);
    void addCharacter ();
    void addCharacter (std::size_t iteam, int id = 0, Direction direction = south, std::size_t i = 0, std::size_t j = 0);
    void addCharacter (Character* character, Team* team, std::size_t i = 0, std::size_t j = 0);
    void moveCharacter (Character* character, float i, float j);
    void delCharacter (std::size_t i = 0, std::size_t j = 0);
    void delCharacter (Character* character);
    Character* getCharacter (std::size_t i, std::size_t j);
    std::vector<Character*> getMainCharacters ();
    std::vector<Character*> getCharacters ();
    std::vector<Team*> getTeams ();
    Team* getTeam (Character* character);
    void delTeam (Team* team);
    void addTeam ();
    void addTeam (Team* team);
    void addQuest (MainQuest* quest);
    void delQuest (MainQuest* quest);
    Team* getMainTeam ();
    Character* getMainCharacter ();
    bool isFighting ();
    Fight* getFight ();
    void setFight (Fight* fight);
    void deploy (int nb = -1);
    void endFight ();
    // Setters and Getters
    int getEpoch() const;
    void setEpoch(int epoch);
    int getEpochRate() const;
    void setEpochRate(int epochRate);
  };

};

#endif
