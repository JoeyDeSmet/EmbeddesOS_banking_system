#include "account.hpp"


namespace Banking{


  Account::Account(const char* name, const char* bank){
    _name = name;
    _bank = bank;
  }

  Account::Account(const char* name, const char* bank, double money){
    _name = name;
    _bank = bank;
    _money = money;
  }

  void Account::add(double amount){
    _money += amount;
  }

  void Account::sub(double amount){
    _money -= amount;
  }
}