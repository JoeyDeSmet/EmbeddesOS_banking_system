

namespace Banking{

  class Account{

    public:
      Account(const char* name, const char* bank);
      Account(const char* name, const char* bank, double money);
      
      void add(double money);
      void sub(double money);

      const char* name(void);
      
    private:
      double _money = 0;
      const char* _name;
      const char* _bank;

  };
}