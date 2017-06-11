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
   string message;
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
			while(true) {
      		std::cin >> message;
      		stream->send(message.c_str(), message.size());
      		printf("sent - %s\n", message.c_str());
      		length = stream->receive(line, sizeof(line));
      		line[length] = '\0';
      		printf("received - %s\n", line);
			}
		}
      delete stream;
   }
   exit(0);
}
