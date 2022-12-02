#pragma once

#include "mbed_rtos.hpp"
#include "account.hpp"

namespace Banking {

  struct BankToBancontactMessage {
    bool ok; // Indication if transation was succesfull
  };

  typedef struct BankToBancontactMessage;

  struct BankToBankMessage {
    std::string bank; // Name of the bank
    std::string name; // Name of account
    uint amount; // Amount to be added
  };

  struct BancontactToBankMessage {
    std::string name; // Name of account
    std::string to_name; // Name of money receiver
    std::string to_bank; // Name of the bank
    uint amount; // Amount of money
    rtos::Mail<BankToBancontactMessage, 1>* mail; // Pointer to mail
  };

  struct BancontactToTerminalMessage {

    bool ok; // Response to terminal

  }; 

  struct TerminalToBancontactMessage {
    std::string name;
    std::string bank;

    std::string to_name;
    std::string to_bank;

    uint amount;

    rtos::Mail<BancontactToTerminalMessage, 1> *mailToTerminal;
  };

  struct ToTerminalMessage{

    Account* from;
    Account* to;
    uint amount;
  };

  
}
