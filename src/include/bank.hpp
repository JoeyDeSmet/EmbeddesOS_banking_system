#define MBED_CONF_RTOS_PRESENT 1

#include "mbed.h"
#include "rtos.h"

namespace Banking {

  class Bank {

    public:
      Bank(void);
      ~Bank();

    private:
      static void loop(void* arg);

    private:
      rtos::Thread m_thread;

  };

}
