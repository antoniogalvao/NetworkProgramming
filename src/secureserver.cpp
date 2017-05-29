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

SSL_CTX* SecureServer::initServerCTX(void)
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

void SecureServer::loadCertificates(SSL_CTX *ctx, char* certFile, char* keyFile)
{
   //set the local variable from certFile
   if(SSL_CTX_use_certificate_file(ctx, certFile, SSL_FILETYPE_PEM ) <= 0)
   {
      ERR_print_errors_fp(stderr);
      abort();
   }
   //set the private key from keyFile(may be the same as CertFile)
   if(SSL_CTX_use_PrivateKey_file(ctx, keyFile, SSL_FILETYPE_PEM) <= 0)
   {
      ERR_print_errors_fp(stderr);
      abort();
   }
   //verify private key
   if( !(SSL_CTX_check_private_key(ctx)) )
   {
      fprintf(stderr, "Private key does not match the public certificates\n");
      abort();
   }
}

void SecureServer::showCerts(SSL *ssl)
{
   X509 *cert;
   char *line;

   cert = SSL_get_peer_certificate(ssl); //get certificate(if available)
   if (cert != NULL)
   {
      printf("Server certificates:\n");
      line = X509_NAME_oneline(X509_get_subject_name(cert),0,0 );
      printf("Subject: %s\n", line);
      free(line);
      line = X509_NAME_oneline(X509_get_issuer_name(cert),0 ,0);
      printf("Issuer: %s\n", line);
      free(line);
      X509_free(cert);
   }
   else
   {
      printf("No certificates\n");
   }
}
