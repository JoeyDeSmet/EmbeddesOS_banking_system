

namespace Banking{

  class Account{

    public:
      Account(const char* name, const char* bank);
      Account(const char* name, const char* bank, double money);
      
      void add(double money);
      void sub(double money);

    private:
      double money = 0;

  };
}