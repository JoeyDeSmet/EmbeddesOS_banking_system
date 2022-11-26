#include "./include/time.hpp"

namespace Banking {

  Time::Time(void) {
    m_thread.start(callback(Time::loop, this));
  }

  Time::~Time() { }

  void Time::loop(void* arg) {
    auto s_this = (Time *) arg;

    while (true) {
      // Add time
      s_this->m_mtx.trylock_for(0xFFFFFFFF);
      s_this->m_time = (s_this->m_time + SECONDS_PER_SECOND) % SECONDS_IN_DAY;
      s_this->m_mtx.unlock();

      ThisThread::sleep_for(std::chrono::seconds(1));
    }
  }

}
