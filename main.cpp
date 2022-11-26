// Tell intellisense where using mbed rtos for syntax highlighting
#define MBED_CONF_RTOS_PRESENT 1

// Naming convention:
// m_ : member variables
// s_ : static variables
// c_ : context variables

#include "mbed.h"
#include "./src/include/bank.hpp"

Banking::Bank kbc("KBC", [](void* arg) {
  auto accounts = (std::unordered_map<std::string, int> *) arg;

  accounts->insert({ "Joey", 500 });
  accounts->insert({ "Szymon", 500 });
});

Banking::Bank belfius("Belfius", [](void* arg) {
  auto accounts = (std::unordered_map<std::string, int> *) arg;

  accounts->insert({ "Jens", 600 });
});

int main() {

  while (true) {
    uint current_time = Banking::Time::get_time();

    printf("\nCurrent time: %i:%i:%i\n", (current_time / 3600), ((current_time / 60) % 60), (current_time % 60));
  
    ThisThread::sleep_for(std::chrono::milliseconds(1000));
  }  

  return 0;
}

