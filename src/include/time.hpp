#pragma once
#define MBED_CONF_RTOS_PRESENT 1

#include "mbed_rtos.hpp"

#define SECONDS_PER_SECOND 288
#define SECONDS_IN_DAY 86400
#define TIME_7_HOUR 25200
#define TIME_20_HOUR 72000

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
      /**
       * @brief Get the current simulated time
       * 
       * This is a threadsafe call
       * 
       * @return uint representation of time in seconds
       */
      static uint get_time(void) {
        Get().m_time_mtx.trylock_for(rtos::Kernel::wait_for_u32_forever);
        uint current = Get().m_time;
        Get().m_time_mtx.unlock();
        return current;
      }

      /**
       * @brief Get the midnight mutex object
       * 
       * Used to get and than lock mutex of condition variable in order to use condition variable
       * 
       * @return rtos::Mutex* 
       */
      static rtos::Mutex* get_midnight_mutex(void) {
        return &Get().m_midnight_mtx;
      }

    private:
      Time (void);
      ~Time();

    private:
      static void loop(void* arg);

    private:
      // Time is in seconds
      uint m_time = 0;

    public:
      rtos::Mutex m_midnight_mtx;
      rtos::ConditionVariable m_is_midnight;

    private:
      rtos::Thread m_thread;
      rtos::Mutex m_time_mtx;

    public:
      // Delete copy and assign
      Time(Time &other) = delete;
      void operator=(const Time &) = delete;

  };

}
