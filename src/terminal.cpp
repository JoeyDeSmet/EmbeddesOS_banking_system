#include "./include/terminal.hpp"

namespace Banking {

  Terminal::Terminal( rtos::Mail<TerminalToBancontactMessage, 1> *acountMail, std::vector<Bancontact*> list) : _bancontacts(list)
  {
    m_thread.start(callback(Terminal::loop, this));
  }

  Terminal::~Terminal() { }

  void Terminal::loop(void* arg) {
    auto c_this = (Terminal *) arg;

    rtos::Mail<Banking::TerminalToBancontactMessage, 5> *bancontactMail = nullptr;

   
    

    while (true) {
      
      // get account data
      auto accountInfo = c_this->_accountInfo->try_alloc_for(rtos::Kernel::wait_for_u32_forever);
      // connect to 1 of 3 bancontacten
      while(bancontactMail == nullptr){
      for(int i = 0; i < 3; i++){
        bancontactMail = c_this->_bancontacts[i]->connect();
        if(bancontactMail != nullptr) break;
      }
    }

      //Get bankcotact mail
      auto msg = bancontactMail->try_alloc_for(rtos::Kernel::wait_for_u32_forever);

      // Create message 
      msg->name = accountInfo->name;
      msg->bank = accountInfo->bank;
      msg->to_name = accountInfo->to_name;
      msg->to_bank = accountInfo->to_bank;
      msg->mailToTerminal = &c_this->m_respons_messages;
      

      // Send in mail
      bancontactMail->put(msg);
      bancontactMail->free(msg);

      // print response

      c_this->_accountInfo->free(accountInfo);
      

    }
  }

}
