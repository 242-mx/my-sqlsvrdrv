#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define QUOTESIZE 500
#define PUERTO 8560
#define MSGSIZE 500

char quote[QUOTESIZE];
int sd;
struct hostent *hp;
struct sockaddr_in sin, pin;
char *host;

void aborta()
{
	close(sd);
	exit(0);
}

main(int argc, char *argv[])
{

	signal(SIGINT,aborta);

	/*verificando numero parametros entrada*/

	if(argc!=2) {
		fprintf(stderr, "Error, uso: % <host> \n", argv[0]);
		aborta();
	}

	/*encontrando todo lo referente acerca de la maquina host*/

	host=argv[1];
	if((hp = gethostbyname(host)) == 0) {
		perror("gethostbyname");
		aborta();
	}

	/*llenar la estructura de direcciones con la informacion del host*/

	pin.sin_family = AF_INET;
	pin.sin_addr.s_addr = (((struct in_addr *) (hp->h_addr))->s_addr);
	pin.sin_port = htons(PUERTO);

	/*obtencion de un socket tipo internet*/

	if((sd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		aborta();
	}

	/*conectandose al PUERTO  en el HOST */
	if(connect(sd, (struct sockaddr *)&pin, sizeof(pin)) == -1) {
		perror("el connect");
		exit();
	}

	/*enviar un mensaje al PUERTO del servidor en la maquina HOST*/

	if(send(sd, argv[1], MSGSIZE, 0) == -1) {
		perror("send");
		aborta();
	}

	/*esperar por la respuesta*/

	if(recv(sd, quote, MSGSIZE, 0) == -1) {
		perror("recv");
		aborta();
	}

	/*imprimir los resultados y cerrando la conexion */

	printf("%s\n", quote);
	close(sd);

} /*del main*/

