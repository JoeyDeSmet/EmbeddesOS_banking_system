#include "./include/terminal.hpp"

namespace Banking {

  Terminal::Terminal(std::vector<Bancontact*> list) : _bancontacts(list), m_max_connections(1)
  {
    m_thread.start(callback(Terminal::loop, this));
  }

  Terminal::~Terminal() { }

  rtos::Mail<TerminalToBancontactMessage, 1>* Terminal::connect(void) {
    m_max_connections.acquire();
    return &_accountInfoMail; // Return pointer to mail
  }

  void Terminal::disconnect(void) {
    m_max_connections.release();
  }

  void Terminal::loop(void* arg) {
    auto c_this = (Terminal *) arg;

    

    while (true) {
      
      // get account data
      auto accountInfo = c_this->_accountInfoMail.try_get_for(rtos::Kernel::wait_for_u32_forever);
      
      rtos::Mail<Banking::TerminalToBancontactMessage, 5> *bancontactMail = nullptr;
      Banking::Bancontact * banConnection = nullptr;
      // connect to 1 of 3 bancontacten
      // while(bancontactMail == nullptr){
      //   for(int i = 0; i < 3; i++){
      //     bancontactMail = c_this->_bancontacts[i]->connect();
          
      //     if(bancontactMail != nullptr){
      //       banConnection = c_this->_bancontacts[i];
      //       break;
      //     }
      //   }
      // }

      bancontactMail = c_this->_bancontacts[0]->connect();
      //Get bankcotact mail
      auto msg = bancontactMail->try_calloc_for(rtos::Kernel::wait_for_u32_forever);

      // Create message 
      msg->name = accountInfo->name;
      msg->bank = accountInfo->bank;
      msg->to_name = accountInfo->to_name;
      msg->to_bank = accountInfo->to_bank;
      msg->mailToTerminal = &c_this->m_respons_messages;

      // Account object
      // msg->name = accountInfo->from->name();
      // msg->bank = accountInfo->from->bank();
      // msg->to_name = accountInfo->to->name();
      // msg->to_bank = accountInfo->to->bank();

      msg->amount = accountInfo->amount;

      // Send in mail
      bancontactMail->put(msg);
      
      // print response
      auto response = c_this->m_respons_messages.try_get_for(rtos::Kernel::wait_for_u32_forever);

      if(response->ok){
        printf("[Terminal] Process Succeed\n");
      }
      else{
        printf("[Terminal] Process didn't Succeed\n");
      }

      c_this->m_respons_messages.free(response);
      c_this->_bancontacts[0]->disconnect();
      c_this->_accountInfoMail.free(accountInfo); // choose better name x)
    }
  }

}
