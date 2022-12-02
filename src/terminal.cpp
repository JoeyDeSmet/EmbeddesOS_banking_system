#include "./include/terminal.hpp"

namespace Banking {

  Terminal::Terminal(std::vector<Bancontact*> list) : _bancontacts(list)
  {
    m_thread.start(callback(Terminal::loop, this));
  }

  Terminal::~Terminal() { }

  rtos::Mail<TerminalToBancontactMessage, 5>* Terminal::connect(void) {
    printf("BEFORE ACQUIRE");
    m_max_connections.acquire();
    printf("AFTER ACQUIRE");
    return &_accountInfo; // Return pointer to mail
  }

  void Terminal::disconnect(void) {
    m_max_connections.release();
  }

  void Terminal::loop(void* arg) {
    auto c_this = (Terminal *) arg;

    

    while (true) {
      
      // get account data
      auto accountInfo = c_this->_accountInfo.try_get_for(rtos::Kernel::wait_for_u32_forever);
      printf("\n\nINSIDE TERMINAL\n\n\n\n");
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
      auto msg = bancontactMail->try_alloc_for(rtos::Kernel::wait_for_u32_forever);

      // Create message 
      msg->name = accountInfo->name;
      msg->bank = accountInfo->bank;
      msg->to_name = accountInfo->to_name;
      msg->to_bank = accountInfo->to_bank;
      msg->mailToTerminal = &c_this->m_respons_messages;

      // msg->name = accountInfo->from->name();
      // msg->bank = accountInfo->from->bank();
      // msg->to_name = accountInfo->to->name();
      // msg->to_bank = accountInfo->to->bank();

      msg->amount = accountInfo->amount;

      // Send in mail
      bancontactMail->put(msg);
      c_this->_accountInfo.free(accountInfo); // choose better name x)

      // print response
      auto response = c_this->m_respons_messages.try_get_for(rtos::Kernel::wait_for_u32_forever);

      if(response->ok){
        printf("[Terminal] Process Succeed");
      }
      else{
        printf("[Terminal] Process didn't Succeed");
      }

      c_this->m_respons_messages.free(response);
      
      c_this->_bancontacts[0]->disconnect();
    }
  }

}
