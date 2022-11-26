#include "./include/terminal.hpp"

namespace Banking {

  Terminal::Terminal(void) {
    m_thread.start(callback(Terminal::loop, this));
  }

  Terminal::~Terminal() { }

  void Terminal::loop(void* arg) {
    auto c_this = (Terminal *) arg;

    while (true) {

    }
  }

}
