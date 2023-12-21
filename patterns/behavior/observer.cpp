#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class ConcreteSubject;

class IObserver {
    public:
        virtual void Update(ConcreteSubject* subject) = 0;
};

class ISubject {
    protected:
        std::vector <IObserver*> observers;
    public:
        void Attach(IObserver* observer) {
            observers.push_back(observer);
        }
        void Dettach(IObserver* observer) {
            std::vector <IObserver*>::iterator position = std::find(observers.begin(), observers.end(), observer);
            observers.erase(position);
        }
        virtual void Notify() {};
};

class ConcreteSubject : public ISubject {
    std::string subjectState;
    public:
        std::string GetState() {
            return subjectState;
        }
        void SetState(std::string new_state) {
            subjectState = new_state;
            this->Notify();
        }
        void Notify () {
            for(IObserver* observer : observers) {
                observer->Update(this);
            }
        }
};

class ConcreteObserver1 : public IObserver {
    ISubject* subject;
    public:
        std::string observerState;
        ~ConcreteObserver1() {
            subject->Dettach(this);
        }
        ConcreteObserver1(ISubject* subject) {
            this->subject = subject;
            subject->Attach(this);
        }
        void Update(ConcreteSubject* subject) {
            observerState = subject->GetState();
        }
};

class ConcreteObserver2 : public IObserver {
    ISubject* subject;
    public:
        std::string observerState;
        ~ConcreteObserver2() {
            subject->Dettach(this);
        }
        ConcreteObserver2(ISubject* subject) {
            this->subject = subject;
            subject->Attach(this);
        }
        void Update(ConcreteSubject* subject) {
            observerState = subject->GetState();
        }
};

int main() {
    ConcreteSubject* subject = new ConcreteSubject();
    IObserver* observer1 = new ConcreteObserver1(subject);
    IObserver* observer2 = new ConcreteObserver2(subject);

    subject->SetState("Old State!");
    std::cout << subject->GetState() << std::endl;

    subject->SetState("New State!");
    std::cout << subject->GetState() << std::endl;

    delete subject;
    delete observer1;
    delete observer2;

    return 0;
}