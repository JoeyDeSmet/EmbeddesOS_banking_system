#pragma once
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed_rtos.hpp"
#include "messages.hpp"

namespace Banking {

  class Terminal {

    public:
      Terminal(void);
      ~Terminal();

    private:
      static void loop(void* arg);

    private:
      rtos::Thread m_thread;

    private:
      rtos::Mail<BancontactToTerminalMessage, 1> m_respons_messages;
  };

}
