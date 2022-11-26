#pragma once
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed.h"
#include "rtos.h"
#include <unordered_map>

#include "time.hpp"

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

    private:
      static void loop(void* arg);
      static void processes(void* arg);

    private:
      std::string m_name;

    private:
      rtos::Thread m_thread;
      rtos::Thread m_process_thread;

    private:
      std::unordered_map<std::string, int> m_accounts;

  };

}
