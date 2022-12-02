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

Banking::Bank kbc("KBC", [](void* arg) {
  auto accounts = (std::unordered_map<std::string, int> *) arg;

  accounts->insert({ "Joey", 500 });
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
Banking::Bancontact bancontact0(banks);
Banking::Bancontact bancontact1(banks);
Banking::Bancontact bancontact2(banks);

// 3 terminals / server
std::vector<Banking::Bancontact*> bancontacts {
  &bancontact0,
  &bancontact1,
  &bancontact2
};

Banking::Terminal terminal1(bancontacts);
// Banking::Terminal terminal2(bancontacts);
// Banking::Terminal terminal3(bancontacts);


int main() {

  // Banking::Account Szymon("Szymon", "KBC", 500);
  // Banking::Account Joey("Joey", "KBC", 500);

  while (true) {
    uint current_time = Banking::Time::get_time();

    printf("\nCurrent time: %i:%i:%i\n", (current_time / 3600), ((current_time / 60) % 60), (current_time % 60));

    auto *m = terminal1.connect();
    
    // rtos::Mail<Banking::TerminalToBancontactMessage, 5U>* m = bancontact0.connect();

    auto message = m->try_calloc_for(rtos::Kernel::wait_for_u32_forever);
    // // Banking::TerminalToBancontactMessage* message = m->try_calloc_for(rtos::Kernel::wait_for_u32_forever);

    message->bank = "KBC";
    message->name = "Joey";
    message->to_bank = "KBC";
    message->to_name = "Szymon";
    message->amount = 50;

    m->put(message);
    

    // bancontact0.disconnect();
    terminal1.disconnect();

    ThisThread::sleep_for(std::chrono::milliseconds(1000));
  }
}

