#pragma once

#include <iostream>

namespace Banking{

  class Account{

    public:
      Account(std::string name, std::string bank);
      Account(std::string name, std::string bank, double money);
      
      void add(double money);
      void sub(double money);

      std::string name(void);
      std::string bank(void);
      uint money(void);

    private:
      double _money = 0;
      std::string _name;
      std::string _bank;

  };
}