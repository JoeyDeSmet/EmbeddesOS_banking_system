#pragma once
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed_rtos.hpp"

#include <unordered_map>

#include "time.hpp"
#include "messages.hpp"

namespace Banking {

  class Bank {

    public:
      /**
       * @brief Construct a new Bank object
       * 
       * @param add_users 
       * Function to add users to bank
       */
      Bank(std::string name, void (*add_users)(void* arg));
      ~Bank();

    public:
      rtos::Mail<BancontactToBankMessage, 5>* connect(void);
      void disconnect(void);

    private:
      static void loop(void* arg);
      static void processes(void* arg);

    private:
      std::string m_name;

    private:
      rtos::Thread m_thread;
      rtos::Thread m_process_thread;

    private:
      rtos::Semaphore m_max_connections;
      // Mail where Bancontact communicates with bank
      rtos::Mail<BancontactToBankMessage, 5> m_messages;

    private:
      // map with name of account and amount of money
      std::unordered_map<std::string, int> m_accounts;

  };

}
