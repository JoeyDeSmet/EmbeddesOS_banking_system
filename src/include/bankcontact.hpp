#pragma once
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed.h"
#include "rtos.h"

/** 
 * Bankcontact can only have 2 connetions
 */
namespace Banking {

  class Bankcontact {

    public:
      Bankcontact(void);
      ~Bankcontact();

    private:
      static void loop(void* arg);

    private:
      rtos::Thread m_thread;

  };

}
