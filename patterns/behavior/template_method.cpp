#include <iostream>

class AbstractClass {
    virtual void CocreteClassMethod1() = 0;
    virtual void CocreteClassMethod2() = 0;
    virtual void AbstractClassMethod() {
        std::cout << "Abstract method" << std::endl;
    }
    public:
        void template_method() {
            if (true) {
                CocreteClassMethod1();
                AbstractClassMethod();
                CocreteClassMethod2();
            }
        }
};

class ConcreteClass : public AbstractClass {
    void CocreteClassMethod1() {
        std::cout << "Hello World" << std::endl;
    }
    void CocreteClassMethod2() {
        std::cout << "Good bye" << std::endl;
    }
};

int main() {
   AbstractClass* object = new ConcreteClass;
   object->template_method();
   return 0;
}
