#include <iostream>
#include <vector>

class IVisitor;

class IElement {
    public:
        virtual void Accept(IVisitor* visitor) = 0;
};

class ConcreteElement1 : public IElement {
    public:
        void Accept(IVisitor* visitor) override;
        void Operation1() {
            std::cout << "Is ConcreteElement1" << std::endl;
        }
};

class ConcreteElement2 : public IElement {
    public:
        void Accept(IVisitor* visitor) override;
        void Operation2() {
            std::cout << "Is ConcreteElement2" << std::endl;
        }
};

class IVisitor {
    public:
        virtual void VisitConcreteElement1(ConcreteElement1* element) = 0;
        virtual void VisitConcreteElement2(ConcreteElement2* element) = 0;
};

class ConcreteVisitor : public IVisitor {
    public:
        void VisitConcreteElement1(ConcreteElement1* element) {
            element->Operation1();
        }
        void VisitConcreteElement2(ConcreteElement2* element) {
            element->Operation2();
        };
};

void ConcreteElement1::Accept(IVisitor* visitor) {
    visitor->VisitConcreteElement1(this);
}

void ConcreteElement2::Accept(IVisitor* visitor) {
    visitor->VisitConcreteElement2(this);
}

int main() {
    IElement* element1 = new ConcreteElement1;
    element1->Accept(new ConcreteVisitor);

    IElement* element2 = new ConcreteElement2;
    element2->Accept(new ConcreteVisitor);

    return 0;
}