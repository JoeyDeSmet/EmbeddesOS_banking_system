#pragma once
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed_rtos.hpp"


#include <iostream>
#include <vector>

#include "messages.hpp"
#include "bancontact.hpp"
#include "account.hpp"

namespace Banking {

  class Terminal {

    public:
      Terminal(std::vector<Bancontact*> list);
      ~Terminal();

      rtos::Mail<TerminalToBancontactMessage, 1>* connect(void);
      void disconnect(void);  
    private:
      static void loop(void* arg);

    private:
      rtos::Thread m_thread;

    private:
      std::vector<Bancontact*> _bancontacts;

    private:
      rtos::Semaphore m_max_connections;
      rtos::Mail<TerminalToBancontactMessage, 1> _accountInfoMail;
      rtos::Mail<BancontactToTerminalMessage, 1> m_respons_messages;
  };

}
