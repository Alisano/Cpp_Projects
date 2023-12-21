#include <iostream>

class IAbstractProductA {
    public:
        virtual void ProductMethod() = 0;
};

class ProductA1 : public IAbstractProductA {
    public:
        void ProductMethod() override {
            std::cout << "ProductA1 doing something" << std::endl;
        }
};

class ProductA2 : public IAbstractProductA {
    public:
        void ProductMethod() override {
            std::cout << "ProductA2 doing something" << std::endl;
        }
};

class IAbstractProductB {
    public:
        virtual void ProductMethod() = 0;
};

class ProductB1 : public IAbstractProductB {
    public:
        void ProductMethod() override {
            std::cout << "ProductB1 doing something" << std::endl;
        }
};

class ProductB2 : public IAbstractProductB {
    public:
        void ProductMethod() override {
            std::cout << "ProductB2 doing something" << std::endl;
        }
};

class IAbstractFactory {
    public:
        virtual ~IAbstractFactory() {} 
        virtual IAbstractProductA* CreateProductA() = 0;
        virtual IAbstractProductB* CreateProductB() = 0;
};

class ConcteteFactory1 : public IAbstractFactory {
    IAbstractProductA* CreateProductA() override {
        return new ProductA1();
    }
    IAbstractProductB* CreateProductB() override {
        return new ProductB1();
    }
};

class ConcteteFactory2 : public IAbstractFactory {
    IAbstractProductA* CreateProductA() override {
        return new ProductA2();
    }
    IAbstractProductB* CreateProductB() override {
        return new ProductB2();
    }
};

class Client {
    IAbstractProductA* productA; 
    IAbstractProductB* productB;
    public:
        Client(IAbstractFactory* factory) {
            productA = factory->CreateProductA();
            productB = factory->CreateProductB();
        }
        void MethodProductA() {
            productA->ProductMethod();
        }
        void MethodProductB() {
            productB->ProductMethod();
        }
};

int main() {
    IAbstractFactory* factory1 = new ConcteteFactory1();
    Client* client1 = new Client(factory1);
    client1->MethodProductA();
    client1->MethodProductB();

    IAbstractFactory* factory2 = new ConcteteFactory2();
    Client* client2 = new Client(factory2);
    client2->MethodProductA();
    client2->MethodProductB();

    delete factory1;
    delete client1;

    delete factory2;
    delete client2;

    return 0;
}