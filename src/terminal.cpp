#include "./include/terminal.hpp"

namespace Banking {

  Terminal::Terminal(void) {
    m_thread.start(callback(Terminal::loop, this));
  }

  Terminal::~Terminal() { }

  void Terminal::loop(void* arg) {
    auto c_this = (Terminal *) arg;

    while (true) {
      
      //Get bankcotact mail

      // Create message 
      // Create account object

      // Send in mail

      // print response

    }
  }

}
