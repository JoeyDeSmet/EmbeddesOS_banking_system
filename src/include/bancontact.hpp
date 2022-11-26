#pragma once
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed_rtos.hpp"

#include "bank.hpp"
#include <unordered_map>

/** 
 * Bancontact can only have 2 connetions
 */
namespace Banking {

  class Bancontact {

    public:
      Bancontact(std::unordered_map<std::string, Bank*> banks);
      ~Bancontact();

    public:
      rtos::Mail<TerminalToBancontactMessage, 5>* connect(void);
      void disconnect(void);

    private:
      static void loop(void* arg);

    private:
      std::unordered_map<std::string, Bank*> m_banks;

    private:
      rtos::Thread m_thread;
      rtos::Semaphore m_max_connections;

    private:
      rtos::Mail<TerminalToBancontactMessage, 5> m_messages;
      rtos::Mail<BankToBancontactMessage, 1> m_respons_messages;

  };

}
