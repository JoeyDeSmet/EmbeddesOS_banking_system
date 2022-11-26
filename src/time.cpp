#include "./include/time.hpp"

namespace Banking {

  Time::Time(void) : m_is_midnight(m_midnight_mtx) {
    m_thread.start(callback(Time::loop, this));
  }

  Time::~Time() { }

  void Time::loop(void* arg) {
    auto s_this = (Time *) arg;

    while (true) {
      s_this->m_midnight_mtx.lock();
      // Add time
      s_this->m_time_mtx.trylock_for(rtos::Kernel::wait_for_u32_forever);
      s_this->m_time = (s_this->m_time + SECONDS_PER_SECOND) % SECONDS_IN_DAY;
      s_this->m_time_mtx.unlock();

      if (s_this->m_time > TIME_20_HOUR || s_this->m_time < TIME_7_HOUR) {
        s_this->m_is_midnight.notify_all();
        s_this->m_midnight_mtx.unlock();
      }

      ThisThread::sleep_for(std::chrono::seconds(1));
    }
  }

}
