#include "./include/bancontact.hpp"

namespace Banking {

  Bancontact::Bancontact(std::unordered_map<std::string, Bank*> banks) : m_banks(banks), m_max_connections(2) {
    // Start thread and give new instance as argument
    m_thread.start(callback(Bancontact::loop, this));
  }

  Bancontact::~Bancontact() { }

  rtos::Mail<TerminalToBancontactMessage, 5>* Bancontact::connect(void) {
    m_max_connections.acquire();
    return &m_messages;
  }

  void Bancontact::disconnect(void) {
    m_max_connections.release();
  }

  void Bancontact::loop(void* arg) {
    auto c_this = (Bancontact *) arg;

    while (true) {
      auto current_message = c_this->m_messages.try_get_for(rtos::Kernel::wait_for_u32_forever);

      printf("Received message\n");

      auto itr = c_this->m_banks.find(current_message->bank);
      if (itr != c_this->m_banks.end()) {
        printf("Bank exists\n");
        // Bank exists
        rtos::Mail<Banking::BancontactToBankMessage, 5U>* bank_mail = itr->second->connect();

        auto n_message = bank_mail->try_calloc_for(rtos::Kernel::wait_for_u32_forever);
        
        n_message->name = current_message->name;
        n_message->to_name = current_message->to_name;
        n_message->to_bank = current_message->to_bank;
        n_message->amount = current_message->amount;
        n_message->mail = &c_this->m_respons_messages;

        bank_mail->put(n_message);

        auto response = c_this->m_respons_messages.try_get_for(rtos::Kernel::wait_for_u32_forever);

        // TODO: message to terminal
        if (response->ok) {
          printf("Success\n");
        } else {
          printf("Fail\n");
        }

        c_this->m_respons_messages.free(response);

        itr->second->disconnect();
      }
    
      c_this->m_messages.free(current_message);
    }
  }  

}
