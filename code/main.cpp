/**
 * @author Khanh Nguyen
 * @brief main.
 * Description: main.
 * Date: Nov 29, 2022
 */
#include "homesystem.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //start up application
    homesystem::startUp(argc, argv);

    //clean up program
    homesystem::Delete();
    
    return 0;

}
