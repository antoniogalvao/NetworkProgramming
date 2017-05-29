# NetworkProgramming


Generating the certificate

$ openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout mycert.pem -out mycert.pem


Building the project

$ make -f Makefile.client

$ make -f Makefile.server
