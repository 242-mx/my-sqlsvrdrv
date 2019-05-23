#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define LONGITUD 512
#define PUERTO 2000
#define CLIENTES 20

//Declaracion de cabeceras de funcion y variables globales
void cierraSockets();
struct sockaddr_in informacionServidor, informacionCliente;
int respuesta, conexion, conexionActual, longitudDireccion, i, frase;
char mensaje[LONGITUD];
FILE *archivo;

main()
	{
	//Establece una rutina para el cierre de los puertos
	signal(SIGINT, cierraSockets);
	//Establece el socket para atender los servicios
	conexion = socket(AF_INET, SOCK_STREAM, 0);
	if(conexion == -1)
		printf("Error Fatal: No fue posible establecer una conexion...\n");
	else
		{
		//Establece las direcciones a las estructuras de informacion
		informacionServidor.sin_family = AF_INET;
		informacionServidor.sin_addr.s_addr = INADDR_ANY;
		informacionServidor.sin_port = htons(PUERTO);
		//Asocia la conexion a un puerto del servidor
		respuesta = bind(conexion, (struct sockaddr *)&informacionServidor, sizeof(informacionServidor));
		if(respuesta == -1)
			printf("Error Fatal: No fue posible asociar el puerto a la conexion...\n");
		else
			{
			//Escucha mediante la conexion realizada
			if(listen(conexion, CLIENTES) == -1)
				printf("Error Fatal: Ha ocurrido un error mientras esperaba por un servicio...\n");
			else
				{
				while(1)
					{
					//Espera por un servicio de un cliente
					conexionActual = accept(conexion, (struct sockaddr *)&informacionCliente, &longitudDireccion);
					if(conexionActual == -1)
						printf("Error Fatal: Ha ocurrido un error durante el establecimiento de un socket para atender un servicio...\n");
					else
						{
						//Abre el archivo de las frases
						archivo = fopen("frases.txt","r");
						if(archivo == NULL)
							printf("Error Fatal: Ha ocurrido un error al abrir el archivo de frases...\n");
						else
							{
							//Limpia el buffer de mensaje
							for(i = 0; i < LONGITUD; i++)
								mensaje[i] = '\0';
							//Cuenta las frases existentes
							i = 0;
							while(fgets(mensaje, LONGITUD, archivo))
								i++;
							rewind(archivo);
							//Determina un numero aleatorio de frase
							i = i-1;
							frase = (rand(srand(time(NULL)))) % i;
							//Manda el mensaje determinado por frase
							for(i = 0; i < frase; i++)
								fgets(mensaje, LONGITUD, archivo);
							//Cierra el archivo
							fclose(archivo);
							//Enviar la frase al cliente
							respuesta = send(conexionActual, mensaje, LONGITUD, 0);
							if(respuesta == -1)
								printf("Error Fatal: Ha ocurrido un error al enviar la respuesta al cliente...\n");
							}
						}
					}
				}
			}
		}
	cierraSockets();
	}

void cierraSockets()
	{
	close(conexion);
	close(conexionActual);
	exit(0);
	}