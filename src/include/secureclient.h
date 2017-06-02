#ifndef SECURE_CLIENT_H
#define SECURE_CLIENT_H


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


class SecureClient
{
   public:
      static SSL_CTX *initCTX(void);
      static void showCerts(SSL *ssl);
};


#endif
