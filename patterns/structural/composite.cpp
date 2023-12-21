#include <iostream>
#include <vector>
#include <string>

class IComponent {
    public:
        virtual ~IComponent() {}
        virtual void Opertaion() = 0;
        virtual IComponent* GetChild(int i) { return NULL; }
        virtual void Remove(IComponent* component) {}
        virtual void Add(IComponent* component) {}
};

class Composite : public IComponent {
    std::vector<IComponent*> component_list;
    public:
        void Opertaion() override {
            for (auto iter = component_list.begin(); iter != component_list.end(); iter++) {
                (*iter)->Opertaion();
            }
        }
        IComponent* GetChild(int i) override {
            return component_list[i];
        }
        void Remove(IComponent* component)  override {
            for (auto iter = component_list.begin(); iter != component_list.end(); iter++) {
                if (*iter == component)
                    component_list.erase(iter);
            }
        }
        void Add(IComponent* component) override {
            component_list.push_back(component);
        }
};

class Leaf : public IComponent {
    std::string leaf_name;
    public:
        Leaf(std::string leaf_name) : leaf_name(leaf_name) {}
        void Opertaion() override {
            std::cout << "Leaf number: " << leaf_name << " doing something" << std::endl;
        }
};

int main() {
    IComponent* leaf1 = new Leaf("1");
    IComponent* leaf2 = new Leaf("2");
    IComponent* branch1 = new Composite();
    branch1->Add(leaf1);
    branch1->Add(leaf2);

    IComponent* leaf3 = new Leaf("3");
    IComponent* leaf4 = new Leaf("4");
    IComponent* branch2 = new Composite();
    branch2->Add(leaf3);
    branch2->Add(leaf4);

    IComponent* tree = new Composite();
    tree->Add(branch1);
    tree->Add(branch2);

    tree->Opertaion();

    delete leaf1;
    delete leaf2;
    delete leaf3;
    delete leaf4;
    delete branch1;
    delete branch2;
    delete tree;

    return 0;
}