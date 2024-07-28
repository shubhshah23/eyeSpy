/**
 * @author Shubh Shah, Ian Guenther Green
 * @brief header file for infomration needed to format email in the notifyuser class. Includes needed definitions.
 * Date: Nov 29, 2022
 */

#ifndef PARA_H
#define  PARA_H

#ifdef  __cplusplus
extern "C" {
#endif

#define FROM "<cs3307eyespy@gmail.com>" // pass it to smtp from
#define TO "<cs3307eyespy@gmail.com>" // pass it to smtp from
#define MAIL_HEADER_FROM "system<cs3307eyespy@gmail.com>" // pass it to header  
#define MAIL_HEADER_TO "system<cs3307eyespy@gmail.com>"   // pass it to header
#define UID "gmail full id :cs3307eyespy@gmail.com"
#define PWD "gmail password :dlzfgourhrlnqvbu"
#define smtp_server_url "smtp.gmail.com"
#define domain_name "localhost"

#ifdef  __cplusplus
}
#endif

#endif  // PARA_H
