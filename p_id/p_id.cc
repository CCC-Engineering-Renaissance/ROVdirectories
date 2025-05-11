#include <iostream>
using namespace std;

double integral, derivative, previous, output = 0;
double kp = 0;
double ki = 0;
double kd = 0;

double pid(double set_value, double sensor_curr, double previous) {
	double error = set_value - sensor_curr;
	double proportional = error;
	integral += error * 0.001; // Error * dt, where dt = 2 seconds / 1000
	derivative = (error - previous) / 0.001; // How much error did we correct since 1 second ago
	previous = error;
	return ((kp * proportional) + (ki * integral) + (kd * derivative));
}
// Note do the previous calculation outside the function 

int main() {

}
