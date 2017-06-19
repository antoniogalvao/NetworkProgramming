#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "include/tcpconnector.h"
#include "include/secureclient.h"

#define FAIL -1


using namespace std;

int main(int argc, char** argv)
{
   if(argc != 3)
   {
      printf("Usage: %s <ip> <port>\n", argv[0]);
  	   exit(1);
   }

	SSL_CTX *ctx;
	SSL *ssl;
   int length;
   char message[256];
   char line[256];

	SSL_library_init();
	ctx = SecureClient::initCTX();

	TCPConnector* connector = new TCPConnector;
   TCPStream* stream = connector->connect(argv[1], atoi(argv[2]));
   if(stream)
   {
		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, stream->getPeerSocketDescriptor());
		if( SSL_connect(ssl) == FAIL)
			ERR_print_errors_fp(stderr);
		else
		{
			printf("Connected with %s encryption", SSL_get_cipher(ssl));
			SecureClient::showCerts(ssl);
			/*while(true) {
      		std::cin >> message;
				std::cout << message << endl;
				//std::cout << message.c_str() << endl;
				stream->sendMessage(message, 256);
      		printf("sent - %s\n", message);
      		length = stream->receiveMessage(line, sizeof(line));
      		line[length] = '\0';
      		printf("received - %s\n", line);
			}*/
			while(1)
			{
    	    	cout << ">";
        		string data;
       			getline(cin, data);
       			memset(&message, 0, sizeof(message));//clear the buffer
       			strcpy(message, data.c_str());
        /*if(data == "exit")
        {
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }*/
        //bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
            	stream->sendMessage(message, 256);
            	printf("sent - %s\n", message);
				 cout << "Awaiting server response..." << endl;
        		memset(&message, 0, sizeof(message));//clear the buffer
        		//bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
        		length = stream->receiveMessage(line, sizeof(line));
        		line[length] = '\0';
      			printf("received - %s\n", line);
      		}
        /*if(!strcmp(msg, "exit"))
        {
            cout << "Server has quit the session" << endl;
            break;
        }
        cout << "Server: " << msg << endl;
    }*/
		}
      delete stream;
   }
   exit(0);
}
