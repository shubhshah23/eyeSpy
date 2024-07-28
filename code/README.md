# EyeSpy - Home Security System
The project is a home security system that runs on a raspberry pi 3. 
Using a passcode, the user can arm and disarm the security system.
When the system is armed, it senses for motion using a pir sensor and activates an alarm.

## Running the program
The project runs the best directly on the raspberry pi. Other comparable Unix/Linux can run 
the program gui, but will not be able to run the external hardware. 
The project can be run using command line arguments using the qt make commands. The steps are shown below.

Create QT project file:
```
$ qmake -project
```
Now we must open the 'filename'.pro file
``` 
$ nano 'filename'.pro
```
Now we must add the following lines to the 'filename'.pro file. They will go immediately after the INCLUDEPATH += line.
This will allow the project to communicate with the databse and also allows it to be compiled from the command line.
```
QT += gui widgets
QT += sql
```
Make the project platform specific:
```
$ qmake 'filename'.pro
```
This creates a Makefile, this will be used to run the project, but first one line needs to be modified. 
Open the MakeFile
```
$ nano Makefile
```
Ensure that in the LIBS line, the following libraries are included.
```
-lwiringPi -L/usr/local/ssl/lib -lssl -lcrypto -lstdc++fs
```
Now the following make command will comile the Makefile into an executable program.
```
$ make
```
Finally, we are ready to run the the executable. This is done using the command.
```
$ ./'filename'
```

## Hardware setup
The security system has two hardware peripherals wired to the raspberry pi's GPIO pins. 
The PIR must be wired to wiring pin 0 (GPIO 17) and the alaram must be wired to wiring pin 1 (GPIO 18).
This [Figure](https://pinout.xyz/) can be used for reference.
