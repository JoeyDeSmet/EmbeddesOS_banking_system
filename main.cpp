// Tell intellisense where using mbed rtos for syntax highlighting
#define MBED_CONF_RTOS_PRESENT 1

// Naming convention:
// m_ : member variables
// s_ : static variables
// c_ : context variables

#include "mbed.h"
#include <unordered_map>

#include "./src/include/bank.hpp"
#include "./src/include/bancontact.hpp"
#include "./src/include/terminal.hpp"
#include "./src/include/account.hpp"
#include "./src/include/messages.hpp"

void pay(std::string name, std::string bank_name, std::string to_name, std::string to_bank_name, double amount);


Banking::Bank kbc("KBC", [](void* arg) {
  auto accounts = (std::unordered_map<std::string, int> *) arg;

  accounts->insert({ "Joey", 1000 });
  accounts->insert({ "Szymon", 500 });
});

Banking::Bank belfius("Belfius", [](void* arg) {
  auto accounts = (std::unordered_map<std::string, int> *) arg;

  accounts->insert({ "Jens", 600 });
});

std::unordered_map<std::string, Banking::Bank*> banks = {
  { "KBC", &kbc },
  { "Belfius", &belfius }
};

// Create 3 bancontact instances
Banking::Bancontact bancontact1(banks);
Banking::Bancontact bancontact2(banks);
Banking::Bancontact bancontact3(banks);

// 3 terminals / bancontact
std::vector<Banking::Bancontact*> bancontacts {
  &bancontact1,
  &bancontact2,
  &bancontact3
};

Banking::Terminal terminal1(bancontacts[0]);
Banking::Terminal terminal2(bancontacts[0]);
Banking::Terminal terminal3(bancontacts[0]);

Banking::Terminal terminal5(bancontacts[1]);
Banking::Terminal terminal4(bancontacts[1]);
Banking::Terminal terminal6(bancontacts[1]);

Banking::Terminal terminal7(bancontacts[2]);
Banking::Terminal terminal8(bancontacts[2]);
Banking::Terminal terminal9(bancontacts[2]);

std::vector<Banking::Terminal*> terminalVector {
  &terminal1,
  &terminal2,
  &terminal3,
  &terminal4,
  &terminal5,
  &terminal6,
  &terminal7,
  &terminal8,
  &terminal9,
};


int main() {

  // Banking::Account Szymon("Szymon", "KBC", 500);
  // Banking::Account Joey("Joey", "KBC", 500);

  while (true) {
    uint current_time = Banking::Time::get_time();

    printf("\n\nCurrent time: %i:%i:%i\n", (current_time / 3600), ((current_time / 60) % 60), (current_time % 60));

    pay("Joey", "KBC", "Szymon", "KBC", 50);
    pay("Joey", "KBC", "Jens", "Belfius", 50);
    pay("Jens", "Belfius","Szymon", "KBC", 50);
    
    ThisThread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void pay(std::string name, std::string bank_name, std::string to_name, std::string to_bank_name, double amount){
  rtos::Mail<Banking::TerminalToBancontactMessage, 1>* connection = nullptr;
  Banking::Terminal * currentTerminal = nullptr;
  for(int i = 0; i < 9; i++){
    connection = terminalVector[i]->connect();
    currentTerminal = terminalVector[i];
    if(connection != nullptr) break;
  }

  auto message = connection->try_calloc_for(rtos::Kernel::wait_for_u32_forever);

  message->name = name;
  message->bank = bank_name;

  message->to_name = to_name;
  message->to_bank = to_bank_name;

  message->amount = amount;

  connection->put(message);

  currentTerminal->disconnect();

}