#include <arpa/inet.h>
#include "include/tcpstream.h"

TCPStream::TCPStream(int sd, struct sockaddr_in* address): m_peerSocketDescriptor(sd)
{
   char ip[50];
   inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
   m_peerIP = ip;
   m_peerPort = ntohs(address->sin_port);
}

TCPStream::~TCPStream()
{
   close(m_peerSocketDescriptor);
}

ssize_t TCPStream::send(const char* buffer, size_t lenght)
{
   return write(m_peerSocketDescriptor, buffer, lenght);
}

ssize_t TCPStream::receive(char* buffer, size_t length)
{
   return read(m_peerSocketDescriptor, buffer, length);
}

int TCPStream::getPeerSocketDescriptor()
{
   return m_peerSocketDescriptor;
}

string TCPStream::getPeerIP()
{
   return m_peerIP;
}

int TCPStream::getPeerPort()
{
   return m_peerPort;
}
