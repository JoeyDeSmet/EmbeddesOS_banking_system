#include "./include/bancontact.hpp"

namespace Banking {

  Bancontact::Bancontact(std::unordered_map<std::string, Bank*> banks) : m_banksList(banks), m_max_connections(2) {
    // Start thread and give new instance as argument
    m_thread.start(callback(Bancontact::loop, this));
  }

  Bancontact::~Bancontact() { }

  rtos::Mail<TerminalToBancontactMessage, 5>* Bancontact::connect(void) {
    m_max_connections.acquire(); // Make sure only 2 can connect at 1 time
    return &m_messages; // Return pointer to mail
  }

  void Bancontact::disconnect(void) {
    m_max_connections.release();
  }

  void Bancontact::loop(void* arg) {
    auto c_this = (Bancontact *) arg;

    while (true) {
      // Wait blocking for a message from terminal
      auto current_message = c_this->m_messages.try_get_for(rtos::Kernel::wait_for_u32_forever);

      printf("Received message\n");

      // check if the bank exists in the list
      auto bank = c_this->m_banksList.find(current_message->bank); 
      if (bank != c_this->m_banksList.end()) {
        // Get Bancontact -> Bank bus and alocate all data
        rtos::Mail<Banking::BancontactToBankMessage, 5U>* bank_mail = bank->second->connect();
        
        // Change it to accout object
        auto n_message = bank_mail->try_calloc_for(rtos::Kernel::wait_for_u32_forever);
        
        n_message->name = current_message->name;
        n_message->to_name = current_message->to_name;
        n_message->to_bank = current_message->to_bank;
        n_message->amount = current_message->amount;

        // mail to let bank know where to send response
        n_message->mail = &c_this->m_respons_messages;
        bank_mail->put(n_message);

        // Get response from bank
        auto bankResponse = c_this->m_respons_messages.try_get_for(rtos::Kernel::wait_for_u32_forever);
        // send response to terminal
        // auto terminalResponse = current_message->mailToTerminal->try_alloc_for(rtos::Kernel::wait_for_u32_forever);

        if (bankResponse->ok) {
          // terminalResponse->ok = true;
          printf("Success\n");
        } else {
          // terminalResponse->ok = false;
          printf("Fail\n");
        }

        // current_message->mailToTerminal->put(terminalResponse);
        c_this->m_respons_messages.free(bankResponse);

        bank->second->disconnect();
      }
    
      c_this->m_messages.free(current_message);
    }
  }  

}
