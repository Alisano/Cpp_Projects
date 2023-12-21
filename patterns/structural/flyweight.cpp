#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

class IFlyWeight {
    public:
        virtual ~IFlyWeight() {}
        virtual void Operation(std::vector<std::string> extristic_state) = 0;
};

class ConcreteFlyWeight1 : public IFlyWeight {
    std::vector<std::string> intristic_state;
    public:
        std::vector<std::string> GetIntristicState () {
            return intristic_state;
        }
        ConcreteFlyWeight1(std::vector<std::string> intristic_state) {
            for (auto state: intristic_state)
            {   
                this->intristic_state.push_back(state);
            }
        }
        void Operation(std::vector<std::string> extristic_state) override {
            std::cout << "ConcreteFlyWeight1 doing something with intristic_state and extristic_state" << std::endl;
        }
};

class ConcreteFlyWeight2 : public IFlyWeight {
    std::vector<std::string> intristic_state;
    public:
        std::vector<std::string> GetIntristicState () {
            return intristic_state;
        }
        ConcreteFlyWeight2(std::vector<std::string> intristic_state) {
            for (auto state: intristic_state)
            {   
                this->intristic_state.push_back(state);
            }
        }
        void Operation(std::vector<std::string> extristic_state) override {
            std::cout << "ConcreteFlyWeight2 doing something with intristic_state and extristic_state" << std::endl;
        }
};

class UnhashedConcreteFlyWeight : public IFlyWeight {
    std::vector<std::string> all_state;
    public:
        UnhashedConcreteFlyWeight(std::vector<std::string> all_state) {
            for (auto state: all_state)
            {   
                this->all_state.push_back(state);
            }
            std::cout << "UnhashedConcreteFlyWeight is created" << std::endl;
        }
        void Operation(std::vector<std::string> extristic_state) override {
            std::cout << "UnhashedConcreteFlyWeight doing something with all_states" << std::endl;
        }
};

class FlyWeightFactory {
    public:
        void FlyWeightsInit(std::vector<std::vector<std::string>> intristic_states, FlyWeightFactory* flyweightfactory)
        {
            for (auto intristic_state: intristic_states)
            {
                flyweightfactory->GetFlyWeight(GetKeyFromState(intristic_state));
            }
        }
        std::vector<std::string> GetStateFromKey(std::string key) {
            std::vector<std::string> intristic_state;
            std::string delimiter = ", ";
            std::string token;
            size_t pos = 0;
            while ((pos = key.find(delimiter)) != std::string::npos) {
                token = key.substr(0, key.find(delimiter));
                intristic_state.push_back(token);
                key.erase(0, pos + delimiter.length());
            }
            intristic_state.push_back(key);
            return intristic_state;
        }
        std::string GetKeyFromState(std::vector<std::string> intristic_state) {
            std::string key = "";
            std::string delimiter = ", ";
            for (auto state: intristic_state)
            {   
                if (key == "") {
                    key.append(state);
                    continue;
                }
                key.append(delimiter);
                key.append(state);
            }
            return key;
        }
        IFlyWeight* GetFlyWeight(std::string key) {
            if (this->flyweights.find(key) == this->flyweights.end()) {
                std::cout << "FlyWeightFactory: Can't find a flyweight, creating new one." << std::endl;
                intristic_state = GetStateFromKey(key);
                switch (intristic_state.size())
                {
                case 5:
                    this->flyweights.insert(std::make_pair(key, new ConcreteFlyWeight2(intristic_state)));
                    std::cout << "FlyWeightFactory: ConcreteFlyWeight2 object is created" << std::endl;
                    break;
                
                default:
                    this->flyweights.insert(std::make_pair(key, new ConcreteFlyWeight1(intristic_state)));
                    std::cout << "FlyWeightFactory: ConcreteFlyWeight1 object is created" << std::endl;
                    break;
                }
            } else {
                std::cout << "FlyWeightFactory: Reusing existing flyweight." << std::endl;
            }
            return this->flyweights.at(key);
        }
        std::unordered_map<std::string, IFlyWeight*>  GetFlyWeights() { 
            return flyweights; 
        }
    private:
        std::unordered_map<std::string, IFlyWeight*> flyweights;
        std::vector<std::string> intristic_state;
};

int main() {
    std::vector<std::string> extristic_state = {"property10", "property11", "property12"};
    FlyWeightFactory* flyweights_factory = new FlyWeightFactory();
    flyweights_factory->FlyWeightsInit({
        {"property1", "property2", "property3"},
        {"property4", "property5", "property6"},
        {"property1", "property2", "property3"},
        {"property4", "property5", "property6"},
        {"property7", "property8", "property9"},
        {"property7", "property8", "property9", "property10", "property11"},
        {"property7", "property8", "property9", "property10", "property11"},
        {"property7", "property8", "property9", "property10", "property13"},
    }, flyweights_factory);
    std::unordered_map<std::string, IFlyWeight*> flyweights = flyweights_factory->GetFlyWeights();
    std::cout << "===========" << std::endl;

    for (auto flyweight: flyweights) {
        flyweight.second->Operation(extristic_state);
    }
    std::cout << "===========" << std::endl;

    IFlyWeight* unhashed_flyweight = new UnhashedConcreteFlyWeight({"property13", "property11", "property12", "property14"});
    unhashed_flyweight->Operation(extristic_state);

    delete flyweights_factory;
    delete unhashed_flyweight;

    return 0;
}