/**
 * @author Shubh Shah, Ian Guenther Green
 * @brief implementation file for the notifyuser class that include functions and variables
 * Description: Sends an email to user using gmail smtp server. Used to notify users of a disturbance, or 
 * send them a report with when the previous alarm ringing instances
 * Code adapted from @link https://codevlog.com/gmailsmtp-gmail-com-using-c-programming-ssl/118
 * Date: Nov 29, 2022
 */

#include "notifyuser.h"
using namespace std;

/**
 * Function for determining the length of a character string
 * Used in place of strlen as this was giving segmentation fault
 * @brief Find length of string.
 * @param character string str.
 * @return length of the input string.
 */
int NotifyUser::my_strlen(char *str) {

	int rv;
	
    // counts the number of chracters in the string
	for (rv=0; str[rv] != 0; rv++) {}
    
	return rv;
}

/**
 * Function for getting the system IP address
 * @brief Gets IP address.
 * @param character string target_domain with the target domain.
 * @return character string with the ip address.
 */
const char* NotifyUser::GetIPAddress(const char* target_domain) {
    const char* target_ip;
    struct in_addr *host_address;
    struct hostent *raw_list = gethostbyname(target_domain);
    int i = 0;
    for (i; raw_list->h_addr_list[i] != 0; i++) {
        host_address = (in_addr*)raw_list->h_addr_list[i];
        target_ip = inet_ntoa(*host_address);
    }
    return target_ip;
}

/**
 * Function for connecting to server
 * @brief Connects to server.
 * @param character string server_address with the server address.
 * @return int containing the server socket.
 */
int NotifyUser::ConnectToServer(const char* server_address) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(465);
    if (inet_pton(AF_INET, GetIPAddress(server_address), &addr.sin_addr) == 1) {
        connect(socket_fd, (struct sockaddr *)&addr, sizeof (addr));
    }
    return socket_fd;
}

/**
 * Function for creating the mail header for the email
 * @brief Creates mail header.
 * @param character strings containing from email, to email, subject line, mime_type, anc character set.
 * @return character string containing the email header.
 */
char * NotifyUser::MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset) {

    time_t now;
    time(&now);
    char *app_brand = "EyeSpy Home Security System";
    char* mail_header = NULL;
    char date_buff[26];
    char Branding[6 + my_strlen(date_buff) + 2 + 10 + my_strlen(app_brand) + 1 + 1];
    char Sender[] = "FROM: system<cs3307eyespy@gmail.com>\r\n";
	char Recip[] = "To: User<cs3307eyespy@gmail.com>\r\n";
	char Subject[] = "Subject: EyeSpy Security System Notification\r\n";
	char mime_data[] = "MIME-Version: 1.0\r\nContent-type: text/plain; charset=US-ASCII\r\n\r\n";

    strftime(date_buff, (33), "%a , %d %b %Y %H:%M:%S", localtime(&now));

    sprintf(Branding, "DATE: %s\r\nX-Mailer: %s\r\n", date_buff, app_brand);

    int mail_header_length = my_strlen(Branding) + my_strlen(Sender) + my_strlen(Recip) + my_strlen(Subject) + 10;

    mail_header = (char*) malloc(mail_header_length * sizeof (char));
    
    memcpy(&mail_header[0], &Branding, my_strlen(Branding));
    memcpy(&mail_header[0 + my_strlen(Branding)], &Sender, my_strlen(Sender));
    memcpy(&mail_header[0 + my_strlen(Branding) + my_strlen(Sender)], &Recip, my_strlen(Recip));
    memcpy(&mail_header[0 + my_strlen(Branding) + my_strlen(Sender) + my_strlen(Recip)], &Subject, my_strlen(Subject));
    return mail_header;
    
}

/**
 * Function for emailing user using Google smtp and open ssl
 * @brief Emails the user.
 * @param character string message containing the email body.
 * @return none.
 */
void NotifyUser::emailUser(char* message) {
    BIO *obj_bio = NULL;
    BIO *obj_out = NULL;
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    int connceted_fd = 0;
    int ret, i;

    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

    obj_bio = BIO_new(BIO_s_file());
    obj_out = BIO_new_fp(stdout, BIO_NOCLOSE);

    // only runs if connection to ssl was successful
    if (SSL_library_init() < 0) {
        BIO_printf(obj_out, "OpenSSL not initialize");
    } else {
        method = SSLv23_client_method();
        if ((ctx = SSL_CTX_new(method)) == NULL) {
            BIO_printf(obj_out, "OpenSSL context not initialize");
        } else {
            SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
            ssl = SSL_new(ctx);
            connceted_fd = ConnectToServer(smtp_server_url);
            if (connceted_fd != 0) {
                BIO_printf(obj_out, "Connected Successfully");
                SSL_set_fd(ssl, connceted_fd);
                if (SSL_connect(ssl) != 1) {
                    BIO_printf(obj_out, "SLL session not created");
                } else {
                    char *header = MailHeader(MAIL_HEADER_FROM, MAIL_HEADER_TO, "Subject: EyeSpy Security System Notification", "text/plain", "US-ASCII");

                    int recvd = 0;
                    char recv_buff[4768];
                    int sdsd;
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;

                    // ssql writes required information for Google smtp 
                    char buff[1000];
                    strcpy(buff, "EHLO ");
                    strcat(buff, domain_name);
                    strcat(buff, "\r\n");
                    SSL_write(ssl, buff, my_strlen(buff));
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;                 

                    char _cmd2[1000];
                    strcpy(_cmd2, "AUTH LOGIN\r\n");
                    int dfdf = SSL_write(ssl, _cmd2, my_strlen(_cmd2));
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;

                    char _cmd3[1000];
                    Base64encode(_cmd3, "cs3307eyespy@gmail.com", my_strlen("cs3307eyespy@gmail.com"));
                    strcat(_cmd3, "\r\n");
                    SSL_write(ssl, _cmd3, my_strlen(_cmd3));
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;

                    char _cmd4[1000];
                    Base64encode(_cmd4, "dlzfgourhrlnqvbu", my_strlen("dlzfgourhrlnqvbu"));
                    strcat(_cmd4, "\r\n");
                    SSL_write(ssl, _cmd4, my_strlen(_cmd4));
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;

                    char _cmd5[1000];
                    strcpy(_cmd5, "MAIL FROM: ");
                    strcat(_cmd5, FROM);
                    strcat(_cmd5, "\r\n");
                    SSL_write(ssl, _cmd5, my_strlen(_cmd5));
                    char skip[1000];
                    sdsd = SSL_read(ssl, skip, sizeof (skip));

                    char _cmd6[1000];
                    strcpy(_cmd6, "RCPT TO: ");
                    strcat(_cmd6, TO); //
                    strcat(_cmd6, "\r\n");
                    SSL_write(ssl, _cmd6, my_strlen(_cmd6));
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;

                    char _cmd7[1000];
                    strcpy(_cmd7, "DATA\r\n");
                    SSL_write(ssl, _cmd7, my_strlen(_cmd7));
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;

                    // writes the header and body using open ssl
                    SSL_write(ssl, header, my_strlen(header));
                    char _cmd8[my_strlen(message)];
                    strcpy(_cmd8, message);
                    SSL_write(ssl, _cmd8, sizeof (_cmd8));
                    
                    char _cmd9[1000];
                    strcpy(_cmd9, "\r\n.\r\n.");
                    SSL_write(ssl, _cmd9, sizeof (_cmd9));
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;

                    char _cmd10[1000];
                    strcpy(_cmd10, "QUIT\r\n");
                    SSL_write(ssl, _cmd10, sizeof (_cmd10));
                    sdsd = SSL_read(ssl, recv_buff + recvd, sizeof (recv_buff) - recvd);
                    recvd += sdsd;
                    printf("0)%s\r\n", &recv_buff);
                    free(header);
                }
            }
        }
    }
}
