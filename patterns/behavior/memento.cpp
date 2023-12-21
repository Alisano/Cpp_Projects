#include <iostream>

class Memento {
    int state;
    public:
        Memento() { state = 0; };
        Memento(int state_obj) : state(state_obj){}
        int GetState(){
            return state;
        }
};

class Originator{
    public:
        Originator(int state_obj) : state(state_obj){}
        int state;
        void SetMemento(Memento m){
            state = m.GetState();
        }
        Memento CreateMemento() {
            return Memento(state);
        }
};

class CareTaker {
    Memento mement_obj;
    public:
        CareTaker(Originator orig_obj){
            mement_obj = orig_obj.CreateMemento();
        }
};

int main() {
    Originator orig_obj(10);
    CareTaker caretaker_obj(orig_obj);

    std::cout << orig_obj.state << std::endl;
    orig_obj.state = 20;
    std::cout << orig_obj.state << std::endl;
    // orig_obj.SetMemento(caretaker_obj.mement_obj);
    // std::cout << orig_obj.state << std::endl;

    return 0;
}