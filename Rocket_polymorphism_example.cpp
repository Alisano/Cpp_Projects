//Author Alexander Popov
//Here is example of the polymorphism by upcast and virtual functions
#include <iostream>

//Here is the interface for different implementation of temp() and speed() functions
class Engine_param {
	public:
	virtual int temp(int time, int thrust_engine) = 0;
	virtual int speed(int time, int thrust_engine) = 0;
};

//Here classes describe different implementation of temp() and speed() functions
class Engine_for_Angara: public Engine_param {
	int temp(int time, int thrust_engine){
		return time*thrust_engine*thrust_engine/2; //This expression is taken as an example
	}
	int speed(int time, int thrust_engine){
		return thrust_engine*time;  //This expression is taken as an example
	}
};

class Engine_for_Souz: public Engine_param {
	int temp(int time, int thrust_engine){
		return time*2*thrust_engine+thrust_engine;
	}
	int speed(int time, int thrust_engine){
		return time*thrust_engine*10;
	}
};

//Here we have a Rocket class, which uses a Engine_param class, the methods of which we can override
class Rocket {
	Engine_param* engine_of_rocket = nullptr;
	public:
	~Rocket() {
		delete engine_of_rocket;
	}
	void set_engine(Engine_param* engine_type){
		engine_of_rocket = engine_type; 
	};
	void status(int time,  int thrust_engine){ //Here is an example of how we can use the methods of the Engine_param class
		if (engine_of_rocket->temp(time, thrust_engine) > 1000) {
			std::cout<<"Warning!!!High temperature of engine"<<std::endl;
		}
		else {
			std::cout<<"Temperature is OK"<<std::endl;
		}
		
		if (engine_of_rocket->speed(time, thrust_engine) > 2000) {
			std::cout<<"Warning!!! Velosity is high, please reduce the speed"<<std::endl;
		}
		else {
			std::cout<<"Speed is OK"<<std::endl;
		}
	}
};

int main () {
	Rocket Angara;
	Rocker Soyz;
	//Here we can override the methods of the Rocket class
	Angara.set_engine(new Engine_for_Angara);
	Soyz.set_engine(new Engine_for_Souz);
	Angara.status(100,20);
	Soyz.status(50,10);
	return 0;
}
