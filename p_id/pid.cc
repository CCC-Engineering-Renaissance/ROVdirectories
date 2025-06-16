#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
//#include "../libraries/matplotlib-cpp/matplotlibcpp.h"
using namespace std;

double pid(double error, double previous) {
	static double integral = 0, derivative = 0;
	double kp = 0.50;
	double ki = 0.30;
	double kd = 0.001;
	double proportional = error;
	integral += error * 0.001; // Error * dt, where dt = 1 second a/ 1000
	derivative = (error - previous) / 0.001; // How much error did we correct since 1 second ago 
	return ((kp * proportional) + (ki * integral) + (kd * derivative));
}
// Note do the previous calculation outside the function

// Need this because our calculator is always in radians, we need to convert our degree value to radians before we plug it into our trig functions
inline double to_rad(double deg) {
	double rad = deg * (M_PI / 180);
	return rad;
}

double do_trig(double old_vec, double new_vec) {
	double new_angle = 0;
	new_angle = atan(
		(sin(to_rad(old_vec)) + sin(to_rad(new_vec)))
		 / 
		(cos(to_rad(old_vec)) + cos(to_rad(new_vec)))
		);	
	return new_angle * (180 / M_PI);
	//return to_deg(new_angle); // JIC 😉
}

double to_deg(double rad) {
	double angle = rad * (180 / M_PI);
	return angle;
}


int main() {
	double set_value = 0, sensor_curr = 0, previous_error = 0, error = 0;
	double new_vec = 0, old_vec = 0, old_old_vec = 0;
	
	ofstream test_file;
	test_file.open ("pid_data.txt");

	// TEST CODE FOR GRAPHING DATA	

	while (1) {
		cout << "Current Sensor Value: ";
		cin >> sensor_curr;
		if (!sensor_curr) {
			test_file.close();
			return 0;
		}

	old_vec = -5;
	error  = set_value - old_vec;
	new_vec = pid(error, previous_error);
	previous_error = error;
	test_file << old_vec << " , " << new_vec << endl;
	for (int i = 0; i < 25; i++) {
		cout << i << ". " << old_vec << " , " << new_vec << endl;
		old_vec = do_trig(old_vec, new_vec);
		cout << "Old vec = " << old_vec << endl;
		error = set_value - old_vec + error * 0.1;
		new_vec = pid(error, previous_error);
		previous_error = error;
		test_file << old_vec << " , " << new_vec << endl;
	}

	old_vec -= 180;	

	for (int i = 0; i < 50; i++) {
		cout << i << ". " << old_vec << " , " << new_vec << endl;
		old_vec = do_trig(old_vec, new_vec);
		cout << "Old vec = " << old_vec << endl;
		error = set_value - old_vec + error * 0.1;
		new_vec = pid(error, previous_error);
		previous_error = error;
		test_file << old_vec << " , " << new_vec << endl;
	}
	test_file.close();

}

