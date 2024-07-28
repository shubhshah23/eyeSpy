/**
 * @author Khanh Nguyen, Vicky Jiang
 * @brief Implementation file for control panels that includes
            button processing and interaction on the GUI.
 * Description: UI for the control panel, process button pressed,
           check for password, send detailed reports, etc.
 * Date: Nov 29, 2022
 */

#include "controlpanel.h"

using namespace std;

/**
 * Create control panel.
 * @author Khanh Nguyen
 * @brief Default constructor.
 */
ControlPanel::ControlPanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    //set default text on the display
    ui->Display->setText(defaultText);
    ui->CurrentState->setText(currState + "deactivated");

    //Assign numbers to its button
    QPushButton *numButs[10];
    for (int i=0;i <10; i++){
        QString numButtonName = "Button" + QString::number(i);
        //add the buttons to the button array
        numButs[i] = ControlPanel::findChild<QPushButton *>(numButtonName);
        //whenever the number button is pressed, execute function
        connect(numButs[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }
      
    //connect the button with the functions so that when a button is pressed, the function is executed
    connect(ui->Activate, SIGNAL(released()), this, SLOT(KeyPressed()));
    connect(ui->Deactivate, SIGNAL(released()), this, SLOT(KeyPressed()));
    connect(ui->ChangePass, SIGNAL(released()), this, SLOT(KeyPressed()));
    connect(ui->StopAlarm, SIGNAL(released()), this, SLOT(KeyPressed()));
    connect(ui->SendReport, SIGNAL(released()), this, SLOT(KeyPressed()));
    connect(ui->Ok, SIGNAL(released()), this, SLOT(ChangePass()));

}

/**
 * @author Khanh Nguyen
 * @brief Default destructor.
 */
ControlPanel::~ControlPanel()
{
    delete ui;
}

/**
 * Process when a number button is pressed and output the number pressed to the screen.
 * @author Khanh Nguyen
 * @brief Process when a button is pressed.
 * @param none.
 * @return output the number pressed to the screen.
 */
void ControlPanel::NumPressed(){
    QPushButton *numButton = (QPushButton *)sender();
    //get value of number from the button
    QString numVal = numButton->text();
    QString displayVal = ui->Display->text();

    //process when number is pressed
    if( (QString::compare(displayVal, defaultText, Qt::CaseInsensitive)==0) || (QString::compare(displayVal, falsePass, Qt::CaseInsensitive)==0) ){
        ui->Display->setText(numVal);
    }else {
        QString newDisplayVal = displayVal + numVal;
        int newPasscode = newDisplayVal.toInt();
        ui->Display->setText(QString::number(newPasscode));
    }
}

/**
 * Process when a non-number button (except for OK) is pressed and execute the function of each button.
 * @author Khanh Nguyen, Vicky Jiang
 * @brief Process when a button is pressed.
 * @param none.
 * @return execute the function of each button.
 */
void ControlPanel::KeyPressed(){
    QString displayVal = ui->Display->text();
    inputPass = displayVal.toInt();
    QPushButton *keyButton = (QPushButton *)sender();
    QString keyVal = keyButton->text();
    //when activate button is pressed
    if (QString::compare(keyVal, "activate", Qt::CaseInsensitive)==0){
        //put in verify password here
        if (inputPass == password.toInt()){
           activate = true;
           ui->CurrentState->setText(currState + "activated");
           ui->Display->setText("Success");
           attemptCount = 0;
           //set state of homesystem and alarm
           homesystem::setSystemState(true);
           homesystem::setAlarmState(true);
      	//if password is enter incorrectly, increment the count and ring alarm after hitting 5 times
        }else{
           ui->Display->setText(falsePass);
           if(attemptCount<4){
               //increment 1 for every failed attempt
               attemptCount= attemptCount + 1;
           }else{
               //alert for exceeding number of failed attempts
               //call alertFunction in HomeSystem
               homesystem::ringAlarm();
               attemptCount = 0;
           }
        }
    //when deactivate button is pressed
    }else if (QString::compare(keyVal, "deactivate", Qt::CaseInsensitive)==0){
        //put in verify password here
        if (inputPass == password.toInt()){
           activate = false;
           ui->CurrentState->setText(currState + "deactivated");
           ui->Display->setText("Success");
           attemptCount = 0;
           //set state of homesystem and alarm
           homesystem::setSystemState(false);
           homesystem::setAlarmState(false);
        //if password is enter incorrectly, increment the count and ring alarm after hitting 5 times
        }else{
           ui->Display->setText(falsePass);
           if(attemptCount<4){
               //increment 1 for every failed attempt
               attemptCount= attemptCount + 1;
           }else{
               //alert for exceeding number of failed attempts
               //call alertFunction in HomeSystem
               homesystem::ringAlarm();
               attemptCount = 0;
           }
        }
    // when send report button is pressed
    }else if (QString::compare(keyVal, "send report", Qt::CaseInsensitive)==0){
        //put in verify password here
        if (inputPass == password.toInt()){
            
           //sends detailed detection report
           homesystem::sendReport();
           
           ui->Display->setText("Report Sent");
           attemptCount = 0;
		
        //if password is enter incorrectly, increment the count and ring alarm after hitting 5 times
        }else{
           ui->Display->setText(falsePass);
           if(attemptCount<4){
               //increment 1 for every failed attempt
               attemptCount= attemptCount + 1;
           }else{
               //alert for exceeding number of failed attempts
               //call alertFunction in HomeSystem
               homesystem::ringAlarm();
               attemptCount = 0;
           }
        }
    //when the change password button is pressed
    }else if (QString::compare(keyVal, "change pass", Qt::CaseInsensitive)==0){
        //put in verify password here
        if (inputPass == password.toInt()){
           ui->Display->setText("Please enter new 6 digits passcode");
           changeAllow = true;
           attemptCount = 0;
        //if password is enter incorrectly, increment the count and ring alarm after hitting 5 times
        }else{
           ui->Display->setText(falsePass);
           if(attemptCount<4){
               //increment 1 for every failed attempt
               attemptCount= attemptCount + 1;
           }else{
               //alert for exceeding number of failed attempts
               //call alertFunction in HomeSystem
               homesystem::ringAlarm();
               attemptCount = 0;
           }
        }
    //when the stop alarm button is pressed
    }else if (QString::compare(keyVal, "stop alarm", Qt::CaseInsensitive)==0){
        //put in verify password here
        if (inputPass == password.toInt()){
           ui->Display->setText("Alarm Stopped");
           homesystem::stopAlarm();
           attemptCount = 0;
        //if password is enter incorrectly, increment the count and ring alarm after hitting 5 times
        }else{
           ui->Display->setText(falsePass);
           if(attemptCount<4){
               //increment 1 for every failed attempt
               attemptCount= attemptCount + 1;
           }else{
               //alert for exceeding number of failed attempts
               //call alertFunction in HomeSystem
               homesystem::ringAlarm();
               attemptCount = 0;
           }
        }
    }

}

/**
 * Process when the OK button is pressed and execute the function of each button.
 * @author Khanh Nguyen, Vicky Jiang
 * @brief process when OK button is clicked to change passcode.
 * @param none.
 * @return change passcode if conditions are met.
 */
void ControlPanel::ChangePass(){
  	//if change passcode is allowed and the new passcode is valid
    if (changeAllow == true && ui->Display->text().length() == 6){
        password = ui->Display->text();
        ui->Display->setText("Passcode changed");
        changeAllow = false;
    }else if(changeAllow == false){
        ui->Display->setText("Enter passcode and click Change Pass");
    }else if (ui->Display->text().length() != 6){
        ui->Display->setText("6 digits only");
    }
}
