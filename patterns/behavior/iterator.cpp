#include <iostream>
#include <vector>
#include <list>

class IContainer;

class IIterator {
    public:
        virtual IIterator* First() = 0;
        virtual IIterator* End() = 0;
        virtual IIterator* Next() = 0;
        virtual int Current() = 0;
        virtual bool isDone() = 0;
};

class IContainer {
    public:
        virtual void remove(int number) = 0;
        virtual void append(int number) = 0;
        virtual int find(int number) = 0;
        virtual int size() = 0;
        virtual int getnumber(int number) = 0;
        virtual IIterator* CreateIterator() = 0;
};

class Array : public IContainer {
    public:
        std::vector<int> vector;
        Array(std::initializer_list<int> concrete_vector) : vector(concrete_vector) {}
        void remove(int number) override{          
            std::vector<int>::iterator position = std::find(vector.begin(), vector.end(), number);
            vector.erase(position); 
        }
        void append(int number) override {
            vector.push_back(number);
        }
        int find(int number) override {
            std::vector<int>::iterator position = std::find(vector.begin(), vector.end(), number);
            return std::distance(vector.begin(), position);
        }
        int size() override {
            return vector.size();
        }
        IIterator* CreateIterator() override;
        int getnumber(int number) override {
            return vector[number];
        }
};

class LinkedList : public IContainer {
    public:
        std::list<int> list;
        LinkedList(std::initializer_list<int> concrete_list) : list(concrete_list) {}
        void remove(int number) override{          
            std::list<int>::iterator position = std::find(list.begin(), list.end(), number);
            list.erase(position);
        }
        void append(int number) override {
            list.push_back(number);
        }
        int find(int number) override {
            std::list<int>::iterator position = std::find(list.begin(), list.end(), number);
            return std::distance(list.begin(), position);
        }
        int size()override {
            return list.size();
        }
        IIterator* CreateIterator() override;
        int getnumber(int number) override {
            int i = 0;
            for(auto list_element: list) {
                if (i == number)
                    return list_element;
                i++;
            }
            return -1;
        }
};

class LinkedListIterator : public IIterator {
    int current_iterator_index = 0;
    IContainer* container;
    public:
        LinkedListIterator(IContainer* container) : container(container) {}
        IIterator* First() override {
            current_iterator_index = 0;
            return this;
        };
        IIterator* End() override {
            LinkedList* list = (LinkedList*) container;
            auto end_iterator = list->list.size();
            current_iterator_index = int(end_iterator - 1);
            return this;
        };
        IIterator*  Next() override {
            current_iterator_index++;
            return this;
        }
        int Current() override {
            LinkedList* list = (LinkedList*) container;
            int element_list = list->getnumber(current_iterator_index);
            return element_list;
        }
        bool isDone() override {
            LinkedList* list = (LinkedList*) container;
            auto end_iterator = list->list.size();
            if(current_iterator_index == int(end_iterator - 1))
                return true;
            else
                return false;
        };
};

class ArrayIterator : public IIterator {
    int current_iterator_index = 0;
    IContainer* container;
    public:
        ArrayIterator(IContainer* container) : container(container) {}
        IIterator* First() override {
            current_iterator_index = 0;
            return this;
        };
        IIterator*  End() override {
            Array* vector = (Array*) container;
            auto end_iterator = vector->vector.size();
            current_iterator_index = int(end_iterator - 1);
            return this;
        };
        IIterator*  Next() override {
            current_iterator_index++;
            return this;
        }
        int Current() override {
            Array* vector = (Array*) container;
            int element_list = vector->getnumber(current_iterator_index);
            return element_list;
        }
        bool isDone() override {
            Array* vector = (Array*) container;
            auto end_iterator = vector->vector.size();
            if(current_iterator_index == int(end_iterator - 1))
                return true;
            else
                return false;
        };
};

IIterator* LinkedList::CreateIterator() {
    return new LinkedListIterator(this);
}

IIterator* Array::CreateIterator() {
    return new ArrayIterator(this);
}

int main() {
    IContainer* vector_container = new Array({1, 2, 4});
    IIterator* vector_iterator = vector_container->CreateIterator();
    std::cout << vector_iterator->Current() << std::endl;
    vector_iterator->Next();
    std::cout << vector_iterator->Current() << std::endl;
    vector_iterator->Next();
    std::cout << vector_iterator->Current() << std::endl;

    IContainer* list_container = new LinkedList({3, 5, 6});
    IIterator* list_iterator = list_container->CreateIterator();
    std::cout << list_iterator->Current() << std::endl;
    list_iterator->Next();
    std::cout << list_iterator->Current() << std::endl;
    list_iterator->Next();
    std::cout << list_iterator->Current() << std::endl;
    return 0;
}
