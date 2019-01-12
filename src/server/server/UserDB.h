// Generated by dia2code
#ifndef SERVER__USERDB__H
#define SERVER__USERDB__H

#include <map>
#include <memory>

namespace server {
  class User;
}

#include "User.h"

namespace server {

  /// class UserDB - 
  class UserDB {
    // Associations
    // Attributes
  protected:
    int idseq;
    std::map<int,std::unique_ptr<User>> users;
    // Operations
  public:
    UserDB ();
    const User* getUser (int id) const;
    int addUser (std::unique_ptr<User> user);
    void setUser (int id, std::unique_ptr<User> user);
    void removeUser (int id);
    // Setters and Getters
    int getIdseq() const;
    void setIdseq(int idseq);
    const std::map<int,std::unique_ptr<User>>& getUsers() const;
    void setUsers(const std::map<int,std::unique_ptr<User>>& users);
  };

};

#endif
