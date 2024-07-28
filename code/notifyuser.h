/**
 * @author Shubh Shah, Ian Guenther Green
 * @brief header file for the notifyuser class that include functions and variables
 * Description: Sends an email to user using gmail smtp server. Used to notify users of a disturbance, or 
 * send them a report with when the previous alarm ringing instances
 * Date: Nov 29, 2022
 */

#ifndef NOTIFYUSER_H
#define NOTIFYUSER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "base64.h"
#include "para.h"
#include "time.h"

class NotifyUser {

public:
	// declerations for constructor and destructor
	NotifyUser() {}
	~NotifyUser() {}
	
	// function to email user
	void emailUser(char* message);

private:
	int my_strlen(char *str);
	
	// backend functions for connecting to open ssql server
	const char* GetIPAddress(const char* target_domain);
	int ConnectToServer(const char* server_address);
	char * MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset);

};

#endif // NOTIFYUSER_H
