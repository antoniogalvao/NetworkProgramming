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

   if(!SecureServer::isRoot())
   {
      printf("This program must be run as a root/sudo user!\n");
   }

   if(argc != 3)
   {
      printf("Usage: %s <ip> <port>\n", argv[0]);
      exit(1);
   }

   TCPStream* stream = NULL;
   TCPAcceptor* acceptor = NULL;
   acceptor = new TCPAcceptor(argv[1], atoi(argv[2]));
   if(acceptor->start() == 0)
   {
      while(1)
      {
         stream = acceptor->accept();
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
   exit(-1);
}
