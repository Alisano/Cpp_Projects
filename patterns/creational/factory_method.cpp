#include <iostream>

class IProduct {
    public:
        virtual ~IProduct() {}
        virtual void ProductMethod() = 0;
};

class ConcreteProduct1 : public IProduct {
    void ProductMethod() {
        std::cout << "Product1 doing something" << std::endl;
    }
};

class ConcreteProduct2 : public IProduct {
    void ProductMethod() {
        std::cout << "Product2 doing something" << std::endl;
    }
};

class Creator {
    virtual IProduct* FactoryMethod() = 0;
    public:
        virtual ~Creator() {}
        IProduct* NewProduct() {
            return FactoryMethod();
        }
};

class ConcreteCreator1 : public Creator {
    public:
        IProduct* FactoryMethod() {
            return new ConcreteProduct1();
        }
};

class ConcreteCreator2 : public Creator {
    public:
        IProduct* FactoryMethod() {
            return new ConcreteProduct2();
        }
};

int main() {
    Creator* creator1 = new ConcreteCreator1();
    IProduct* product1 = creator1->NewProduct();
    product1->ProductMethod();

    Creator* creator2 = new ConcreteCreator2();
    IProduct* product2 = creator2->NewProduct();
    product2->ProductMethod();

    delete creator1;
    delete creator2;
    delete product1;
    delete product2;

    return 0;
}