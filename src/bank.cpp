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

      printf("[%s] Transaction %s to %s amount: %i\n", c_this->m_name.c_str(), message->name.c_str(), message->to_name.c_str(), message->amount);

      // Create a response struct and default to false
      auto response = message->mail->try_calloc_for(rtos::Kernel::wait_for_u32_forever);
      response->ok = false;


      // Check if account exists
      auto account = c_this->m_accounts.find(message->name); 
      if (account != c_this->m_accounts.end()) {
            
        // Check if account has enough money
        if (account->second > message->amount) { 
          // Remove money from account
          account->second -= message->amount;

          // TODO: Store somwhere and check to which bank to send
          response->ok = true;
        } 
      }

      message->mail->put(response);
      c_this->m_messages.free(message);
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
      // This thread will wait here for midnight signal
      Time::get_midnight_mutex()->lock();
      Time::Get().m_is_midnight.wait();

      printf("[%s] It is midnight processing now\n", c_this->m_name.c_str());

      // TODO: Send buffered messageds to other banks so that they can process it

      // TODO: Check mail for message from other bank (non-blocking) 

      Time::get_midnight_mutex()->unlock();
    }
  }

}
