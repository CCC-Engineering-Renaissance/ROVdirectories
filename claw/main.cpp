#include <iostream>
#include <SDL2/SDL.h>
#include <lgpio.h>
#include <thread>
#include <cmath>
#include "pca9685.h"
#include "xbox.h"
using namespace std;


const int THRUSTER1 = 26;  //19
const int THRUSTER2 = 3;   // 26
const int THRUSTER3 = 13;
const int THRUSTER4 = 0;


const int LFLAGS = 0;

double power = 0.5;
bool thing = true;

int h;
uint16_t  lerp(uint16_t a, uint16_t b, float t){
	return a + t * (b - a);
}

class Thruster{
	int pin;
	thread t;

	public:
	Thruster(){}
	Thruster(int newPin){pin = newPin; t = thread(this->loop, pin);}

	static void loop(int pin){
		cout << "Bruh" << endl;
		for (int i = 0; i < 100000; i++){
			lgTxServo(h, pin, 1500, 50, 0, 0); /* 1500 microseconds, 50 Hz */
		}

		usleep(1000000);

		// Send controller signal to thruster
		while(thing){
			lgTxServo(h, pin, 1500 + power * 400, 50, 0, 0); /* 1500 microseconds, 50 Hz */
		}	
	}

};

#define PCA9685_ADDR 0x40

int main(){
	h = lgGpiochipOpen(0); 
	SDL_Init(SDL_INIT_GAMECONTROLLER); // Initialize SDL

	SDL_GameController* controller = SDL_GameControllerOpen(0); // Get the controller

	if (!controller) {
		cerr << "Controller not detected: " << SDL_GetError() << endl;
		return -1;
	} else {
		cout << "Controller Connected: " << SDL_GameControllerName(controller) << endl;
	}

	pca9685 driver;
	driver.setFrequency(50);

	thread control(controls1, controller);
	uint16_t test = 1500;

	/* for (int i = 0; i < 10; i++){
	   driver.setPWM(i, 1000);
	   driver.setPWM(i, 2000);
	   driver.setPWM(i, 1500);
	   } */

	driver.setPWM(0, 1000);		// For some reason the motors are more responsive when doing this rather than in a for loop
	driver.setPWM(0, 2000);
	driver.setPWM(0, 1500);

	driver.setPWM(1, 1000);
	driver.setPWM(1, 2000);
	driver.setPWM(1, 1500);

	driver.setPWM(2, 1000);
	driver.setPWM(2, 2000);
	driver.setPWM(2, 1500);




	//lgTxServo(h, 6, 1000, 50, 0, 0);
	//lgTxServo(h, 6, 2000, 50, 0, 0);
	//lgTxServo(h, 6, 1500, 50, 0, 0);

	//	usleep(1000);

	//lgTxServo(h, 5, 1000, 50, 0, 0);
	//lgTxServo(h, 5, 1500, 50, 0, 0);


	cout << lgTxServo(h, THRUSTER1, 1500, 50, 0, 0) << endl;

	usleep(50000);
	uint16_t back = test;



	while (isRunning()){
		int x;
		double smoothing = 0.5;
		cout << lerp(back, test, smoothing) << '\r';
		cout.flush();

		//cin >> x;
		test = 1500 + 400 * back;
		for (int i = 0; i < 10; i++){
			driver.setPWM(i, test);
		}

//		back = lerp(back, test, smoothing);
//		cin >> x;
//		lgTxServo(h, THRUSTER1, x, 50, 0, 0);
//		//	usleep(100);
//		//lgTxServo(h, 5, x, 50, 0, 0);
//		//	usleep(100);

	}


	// Join Thread
	control.join(); //
	SDL_GameControllerClose(controller);
	SDL_Quit();

	lgGpiochipClose(h);
}
