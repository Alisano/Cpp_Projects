#include <iostream>
#include <string>

class IMediator;

class Colleague {
    protected:
        IMediator* mediator;
    public:
        Colleague(IMediator* mediator) : mediator(mediator) {};
};

class ConcreteColleague1 : public Colleague {
    public:
        ConcreteColleague1(IMediator* mediator) : Colleague(mediator) {};
        void SetState(std::string message) {
            std::cout << "Colleague1 get message: " << message << std::endl;
        }
        void ChangeState(std::string message);
};

class ConcreteColleague2 : public Colleague  {
    public:
        ConcreteColleague2(IMediator* mediator) : Colleague(mediator) {};
        void SetState(std::string message) {
            std::cout << "Colleague2 get message: " << message << std::endl;
        }
        void ChangeState(std::string message);
};

class IMediator {
    public:
        virtual void ChangeState(Colleague* colleague, std::string message) = 0;
        virtual void SetColleague1(ConcreteColleague1 *c) = 0;
        virtual void SetColleague2(ConcreteColleague2 *c) = 0;
};

class ConcreteMediator : public IMediator {
    ConcreteColleague1* colleague1;
    ConcreteColleague2* colleague2;
    public:
        void SetColleague1(ConcreteColleague1 *c) override
        {
                colleague1=c;
        }
 
        void SetColleague2(ConcreteColleague2 *c) override
        {
                colleague2=c;
        }

        void ChangeState(Colleague* colleague, std::string message) override {
            if (colleague == colleague1) {
                colleague1->SetState(message);
            }
            else if (colleague == colleague2) {
                colleague2->SetState(message);
            }
        }
};

void ConcreteColleague1::ChangeState(std::string message) {
    mediator->ChangeState(this, message);
}

void ConcreteColleague2::ChangeState(std::string message) {
    mediator->ChangeState(this, message);
}

int main() {
    IMediator * mediator = new ConcreteMediator;
    ConcreteColleague1 c1(mediator);
    ConcreteColleague2 c2(mediator);
 
    mediator->SetColleague1(&c1);
    mediator->SetColleague2(&c2);
 
    c1.ChangeState("How are you?");
    c2.ChangeState("Fine, thanks");

    return 0;
}