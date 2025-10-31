#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;                    // Descriptor de socket
    struct sockaddr_in serv_addr;     // Dirección del servidor
    char *hello = "Hola servidor!";  // Mensaje a enviar
    char buffer[BUFFER_SIZE] = {0};  // Buffer para recibir datos

    // Crear socket TCP
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nError al crear socket\n");
        return -1;
    }

    // Configurar dirección del servidor
    serv_addr.sin_family = AF_INET;           // IPv4
    serv_addr.sin_port = htons(PORT);         // Puerto en network byte order

    // Convertir IP de string a binario
    if (inet_pton(AF_INET, "192.168.1.100", &serv_addr.sin_addr) <= 0) {
        printf("\nDirección inválida/No soportada\n");
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConexión fallida\n");
        return -1;
    }

    // Enviar mensaje al servidor
    send(sock, hello, strlen(hello), 0);
    printf("Mensaje enviado\n");

    // Recibir respuesta del servidor
    int valread = read(sock, buffer, BUFFER_SIZE);
    printf("Respuesta del servidor: %s\n", buffer);

    // Cerrar socket
    close(sock);

    return 0;
}
