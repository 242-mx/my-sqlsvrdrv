#include <stdio.h>
#include <winsock.h>

#define QUOTESIZE 512

char quote[QUOTESIZE];

LPHOSTENT he;
SOCKET sd;
SOCKADDR_IN serverin;
int puerto=2000;

void main(int argc, char *argv[])
{
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	char *host;

	/* verificando los parametros entrada */

	if(argc==3){
		host=argv[1];
		puerto=atoi(argv[2]);
	}
	else if(argc>3 || argc<2) {
		fprintf(stderr, "Error, uso: % <host> <puerto>\n", argv[0]);
		exit(1);
	}

	WSAStartup(wVersionRequested, &wsaData);

	/*encontrando todo lo referente acerca de la maquina host*/

	he = gethostbyname(host);
	if(he == NULL) {
		perror("gethostbyname");
		return;
	}
	gethostbyname(argv[1]);

	/*Crear un socket TCP/IP stream*/

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sd == INVALID_SOCKET)
	{
		fprintf(stderr,"\n%s: %d\n", "socket()", WSAGetLastError());
		return;
	}

	/*llenar la estructura de direcciones con la informacion del host*/

	serverin.sin_family = AF_INET;
	serverin.sin_addr = *((LPIN_ADDR)*he->h_addr_list);
	serverin.sin_port = htons(puerto);

	/*conectandose al servidor*/
	
	if(connect(sd,(LPSOCKADDR)&serverin,sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		fprintf(stderr,"\n%s: %d\n", "connect()", WSAGetLastError());
		closesocket(sd);
		return;
	}

	if(recv(sd, quote, QUOTESIZE, 0) == SOCKET_ERROR)
	{
		fprintf(stderr,"\n%s: %d\n", "recv()", WSAGetLastError());
		closesocket(sd);
		return;
	}

	printf("%s", quote);

	closesocket(sd);
	
	/* Cerrar winsock */
	WSACleanup();
}

