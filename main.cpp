// Tell intellisense where using mbed rtos for syntax highlighting
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed.h"
#include "./src/include/time.hpp"



int main() {

  while (true) {
    uint current_time = Banking::Time::get_time();

    printf("Current time: %i:%i:%i\n", (current_time / 3600), ((current_time / 60) % 60), (current_time % 60));
  
    ThisThread::sleep_for(std::chrono::milliseconds(500));
  }  

  return 0;
}

