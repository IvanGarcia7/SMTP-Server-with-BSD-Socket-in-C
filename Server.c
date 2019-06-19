
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define HELO "HELO PcIvan\r\n"
#define DATA "DATA\r\n"
#define QUIT "QUIT\r\n"



//Defino el socket cliente y el SocketServer que usaremos a lo largo de la clase, así como las estructuras necesarias.

int sock;
struct sockaddr_in server;
struct hostent *hp, *gethostbyname();
char buf[BUFSIZ+1];
int len;
char *host_id="192.168.0.103"; //Modificar dirección donde se aloje el servidor de correo FAKESMTP
char from_id[100];
char to_id[100];
char sub[100];
char wkstr[1000];
char bueno[1000];
int comprobarenvio;
int comprobarlectura;
char errores [1000];

/*
 Funcion creada para, a partir de una condición pasada como parámetro, controlar errores
 y mostrar la cadena pasada
 */

void checkError(int condition, const char * message){
    if(condition){
        
        
        close(sock);
        exit(-1);
    }
}

/*Funcion para enviar un String al Socket*/
void send_socket(char *s){
    
    //Con la funcion write le paso como primer argumento el socket cliente, como segundo la cadena a enviar y como tercero
    //el tamaño de dicha cadena
    
    comprobarenvio = write(sock,s,strlen(s));
    strcpy(errores,"Error en el envio al servidor de ");
    strcat(errores,s);
    if(comprobarenvio <0){
        //Envio al servidor una señal RST;
        
        char envios[100];
        strcpy(envios,"RST\r\n");
        write(sock,envios,strlen(envios));
        
        
        
    }
    
    checkError(comprobarenvio < 0, errores);
    write(1,s,strlen(s));
    
}






/*Funcion para leer un String del Socket*/

void read_socket(){
    
    //La función read se utiliza para leer los datos del Socket que se le pasa como argumento.
    //para ello se le indica como comentaba anteriormente el socket, y la longitud del buffer en bytes..
    
    len = read(sock,buf,BUFSIZ);
    strcpy(errores,"Error en la recepción de datos del servidor ");
    checkError(comprobarenvio < 0, errores);
    
    write(1,buf,len);
}









int main(int argc, char* argv[]){
    
    
    
    
    
    
    /*
     
     La función Socket devuelve un descriptor de Socket, si se devuelve -1
     se ha producido un error, esta función se compone de (dominio,tipo,protocolo);
     
     En el dominio, se le pasa la variable AF_INET para usar los protocolos de Internet.
     
     En el tipo especifico la clase de Socket que quiero usar, en este caso de flujo de datos,
     Es por ello por lo que se le pasa la variable SOCK_STREAM.
     
     En el protocolo se establece el IPPROTO_TCP.
     
     */
    
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    checkError(sock<0,"Error al abrir el Socket\n");
    
    printf( "Socked creado correctamente\n");
    
    
    
    /*Verifico el Host*/
    server.sin_family = AF_INET;
    hp = gethostbyname(host_id);
    checkError(hp==(struct hostent *) 0,"Error Host desconocido\n");
    
    
    /*Me conecto al puerto del FakeSmtp*/
    
    memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
    server.sin_port=htons(1233); //Port de fakesmtp puesto a 1233
    
    /*
     La funcion connect la utilizo para conectarme a un puerto definido en una dirección IP
     dicha función devuelve -1 si se ha producido algún error
     
     Se le pasa como argumentos el socket creado anteriormente, el puntero a la estructura
     que contiene la dirección ip y el puerto y funalmente el tamaño de la estructura.
     */
    
    checkError(connect(sock, (struct sockaddr *) &server, sizeof server)==-1,"Error al conectar con el Socket\n");
    
    
    
    printf( "Conexión realizada\n");
    
    
    
    
    /*  Envío y lectura de mensajes con el servidor a través de las funciones
     send_socket y write;
     
     
     */
    
    
    
    read_socket();
    send_socket(HELO); /* Envio del mensaje HELO */
    read_socket(); /*Leo la respuesta del servidor*/
    
    
    //Leo la dirección del remitente
    
    printf("Introduzca el correo del remitente:\n");
    scanf("%s",from_id);
    
    
    
    //Envío Mail from junto a la dirección del remitente
    send_socket("MAIL FROM: ");
    send_socket(from_id);
    send_socket("\r\n");
    
    
    read_socket(); /* Recibo la respuesta por parte del servidor */
    
    
    
    //Leo la dirección del destinatario
    
    
    printf("Introduzca el correo del destinatario:\n");
    scanf("%s",to_id);
    
    
    
    //Envío Mail from junto a la dirección del destinatario
    send_socket("RCPT TO: "); /*Mail to*/
    send_socket(to_id);
    send_socket("\r\n");
    
    
    read_socket(); // Recibo la respuesta por parte del servidor */
    
    //Envío los datos
    
    
    //Leo el asunto del correo
    
    printf("Introduzca el asunto:\n");
    scanf(" %[^\n]s",sub);
    strcat(sub,"\r\n");
    
    char compara[50];
    strcpy(compara,"FIN");
    
    //Leo el cuerpo del mensaje hasta introducir la palabra FIN
    
    printf("Introduzca el cuerpo del mensaje:\n");
    
    
    
    scanf(" %[^\n]s",wkstr);
    while(strcmp(wkstr,compara)!=0){
        strcat(bueno,wkstr);
        memset(wkstr, 0, sizeof(wkstr));
        strcat(bueno," \n");
        //printf("%s", bueno);
        scanf(" %[^\n]s",wkstr);
    }
    strcat(bueno,"\r\n.\r\n");
    
    
    
    
    
    
    
    send_socket(DATA);
    read_socket();
    send_socket("MAIL FROM: ");
    send_socket(from_id);
    send_socket("\n");
    send_socket("RCPT TO: ");
    send_socket(to_id);
    send_socket("\n");
    send_socket("Subject: ");
    send_socket(sub);
    send_socket("Cuerpo del mensaje: \n");
    send_socket(bueno);
    read_socket();
    
    //Envío el mensaje quit
    
    send_socket(QUIT);
    
    //Leo la respuesta del servidor
    read_socket();
    
    //Cierro el Socket cliente y finalizo la ejecución*/
    close(sock);
    exit(0);
}

