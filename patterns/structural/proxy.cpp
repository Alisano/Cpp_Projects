#include <iostream>

class ISubject {
    public:
        virtual void SubjectMethod() = 0;
};

class RealSubject: public ISubject {
    public:
        void SubjectMethod() {
            std::cout << "Real Subject doing something" << std::endl;
        }
};

class Proxy : public ISubject {
    RealSubject* subject = new RealSubject;
    public:
        void SubjectMethod() {
            subject->SubjectMethod();
        }    
};

int main() {
    ISubject * subject = new Proxy;
    subject->SubjectMethod();
    return 0;
}