#include <iostream>

class IPrototype {
    public:
        virtual ~IPrototype() {}
        virtual IPrototype* Clone() = 0;
        virtual void PrototypeMethod() = 0;
};

class ConcretePrototype1 : public IPrototype {
    public:
        IPrototype* Clone() override {
            return new ConcretePrototype1(*this);
        }
        void PrototypeMethod() override {
            std::cout << "Prototype1 doing something" << std::endl;
        }
};

class ConcretePrototype2 : public IPrototype {
    public:
        IPrototype* Clone() override {
            return new ConcretePrototype2(*this);
        }
        void PrototypeMethod() override {
            std::cout << "Prototype2 doing something" << std::endl;
        }
};

class Client {
    IPrototype* prototype;
    public:
        Client(IPrototype* prototype) : prototype(prototype) {}
        void SetPrototype(IPrototype* prototype) {
            this->prototype = prototype;
        }
        IPrototype* Operation() {
            return prototype->Clone();
        }
};

int main() {
    IPrototype* prototype1 = new ConcretePrototype1();
    Client* client = new Client(prototype1);
    IPrototype* cloned_prototype1 = client->Operation();
    cloned_prototype1->PrototypeMethod();

    IPrototype* prototype2 = new ConcretePrototype2();
    client->SetPrototype(prototype2);
    IPrototype* cloned_prototype2 = client->Operation();
    cloned_prototype2->PrototypeMethod();

    delete client;
    delete prototype1;
    delete prototype2;
    delete cloned_prototype1;
    delete cloned_prototype2;

    return 0;
}