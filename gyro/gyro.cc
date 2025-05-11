#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <vector>
#include <algorithm>
#include <cmath>

#define MPU6050_ADDR 0x68
using namespace std;

int main() {
    int file;
    const char *bus = "/dev/i2c-1";

    if ((file = open(bus, O_RDWR)) < 0) {
        cerr << "Failed to open the I2C bus\n";
        return 1;
    }

    if (ioctl(file, I2C_SLAVE, MPU6050_ADDR) < 0) {
        cerr << "Failed to connect to the sensor\n";
        return 1;
    }

    // Wake up the MPU6050 (exit sleep mode)
    char config[2] = {0};
    config[0] = 0x6B;  // Power management register
    config[1] = 0x00;  // Set to zero (wakes up the MPU-6050)
    write(file, config, 2);

    usleep(100000); // wait 100ms

    // Calibration (Is not working as it should)
    int32_t gyroX_offset = 0, gyroY_offset = 0, gyroZ_offset = 0;
    int samples = 1000; // Increase samples for better averaging

    vector<int16_t> gyroX_vals, gyroY_vals, gyroZ_vals;

    cout << "Calibrating... Keep the sensor still.\n";

    for (int i = 0; i < samples; i++) {
        char reg = 0x43;
        write(file, &reg, 1);

        char data[6] = {0};
        read(file, data, 6);

        int16_t gyroX = (data[0] << 8) | data[1];
        int16_t gyroY = (data[2] << 8) | data[3];
        int16_t gyroZ = (data[4] << 8) | data[5];

        gyroX_vals.push_back(gyroX);
        gyroY_vals.push_back(gyroY);
        gyroZ_vals.push_back(gyroZ);

        usleep(3000); // Short delay between samples
   }

    // Sort values to remove outliers and calculate the median (less sensitive to extreme values)
    sort(gyroX_vals.begin(), gyroX_vals.end());
    sort(gyroY_vals.begin(), gyroY_vals.end());
    sort(gyroZ_vals.begin(), gyroZ_vals.end());

    // Median is more robust than mean for removing outliers
    gyroX_offset = gyroX_vals[samples / 2];
    gyroY_offset = gyroY_vals[samples / 2];
    gyroZ_offset = gyroZ_vals[samples / 2];

    cout << "Calibration complete!\n";
    cout << "Offsets - X: " << gyroX_offset
              << ", Y: " << gyroY_offset
              << ", Z: " << gyroZ_offset << "\n";

    // Start reading corrected values
    while (true) {
        char reg = 0x43;
        write(file, &reg, 1);

        char data[6] = {0};
        read(file, data, 6);

        int16_t gyroX = (((data[0] << 8) | data[1]) - gyroX_offset) / 16384.0;
        int16_t gyroY = ((data[2] << 8) | data[3]) - gyroY_offset / 16384.0;
        int16_t gyroZ = ((data[4] << 8) | data[5]) - gyroZ_offset / 16384.0;
	double accAngleX = atan(gyroY / sqrt(pow(gyroX, 2) + pow(gyroZ, 2))) * 180 /M_PI;
	double accAngleY= atan(gyroX / sqrt(pow(gyroY, 2) + pow(gyroZ, 2))) * 180 /M_PI;

	// Gyro orientation: https://lastminuteengineers.com/wp-content/uploads/arduino/MPU6050-Module-Gyroscope-Axis.jpg
	// Measured in degrees
 	cout << "Gyro X: " << gyroX
                  << " | Gyro Y: " << gyroY
                  << " | Gyro Z: " << gyroZ << endl;
cout << accAngleX << endl;
cout << accAngleY << endl;

        usleep(500000); // 500ms delay
    }

    close(file);
    return 0;
}
