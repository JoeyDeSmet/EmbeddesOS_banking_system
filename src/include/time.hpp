#define MBED_CONF_RTOS_PRESENT 1

#include "mbed.h"
#include "rtos.h"

#define SECONDS_PER_SECOND 288
#define SECONDS_IN_DAY 86400

// Time simulation 
// To get time in seconds call
// Banking::Time::get_time();

namespace Banking {

  class Time {

    public:
      static Time& Get(void) {
        static Time instance;
        return instance;
      }

    public:
      static uint get_time(void) {
        return Get().m_time;
      }

    private:
      Time (void);
      ~Time();

    private:
      static void loop(void* arg);

    private:
      // Time is in seconds
      uint m_time = 0;
      rtos::Thread m_thread;

    public:
      // Delete copy and assign
      Time(Time &other) = delete;
      void operator=(const Time &) = delete;

  };

}
