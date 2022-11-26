#include "./include/bankcontact.hpp"

namespace Banking {

  Bankcontact::Bankcontact(void) {
    // Start thread and give new instance as argument
    m_thread.start(callback(Bankcontact::loop, this));
  }

  Bankcontact::~Bankcontact() { }

  void Bankcontact::loop(void* arg) {
    auto c_this = (Bankcontact *) arg;

    while (true) {

    }
  }  

}
