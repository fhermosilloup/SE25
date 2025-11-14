// Compilar en windows: g++ client_windows.cpp -o client_windows.exe -lws2_32
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024


int main(void) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr;

    // Inicializar Winsock (Windows Sockets API)
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "Error iniciando Winsock. Código: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Crear socket TCP (SOCK_STREAM) sobre IPv4 (AF_INET)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cout << "Error al crear socket. Código: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Configurar servidor (dirección y puerto)
    server_addr.sin_family = AF_INET;	// IPv4
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // Dirección IP del servidor
    server_addr.sin_port = htons(SERVER_PORT); // Puerto del servidor

    // Conectarse al servidor
    std::cout << "Conectando al servidor " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cout << "No se pudo conectar. Código: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // En este punto, se ha establecido una conexión con el servidor
    std::cout << "Conectado al servidor" << std::endl;

    // Bucle principal
    std::string Data2Tx;
	char Data2Rx[BUFFER_SIZE];
    while (1) {
        // Transmitir mensaje al servidor
		std::cout << "Mensaje a Transmitir: ";
		std::getline(std::cin, Data2Tx);
        if (send(sock, Data2Tx.c_str(), Data2Tx.length(), 0) == SOCKET_ERROR) {
            std::cout << "Error enviando setpoint. Código: " << WSAGetLastError() << std::endl;
            break;
        }

        // Recibir mensaje con un tamaño maximo de BUFFER_SIZE bytes
        if (recv(sock, Data2Rx, BUFFER_SIZE, 0) == SOCKET_ERROR) {
            std::cout << "Error recibiendo mensaje. Código: " << WSAGetLastError() << std::endl;
            break;
        }
        std::cout << "Mensaje Recibido: " << Data2Rx << std::endl;
    }

    // En este punto, se ha desconectado del servidor
    std::cout << "Desconectado del servidor" << std::endl;

    // Cerrar socket y limpiar recursos de Winsock
    closesocket(sock);
    WSACleanup();

    
    return 0;
}
