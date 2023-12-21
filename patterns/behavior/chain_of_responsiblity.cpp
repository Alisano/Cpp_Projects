#include <iostream>

class Handler {
    public:
        Handler* successor = NULL;
        virtual void HandleHelp(int request) = 0;
        void SetSuccessor(Handler* new_successor) {
            successor = new_successor;
        }
};

class ConcreteHandler1 : public Handler {
    void ShowHelp() {
        std::cout << "Hello I'am Yoba Handler 1, your number is 10" << std::endl;
    }
    public:
        ConcreteHandler1(){}
        void HandleHelp(int request) {
            if (request == 10) {
                ShowHelp();
            } else if (successor != NULL) {
                successor->HandleHelp(request);
            }
        }
};

class ConcreteHandler2 : public Handler{
    void ShowHelp() {
        std::cout << "Hello I'am Yoba Handler 2, your number is not 10" << std::endl;
    }
    public:
        ConcreteHandler2(){}
        void HandleHelp(int request) {
            if (request != 10) {
                ShowHelp();
            } else if (successor != NULL) {
                successor->HandleHelp(request);
            }
        }
};

int main() {
    Handler* handler1 = new ConcreteHandler1();
    Handler* handler2 = new ConcreteHandler2();
    handler1->SetSuccessor(handler2);
    // handler2->SetSuccessor(handler1);

    handler1->HandleHelp(0);
    handler1->HandleHelp(10);
    handler1->HandleHelp(20);

    return 0;
}