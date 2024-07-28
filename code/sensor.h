/**
 * @author Ian Guenther Green, Shubh Shah
 * @brief header file for the sensor class that include functions and variables
 * Description: Sensor Singleton class that sets up PIR sensor and returns its read value for if it senses movement
 * Date: Nov 29, 2022
 */

#ifndef SENSOR_H
#define SENSOR_H
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

class Sensor
{
private:
    // keeps track of wiring pin and if there is a disturbance
    int sensorPin;
    bool disturbance;

    static Sensor* _instance;

    //prevent copying and assignment
    Sensor(const Sensor& other) = delete;
    Sensor& operator=(const Sensor& other) = delete;
    Sensor();

public:

    static Sensor *instance();
    bool getDisturbance();
    
};

#endif // SENSOR_H
