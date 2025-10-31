#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // memset, strlen
#include <unistd.h>     // close, read, write
#include <arpa/inet.h>  // sockaddr_in, inet_addr, htons

#define PORT 5000       // Puerto TCP donde escuchará el servidor
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;             // Descriptores de socket
    struct sockaddr_in address;             // Estructura que define IP y puerto
    int addrlen = sizeof(address);          // Tamaño de la estructura
    char buffer[BUFFER_SIZE] = {0};         // Buffer para recibir datos

    // Crear socket TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if (server_fd == 0) {
        perror("socket failed");  // Imprime error si no se puede crear
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
    address.sin_family = AF_INET;          // Familia IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Escucha en todas las interfaces
    address.sin_port = htons(PORT);        // Puerto en formato network byte order

    // Vincular socket a IP y puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes (máx 3 en cola)
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Servidor TCP escuchando en el puerto %d...\n", PORT);

    // Aceptar conexión entrante
    new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Conexión aceptada\n");

    // Recibir datos del cliente
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    printf("Recibido: %s\n", buffer);

    // Enviar respuesta al cliente
    char *message = "Hola desde Raspberry Pi!";
    send(new_socket, message, strlen(message), 0);

    // 8. Cerrar sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
