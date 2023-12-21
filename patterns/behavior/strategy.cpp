#include <iostream>
#include <string>

class Context;

class IStrategy {
    public:
        virtual void execute(Context* context) = 0;
};

class ConcteteStrategy1 : public IStrategy {
    public:
        void execute(Context* context);
};

class ConcteteStrategy2 : public IStrategy {
    public:
        void execute(Context* context);
};

enum Algorithms { QuickSort, BubbleSort };

class Context {
    IStrategy* strategy;
    Algorithms algorithm;
    public:
        Context(Algorithms algorithm_var) : algorithm(algorithm_var){};
        void execute() {
            switch (algorithm)
            {
                case QuickSort:
                    strategy = new ConcteteStrategy1();
                    strategy->execute(this);
                    break;

                case BubbleSort:
                    strategy = new ConcteteStrategy2();
                    strategy->execute(this);
                    break;
                
                default:
                    throw("Error algorithm");
                    break;
            }
        }
        void printHello() {
            std::cout << "Hello";
        }
};

void ConcteteStrategy1::execute(Context* context) {
            context->printHello();
            std::cout << " Quick Sort" << std::endl;
}

void ConcteteStrategy2::execute(Context* context) {
            context->printHello();
            std::cout << " Bubble Sort" << std::endl;
}

int main() {
    Algorithms algorithm = QuickSort;
    Context* context = new Context(algorithm);
    context->execute();
    return 0;
}