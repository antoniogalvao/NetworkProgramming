#ifndef SECURITYSERVER_H
#define SECURITYSERVER_H


#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"

class SecureServer
{
   public:
      static int isRoot();
      static SSL_CTX *initServerCTX(void);
      static void loadCertificates(SSL_CTX *ctx, char* certFile, char* keyFile);
      static void showCerts(SSL *ssl);
};


#endif
