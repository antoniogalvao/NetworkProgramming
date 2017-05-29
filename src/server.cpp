#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "include/tcpacceptor.h"
#include "include/secureserver.h"

int main(int argc, char** argv)
{
   SSL_CTX *ctx;
   char certFile[] = "certs/mycert.pem";
   char keyFile[] = "certs/mycert.pem";

   if(!SecureServer::isRoot())
   {
      printf("This program must be run as a root/sudo user!\n");
   	exit(1);
	}

   if(argc != 3)
   {
      printf("Usage: %s <ip> <port>\n", argv[0]);
      exit(1);
   }


   SSL_library_init();
   ctx = SecureServer::initServerCTX();
   SecureServer::loadCertificates(ctx, certFile, keyFile);


   TCPStream* stream = NULL;
   TCPAcceptor* acceptor = NULL;
   acceptor = new TCPAcceptor(argv[1], atoi(argv[2]));
   cout << "Server online" << endl;

	if(acceptor->start() == 0)
   {
      while(1)
      {
         stream = acceptor->accept();
         std::cout << "Connection: " << stream->getPeerIP() << ":" << stream->getPeerPort() << std::endl;
         //SSL *ssl;
         //ssl = SSL_new(ctx);
         //SSL_set_fd(ssl, )

         if(stream != NULL)
         {
            ssize_t length;
            char line[256];
            while ((length = stream->receive(line, sizeof(line))) > 0)
            {
               line[length] = 0;
               printf("received - %s\n", line);
					for(unsigned i = 0; i < sizeof(line); i++)
					{
						line[i] = toupper(line[i]);
					}
               stream->send(line, length);
            }
            delete stream;
         }
      }
   }

   perror("Could not start the server\n");
   SSL_CTX_free(ctx);
   exit(-1);
}
