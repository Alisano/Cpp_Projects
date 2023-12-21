#include <iostream>

class IAdapter {
    public:
        virtual void AdaptorOperartion() = 0;
};

class Adaptee {
    public:
        void AdapteeOperartion() {
            std::cout << "Adaptee doing something" << std::endl;
        }
};

class ConcreteAdapter : public IAdapter {
    Adaptee* adaptee = new Adaptee;
    public:
        void AdaptorOperartion() override {
            std::cout << "Adapter doing something" << std::endl;
            adaptee->AdapteeOperartion();
        }
};

int main() {
    IAdapter* adapter = new ConcreteAdapter;
    adapter->AdaptorOperartion();
    return 0;
}