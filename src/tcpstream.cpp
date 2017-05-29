#include <arpa/inet.h>
#include "include/tcpstream.h"

TCPStream::TCPStream(int sd, struct sockaddr_in* address): m_socketDescriptor(sd)
{
   char ip[50];
   inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
   m_peerIP = ip;
   m_peerPort = ntohs(address->sin_port);
}

TCPStream::~TCPStream()
{
   close(m_socketDescriptor);
}

ssize_t TCPStream::send(const char* buffer, size_t lenght)
{
   return write(m_socketDescriptor, buffer, lenght);
}

ssize_t TCPStream::receive(char* buffer, size_t length)
{
   return read(m_socketDescriptor, buffer, length);
}

string TCPStream::getPeerIP()
{
   return m_peerIP;
}

int TCPStream::getPeerPort()
{
   return m_peerPort;
}
