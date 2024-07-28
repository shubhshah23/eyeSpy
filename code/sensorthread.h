/**
 * @author Khanh Nguyen, Ian Guenther Green
 * @brief header file for the thread of the sensor.
 * Description: Start the thread of the sensor so it runs while loop function waiting for signal from the sensor
 * Date: Nov 29, 2022
 */
#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H

#include <QThread>
#include "sensor.h"

class sensorThread : public QThread
{
    Q_OBJECT
public:
    explicit sensorThread(QObject *parent = 0);
    void run();
    bool exit = false;
};

#endif // SENSORTHREAD_H
