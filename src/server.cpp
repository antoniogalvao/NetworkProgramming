#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "include/thread.h"
#include "include/workqueue.h"
#include "include/tcpacceptor.h"
#include "include/secureserver.h"

#define FAIL -1

class WorkItem
{
   TCPStream* m_stream;
public:
   WorkItem(TCPStream* stream): m_stream(stream) {}
   ~WorkItem() {delete m_stream;}

   TCPStream* getStream() {return m_stream;}
};

class ConnectionHandler : public Thread
{
   WorkQueue<WorkItem*>& m_queue;
public:
   ConnectionHandler(WorkQueue<WorkItem*>& queue) : m_queue(queue) {}

   void* run()
   {
      for(int i = 0;; i++)
      {
         printf("thread %lu, loop %d - waiting for item...\n",
            (long unsigned)self(), i);
         WorkItem* item = m_queue.remove();
         printf("thread %lu, loop %d - got one item\n",
            (long unsigned)self(), i);


         TCPStream* stream = item->getStream();

         ssize_t length;
         char line[256];
         while ((length = stream->receiveMessage(line, sizeof(line))) > 0)
         {
            line[length] = 0;
            printf("received - %s\n", line);
            for(unsigned i = 0; i < sizeof(line); i++)
            {
               line[i] = toupper(line[i]);
            }
            stream->sendMessage(line, length);
         }
         delete stream;
      }
      return NULL;
   }
};



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

   if(argc != 4)
   {
      printf("Usage: %s <ip> <port> <workers>\n", argv[0]);
      exit(1);
   }

   //create the queue and consumer(worker) threads
   int workers = atoi(argv[3]);
   WorkQueue<WorkItem*> queue;
   for (int i = 0; i < workers; i++)
   {
      ConnectionHandler* handler = new ConnectionHandler(queue);
      if(!handler) {
         printf("Could not create ConnectionHandler %d\n", i);
         exit(1);
      }
      handler->start();
   }
   //start ssl
   SSL_library_init();
   ctx = SecureServer::initServerCTX();
   SecureServer::loadCertificates(ctx, certFile, keyFile);

   //Create an acceptor then start listening for connections
   WorkItem* item;
   //TCPStream* stream = NULL;
   TCPAcceptor* acceptor = NULL;
   acceptor = new TCPAcceptor(argv[1], atoi(argv[2]));
   if(!acceptor || acceptor->start() > 0){
      printf("Could not create an connection acceptor\n");
      exit(1);
   }

   cout << "Server online" << endl;


   while(1)
   {
      SSL* ssl;
      TCPStream* connection = acceptor->accept();
      std::cout << "Connection: " << connection->getPeerIP() << ":" << connection->getPeerPort() << std::endl;
      ssl = SSL_new(ctx);
      SSL_set_fd(ssl, connection->getPeerSocketDescriptor());

      if ( SSL_accept(ssl) == FAIL )     // do SSL-protocol accept
         ERR_print_errors_fp(stderr);
      else
         SecureServer::showCerts(ssl);        // get any certificate


      if(!connection){
         printf("Could not accept a connection\n");
         continue;
      }
      item = new WorkItem(connection);
      if(!item)
      {
         printf("Could not create work item \n");
         continue;
      }
      queue.add(item);
   }

/*
	if(acceptor->start() == 0)
   {
      while(1)
      {
         SSL* ssl;
         stream = acceptor->accept();
         std::cout << "Connection: " << stream->getPeerIP() << ":" << stream->getPeerPort() << std::endl;
         ssl = SSL_new(ctx);
         SSL_set_fd(ssl, stream->getPeerSocketDescriptor());

         if ( SSL_accept(ssl) == FAIL )     // do SSL-protocol accept
            ERR_print_errors_fp(stderr);
         else
            SecureServer::showCerts(ssl);        // get any certificate

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
*/
   perror("Could not start the server\n");
   SSL_CTX_free(ctx);
   exit(-1);
}
