#include "./include/bank.hpp"

// Mail to use when bank communicate to each other
rtos::Mail<Banking::BankToBankMessage, 5> bank_to_bank_mail;

namespace Banking {

  Bank::Bank(std::string name, void (*add_users)(void* arg)) : m_name(name), m_max_connections(2) {
    add_users((void*) &m_accounts);

    m_thread.start(callback(Bank::loop, this));
    m_process_thread.start(callback(Bank::processes, this));
  }

  Bank::~Bank() { }

  void Bank::loop(void* arg) {
    auto c_this = (Bank *) arg;

    while (true) {
      // Get current message
      auto message = c_this->m_messages.try_get_for(rtos::Kernel::wait_for_u32_forever);

      printf("[%s] Transaction %s to %s amount: %i\n", c_this->m_name, message->name, message->to_name, message->amount);

      // Create a response struct
      auto response = message->mail->try_calloc_for(rtos::Kernel::wait_for_u32_forever);

      auto itr = c_this->m_accounts.find(message->name); // Check if account exists
      if (itr != c_this->m_accounts.end()) {
        // Acount exist

        if (itr->second > message->amount) { // Check if enough money
          itr->second -= message->amount;
          // TODO: Store somwhere and check bank
          response->ok = true;
        } else {
          response->ok = false;
        }

        message->mail->put(response);
      } else {
        // Acount does not exist
        response->ok = false;
        message->mail->put(response);
      }
    }
  }

  rtos::Mail<BancontactToBankMessage, 5>* Bank::connect(void) {
    m_max_connections.acquire(); // Make sure only 2 can connect at 1 time
    return &m_messages; // Return pointer to mail
  }

  void Bank::disconnect(void) {
    m_max_connections.release();
  }

  void Bank::processes(void* arg) {
    auto c_this = (Bank *) arg;

    while (true) {
      // Wait for midnight
      Time::get_midnight_mutex()->lock();
      Time::Get().m_is_midnight.wait();

      printf("[%s] It is midnight processing now\n", c_this->m_name.c_str());

      // TODO: Send data to other banks 

      Time::get_midnight_mutex()->unlock();
    }
  }

}
