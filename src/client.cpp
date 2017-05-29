#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tcpconnector.h"

using namespace std;

int main(int argc, char** argv)
{
   if(argc != 3)
   {
      printf("Usage: %s <ip> <port>\n", argv[0]);
  	   exit(1);
   }

   int length;
   string message;
   char line[256];
   TCPConnector* connector = new TCPConnector;
   TCPStream* stream = connector->connect(argv[1], atoi(argv[2]));
   if(stream)
   {
      message = "Hello World!!";
      stream->send(message.c_str(), message.size());
      printf("sent - %s\n", message.c_str());
      length = stream->receive(line, sizeof(line));
      line[length] = '\0';
      printf("received - %s\n", line);
      delete stream;
   }
   exit(0);
}
