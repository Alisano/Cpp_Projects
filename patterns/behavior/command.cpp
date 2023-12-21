#include <iostream>

class ICommand {
    public:
        virtual void Execute() = 0;
};

class Invoker {
    ICommand* command;
    public:
        void SetCommand(ICommand* concrete_command) {
            command = concrete_command;
        }
        void Execute() {
            command->Execute();
        }
};

class Receiver {
    public:
        void Action() {
            std::cout << "receiver\n";
        }
};

class ConcreteCommand1 : public ICommand {
    Receiver* receiver;
    public:
        ConcreteCommand1(Receiver* receiver) {
            this->receiver = receiver;
        }
        void Execute() {
            std::cout << "Command 1 is runing by ";
            receiver->Action();
        }
};

class ConcreteCommand2: public ICommand {
    Receiver* receiver;
    public:
        ConcreteCommand2(Receiver* receiver) {
            this->receiver = receiver;
        }
        void Execute() {
            std::cout << "Command 2 is runing by ";
            receiver->Action();
        }
};

int main() {
    Receiver* receiver = new Receiver;
    Invoker* invoker = new Invoker;

    ICommand* command1 = new ConcreteCommand1(receiver);
    ICommand* command2 = new ConcreteCommand2(receiver);

    invoker->SetCommand(command1);
    invoker->Execute();

    invoker->SetCommand(command2);
    invoker->Execute();

    delete receiver;
    delete invoker;

    return 0;
}