#include <iostream>

class Context;

class IState {
    public:
        virtual void goNext(Context* context) = 0;
        virtual void getState() = 0;
};

class Context {
    public:
        IState* state;
        void goNext() {
            state->goNext(this);
        }
        void setState(IState* state) {
            this->state = state;
        }
        void getState() {
            state->getState();
        }
};

class ConcreteState3 : public IState {
    public:
        void goNext(Context* context);
        void getState() {
            std::cout << "Hi! I'm State 3" << std::endl;
        }
};

class ConcreteState2 : public IState {
    public:
        void goNext(Context* context) {
            context->setState(new ConcreteState3());
        }
        void getState() {
            std::cout << "Hi! I'm State 2" << std::endl;
        }
};

class ConcreteState1 : public IState {
    public:
        void goNext(Context* context) {
            context->setState(new ConcreteState2());
        }
        void getState() {
            std::cout << "Hi! I'm State 1" << std::endl;
        }
};

void ConcreteState3::goNext(Context* context) {
    context->setState(new ConcreteState1());
}

int main() {
    Context* context = new Context();
    context->setState(new ConcreteState1);

    context->getState();
    context->goNext();
    context->getState();
    context->goNext();
    context->getState();
    context->goNext();
    context->getState();

    return 0;
}