#include <iostream>

class IImpelementor {
    public:
        virtual void OperationImpl() = 0;
};

class ConcreteImplementor1 : public IImpelementor {
    void OperationImpl() override {
        std::cout << "ConcreteImplementor1 doing something" << std::endl;
    }
};

class ConcreteImplementor2 : public IImpelementor {
    void OperationImpl() override {
        std::cout << "ConcreteImplementor2 doing something" << std::endl;
    }
};

class Abstraction {
    IImpelementor* implementor;
    public:
        Abstraction(IImpelementor* implementor) : implementor(implementor) {}
        void operation() {
            implementor->OperationImpl();
        }
        void SetImplementor(IImpelementor* implementor) {
            this->implementor = implementor;
        }
};

int main() {
    Abstraction* abstraction = new Abstraction(new ConcreteImplementor1);
    abstraction->operation();

    abstraction->SetImplementor(new ConcreteImplementor2);
    abstraction->operation();

    return 0;
}