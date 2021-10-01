#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "MQTTClient.h"
#include <mysql.h>
#include <stdio.h>
#include <wiringPi.h>

#define ADDRESS     "192.168.43.131"
#define CLIENTID    "subscriber_demo"
#define SUB_TOPIC   "Paul_MQTT"
#define PUB_TOPIC   "Paul_MQTT_control"
//#define QOS         1

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

char *server = "localhost";
char *user = "Paul";
char *password = "Phuoc12345"; /* set me first */
char *database = "esp32_mqtt";
char cmd[200]; 
char data[200];
char sql[200];
int state_mess =0;
char* payload_ms;

int timer=0;
int status_connect =0;

void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    printf("Message '%s' with delivery token %d delivered\n", payload, token);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    payload_ms = message->payload;
    printf("Received message: %s\n", payload_ms);
	state_mess =1;
	
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    //conn_opts.username = "your_username>>";
    //conn_opts.password = "password";

    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
   
    //listen for operation
    MQTTClient_subscribe(client, SUB_TOPIC, 0);
		// ket noi database
    conn = mysql_init(NULL);
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }  
	else
	mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    while(1) {	
	//mỗi 1 giây sẽ kiểm tra trạng thái
	if(millis()> timer + 1000)
	{
		timer = millis();
		sprintf(data,"select * from control");
		mysql_query(conn,data);
		res = mysql_store_result(conn); 
		row = mysql_fetch_row(res); //row[0]-> status; row[2]->isUpdated row[3]: status_web
		
		if(atoi(row[3]) ==1)
		{
			if( atoi(row[2])==1)
			{		
				char msg [20];
				//update dữ liệu từ database
				sprintf(msg,"%s-%s",row[0],row[1]);
				publish(client, PUB_TOPIC, msg);
				//tat update
				sprintf(cmd,"update control set isUpdated=0");
				// send SQL query 
				mysql_query(conn, cmd);		
			}
			//turn on status_connect
			status_connect =1;	
			//feedback
			sprintf(cmd,"update control set status_feedback=1");
			// send SQL query 
			mysql_query(conn, cmd);
		}
		else if(atoi(row[3]) ==0)
		{
			//turn off status_connect
			status_connect =0;	
			//feedback
			sprintf(cmd,"update control set status_feedback=0");
			// send SQL query 
			mysql_query(conn, cmd);
		}
	}
	if(status_connect ==1)
	{
		if(state_mess ==1)
		{
			sprintf(sql,"insert into khoang_cach(kcach) values (%s)",payload_ms);
			mysql_query(conn,sql);
			state_mess=0;
		}		
	}	
	}
	//tắt truy cập
	mysql_close(conn);
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;
}