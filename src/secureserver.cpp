#include "include/secureserver.h"

#include <unistd.h>
#include <sys/types.h>

int SecureServer::isRoot()
{
   if (getuid() != 0)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

SSL_CTX* SecureServer::InitServerCTX(void)
{
   const SSL_METHOD *method;
   SSL_CTX *ctx;
   OpenSSL_add_all_algorithms();
   SSL_load_error_strings();
   method = TLSv1_2_server_method();
   ctx = SSL_CTX_new(method);
   if (ctx == NULL)
   {
      ERR_print_errors_fp(stderr);
      abort();
   }
   return ctx;
}
