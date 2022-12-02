#include "account.hpp"


namespace Banking{

  Account::Account(std::string name, std::string bank){
    _name = name;
    _bank = bank;
  }

  /**
   * @brief Account object > change for usage in mail insted of 
   * BancontactToBankMessage and TerminalToBancontactMessage
   * 
   * @param name 
   * @param bank 
   * @param money
   */
  Account::Account(std::string name, std::string bank, double money){
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

  std::string Account::name(void) { return _name; }
  std::string Account::bank(void) { return _bank; }
  uint Account::money(void) { return _money; }
}