#include <iostream>
#include <string>

class Product {
    std::string part1, part2, part3;
    public:
        Product(std::string part1, std::string part2, std::string part3) : part1(part1), part2(part2), part3(part3) {}
        void GetParts() {
            std::cout << part1 << ", " << part2 << ", " << part3 << std::endl;
        }
};

class Builder {
    public:
        virtual ~Builder() {}
        virtual Product* BuildPart() = 0;
};

class ConcreteBuilder1 : public Builder {
    public:
        Product* BuildPart() override {
            return new Product("part1 from builder 1", "part2 from builder 1", "part3 from builder 1");
        }
};

class ConcreteBuilder2 : public Builder {
    public:
        Product* BuildPart() override {
            return new Product("part1 from builder 2", "part2 from builder 2", "part3 from builder 2");
        }
};

class Director {
    Builder * builder;
    public:
        Director(Builder * builder) : builder(builder) {}
        Product* Construct() {
            return builder->BuildPart();
        }
        void SetBuilder(Builder* builder) {
            this->builder = builder;
        }
};

int main() {
    Builder* builder1 = new ConcreteBuilder1();
    Director* director = new Director(builder1);
    Product* product1 = director->Construct();
    product1->GetParts();

    Builder* builder2 = new ConcreteBuilder2();
    director->SetBuilder(builder2);
    Product* product2 = director->Construct();
    product2->GetParts();

    delete director;
    delete builder1;
    delete builder2;
    delete product1;
    delete product2;

    return 0;
}