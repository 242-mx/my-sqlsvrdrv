#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define LONGITUD 512
#define PUERTO 2000

//Declaracion de las variables globales
void cierraSocket();
char mensaje[LONGITUD], *nombreServidor;
int conexion, respuesta;
struct hostent *servidor;
struct sockaddr_in informacionServidor;

main(int argc, char *argv[])
	{
	//Establece una rutina para cerrar los sockets en caso de interrupcion
	signal(SIGINT,cierraSocket);
	//Obten el URL del servidor 
	if(argc!=2)
		printf("Error Fatal: Es necesario especificar correctamnte el URL del Servidor...\n");
	else
		{
		nombreServidor = argv[1];
		servidor = gethostbyname(nombreServidor);
		if(nombreServidor == NULL)
			printf("Error Fatal: El URL del Servidor no es correcto...\n");
		else
			{
			//Obtener la informacion referente al servidor
			informacionServidor.sin_family = AF_INET;
			informacionServidor.sin_addr.s_addr = (((struct in_addr *) (servidor->h_addr))->s_addr);
			informacionServidor.sin_port = htons(PUERTO);
			//Obtener una conexion para Internet
			conexion = socket(AF_INET, SOCK_STREAM, 0);
			if(conexion == -1)
				printf("Error Fatal: No fue posible obtener un conexion con el Servidor...\n");
			else
				{
				//Conectarse al Servidor
				respuesta = connect(conexion, (struct sockaddr *)&informacionServidor, sizeof(informacionServidor));
				if(respuesta == -1)
					printf("Error Fatal: No fue posible establecer una conexion con el Servidor...\n");
				else
					{
					//Esperar una respuesta del Servidor
					respuesta = recv(conexion, mensaje, LONGITUD, 0);
					if(respuesta == -1)
						printf("Error Fatal: No se recibio ninguna respuesta del Servidor...\n");
					else
						{
						//Imprimir la frase recibida
						printf("Mensaje Recibido: %s\n", mensaje);
						}
					}
				}
			}
		}
	cierraSocket();
	}

void cierraSocket()
	{
	close(conexion);
	exit(0);
	}