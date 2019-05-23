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

void lee_quote(char q[QUOTESIZE]);

FILE *archivo;
char quote[QUOTESIZE];
int sd, sd_actual;
int addrlen;
struct sockaddr_in sin, pin;
char archivodefrases[256];

void aborta()
{
	close(sd);
	close(sd_actual);
	unlink("./socket");
	exit(0);
}


main(int argc, char *argv[])
{

	//aborta();
	signal(SIGINT,aborta);
	if(argc>1)
		strcpy(archivodefrases,argv[1]);
	else
		strcpy(archivodefrases,"frases_celebres");

	
	/*obtencion de un socket tipo internet*/

	if((sd=socket(AF_INET, SOCK_STREAM, 0))== -1) {
		perror("socket");
		aborta();
	}


	/*asignar direcciones en la estructura de direcciones*/

	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=INADDR_ANY;
	sin.sin_port=htons(PUERTO);

	/*asociando el socket al numero de puerto*/

	if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))== -1) {
		perror("bind");
		aborta();
	}

	/*ponerse a escuchar a traves del socket*/

	if(listen(sd,5)== -1) {
		perror("listen");
		aborta();
	}


	while(1){
		/*esperando que un cliente solicite un servicio*/

		if((sd_actual=accept(sd, (struct sockaddr *)&pin, &addrlen))== -1) {
			perror("accept");
			aborta();
		}
	
		/*tomar un mensaje del cliente */

		if(recv(sd_actual, quote, MSGSIZE, 0)== -1) {
			perror("recv");
			aborta();
		}


		/*extrayendo la frase */

		lee_quote(quote);

		printf("Voy a mandar: %s\n",quote);

		/* enviando la respuesta al servidor*/

		if(send(sd_actual, quote, MSGSIZE, 0)== -1) {
			perror("send");
			aborta();
		}

	}

	/*cerrar los dos sockets*/
		
	close(sd_actual); 

	close(sd);

}/*del main*/

void lee_quote(char q[QUOTESIZE]){

	int lc=0,buffer,i,random;

	if ((archivo = fopen(archivodefrases, "r")) == NULL) {
		fprintf(stderr, "No se puede abrir %s\n", archivodefrases);
	 	aborta();	  
	}
			
	q[0]='\0';
	while(fgets(q,QUOTESIZE,archivo))
		lc++;

	printf("lc:%d\n",lc-1);

	rewind(archivo);
	
	random = rand(srand(time(NULL))) % (lc-1);
	printf("random=%d\n",random);

	for(i=0;i<random;i++)
		fgets(q,QUOTESIZE,archivo);

	fclose(archivo);

}