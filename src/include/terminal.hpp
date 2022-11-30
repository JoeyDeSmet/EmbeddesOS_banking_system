#pragma once
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed_rtos.hpp"
#include "messages.hpp"

#include "bancontact.hpp"

namespace Banking {

  class Terminal {

    public:
      Terminal( rtos::Mail<TerminalToBancontactMessage, 1>* acountMail, std::vector<Bancontact*> list);
      ~Terminal();

    private:
      static void loop(void* arg);

    private:
      rtos::Thread m_thread;

    private:
      std::vector<Bancontact*> _bancontacts;

    private:
      // use this mail to send account info
      rtos::Mail<TerminalToBancontactMessage, 1>* _accountInfo;
      rtos::Mail<BancontactToTerminalMessage, 1> m_respons_messages;
  };

}
