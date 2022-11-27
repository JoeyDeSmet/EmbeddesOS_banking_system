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

    // Connect and disconnect are both used in Bank and Bancontact maby use inheritance to make this more DRY ?
    public:
      /**
       * @brief Connect to bancontact instance
       * Will block when more than 2 client are already connected.
       * 
       * @return rtos::Mail<TerminalToBancontactMessage, 5>* 
       * Where messages to bancontact have to be placed
       */
      rtos::Mail<TerminalToBancontactMessage, 5>* connect(void);
      
      /**
       * @brief Disconnect
       * Lets other thread blocking on connect, connect to bancontact
       */
      void disconnect(void);

    private:
      static void loop(void* arg);

    private:
      std::unordered_map<std::string, Bank*> m_banksList;

    private:
      rtos::Thread m_thread;
      rtos::Semaphore m_max_connections;

    private:
      rtos::Mail<TerminalToBancontactMessage, 5> m_messages;
      rtos::Mail<BankToBancontactMessage, 1> m_respons_messages;

  };

}
