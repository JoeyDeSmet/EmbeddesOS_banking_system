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

          if(message->to_bank == c_this->m_name){
            // If in the same bank add to money to user
            auto accountToPay = c_this->m_accounts.find(message->to_name);
            accountToPay->second += message->amount;
          }
          else{
            // add to list that will be processed on midnight
            BankToBankMessage nightListMsg;
            nightListMsg.amount = message->amount;
            nightListMsg.bank = message->to_bank;
            nightListMsg.name = message->to_name;

            c_this->m_nightProcess.push_back(nightListMsg);
          }

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

      // Check mail for message from other bank (non-blocking) 
      auto getMessage = bank_to_bank_mail.try_get();
      if(getMessage != nullptr) { 
        if(getMessage->bank == c_this->m_name){
        
          auto user = c_this->m_accounts.find(getMessage->name);
          user->second += getMessage->amount;

          printf("[%s] Processed night payment to [%s] %s\n",c_this->m_name.c_str(), getMessage->bank.c_str(), getMessage->name.c_str());
          
          bank_to_bank_mail.free(getMessage);
        }
      }
      
      // Send buffered messageds to other banks so that they can process it
      if(c_this->m_nightProcess.size() != 0){
        
        // change it to single value insted of loop?
        for(int i = c_this->m_nightProcess.size() - 1; i >= 0; i--){
          auto message = bank_to_bank_mail.try_calloc_for(rtos::Kernel::wait_for_u32_forever);
          message->amount = c_this->m_nightProcess[i].amount;
          message->bank = c_this->m_nightProcess[i].bank;
          message->name = c_this->m_nightProcess[i].name;

          bank_to_bank_mail.put(message);
          c_this->m_nightProcess.pop_back();
        }
      }

      Time::get_midnight_mutex()->unlock();
    }
  }

}
