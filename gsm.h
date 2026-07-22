// Function Prototypes
void gsm_Init(void);  // Initialize GSM module
void send_sms(char *num, char *msg);  // Send SMS to a number
void send_sms1(char *num, char *msg, char *sp);
int GSM_Wait(void);  // Check if a new message has arrived
void read_sms(void);  // Read the received message
void delete_msg(void); 
void msg_ext_dis_GSM(void);
/*void GSM_Response(void); // Delete message at the specified position
void GSM_Response_Display(void);*/

//#endif // GSM_H
