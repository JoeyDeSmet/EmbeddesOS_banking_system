#include "./include/bank.hpp"

namespace Banking {

  Bank::Bank(std::string name, void (*add_users)(void* arg)) : m_name(name) {
    add_users((void*) &m_accounts);

    m_thread.start(callback(Bank::loop, this));
    m_process_thread.start(callback(Bank::processes, this));
  }

  Bank::~Bank() { }

  void Bank::loop(void* arg) {
    auto c_this = (Bank *) arg;

    while (true) {
      // TODO: handel data 
    }
  }

  void Bank::processes(void* arg) {
    auto c_this = (Bank *) arg;

    while (true) {
      // Wait for midnight
      Time::get_midnight_mutex()->lock();
      Time::Get().m_is_midnight.wait();

      printf("It is midnight processing now\n");

      // TODO: Send data to other banks 

      Time::get_midnight_mutex()->unlock();
    }
  }

}
