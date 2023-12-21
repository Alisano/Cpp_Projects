#include <iostream>

class Singleton {
    public:
        static Singleton* CreateObj(int data) {
            if(!_self) {
                _self = new Singleton(data);
            }
            return _self;
        }
    	static bool DeleteObj() {
            if(_self) {
                delete _self;
                _self = 0;
                return true;
            }
            return false;
        }
        int GetData() {
            return this->data;
        }
    private:
        static Singleton* _self;
        int data;

        Singleton(int data) : data(data) {}
        Singleton() = delete;
        Singleton(Singleton&) = delete;
        Singleton& operator=(Singleton&) = delete;
};

Singleton* Singleton::_self = NULL;

int main() {
    Singleton* single_obj = Singleton::CreateObj(5);
    single_obj->GetData();

    std::cout << "Data is: " << single_obj->GetData() << std::endl;
    std::cout << "Deleting result (1 is true, 0 is false): " << Singleton::DeleteObj() << std::endl;

    return 0;
};