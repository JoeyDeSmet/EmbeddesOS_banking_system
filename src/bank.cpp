#include "./include/bank.hpp"

namespace Banking {

  Bank::Bank(void) {
    m_thread.start(callback(Bank::loop, this));
  }

  Bank::~Bank() { }

  void Bank::loop(void* arg) {
    auto c_bank = (Bank *) arg;

    while (true) {

    }
  }

}
