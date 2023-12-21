#include <iostream>
#include <string>
#include <cassert>

class Interface1 {
    public:
        virtual ~Interface1() {}
        virtual void ObjMethod() = 0;
};

class Interface2 {
    public:
        virtual ~Interface2() {}
        virtual void ObjMethod() = 0;
};

class Interface3 {
    public:
        virtual ~Interface3() {}
        virtual void ObjMethod() = 0;
};

class Obj1 : public Interface1 {
    public:
        void ObjMethod() override {
            std::cout << "Obj1 doing something" << std::endl;
        }
};

class Obj2 : public Interface2 {
    public:
        void ObjMethod() override {
            std::cout << "Obj2 doing something" << std::endl;
        }
};

class Obj3 : public Interface3 {
    public:
        void ObjMethod() override {
            std::cout << "Obj3 doing something" << std::endl;
        }
};

typedef enum {
    enum_obj1,
    enum_obj2,
    enum_obj3
} Objects;

class Facade {
    Obj1* obj1 = new Obj1();
    Obj2* obj2 = new Obj2();
    Obj3* obj3 = new Obj3();
    public:
        ~Facade() {
            delete obj1;
            delete obj2;
            delete obj3;
        }
        void SetMethod(Objects enum_obj) {
            switch (enum_obj)
            {
            case enum_obj1:
                obj1->ObjMethod();
                break;
            case enum_obj2:
                obj2->ObjMethod();
                break;
            case enum_obj3:
                obj3->ObjMethod();
                break;
            default:
                assert(("Invalid expression", false));
                break;
            }
        }
};

int main() {
    Facade* facade = new Facade();
    facade->SetMethod(enum_obj1);
    facade->SetMethod(enum_obj2);
    facade->SetMethod(enum_obj3);

    delete facade;

    return 0;
}
