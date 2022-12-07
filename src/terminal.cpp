#include "./include/terminal.hpp"

namespace Banking {

  Terminal::Terminal(Bancontact* bancontact) : _bancontact(bancontact), m_max_connections(1)
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

      //Get bankcotact mail
      auto bancontactMail = c_this->_bancontact->connect();
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
      c_this->_bancontact->disconnect();
      c_this->_accountInfoMail.free(accountInfo);
    }
  }

}
