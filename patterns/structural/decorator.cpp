#include <iostream>

class IComponent {
    public:
        virtual ~IComponent() {}
        virtual void Opertaion() = 0;
};

class ConcreteComponent1: public IComponent {
    void Opertaion() override {
        std::cout << "ConcreteComponent1 doing something" << std::endl;
    } 
};

class ConcreteComponent2: public IComponent {
    void Opertaion() override {
        std::cout << "ConcreteComponent2 doing something" << std::endl;
    } 
};

class Decorator : public IComponent {
    IComponent* component;
    public:
        Decorator(IComponent* component) : component(component) {}
        void SetComponent(IComponent* component) {
            this->component = component;
        }
        virtual void Opertaion() override {
            component->Opertaion();
        };
};

class ConcreteDecorator1 : public Decorator {
    public:
        ConcreteDecorator1(IComponent* component) : Decorator(component) {} 
        void Opertaion() override {
            std::cout << "Decorator1 doing something" << std::endl;
            Decorator::Opertaion();
        } 
};

class ConcreteDecorator2 : public Decorator {
    public:
        ConcreteDecorator2(IComponent* component) : Decorator(component) {} 
        void Opertaion() override {
            std::cout << "Decorator2 doing something" << std::endl;
            Decorator::Opertaion();
        } 
};

int main() {
            IComponent* comp1 = new ConcreteComponent1();
            Decorator* decorator1 = new ConcreteDecorator1(comp1);
            decorator1->Opertaion();
            std::cout << "=================" << std::endl;

            Decorator* decorator2 = new ConcreteDecorator2(decorator1);
            decorator2->Opertaion();
            std::cout << "=================" << std::endl;

            IComponent* comp2 = new ConcreteComponent2();
            decorator1->SetComponent(comp2);
            decorator1->Opertaion();
            std::cout << "=================" << std::endl;

            decorator2->SetComponent(comp2);
            decorator2->Opertaion();

            delete comp1;
            delete comp2;
            delete decorator1;
            delete decorator2;

    return 0;
}