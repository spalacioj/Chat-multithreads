#include "ServidorLB.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <cstring>

// Constructor que crea el vector con los puertos de la sala
ServidorLB::ServidorLB(int puerto1, int puerto2, int puerto3, std::string ip)
    : direccionIP(ip), puertosLB{puerto1, puerto2, puerto3} {}


bool ServidorLB::salaActiva(int puerto) {
    int descriptorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (descriptorSocket == -1) {
        return false;
    }

    sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(puerto);
    inet_pton(AF_INET, direccionIP.c_str(), &direccionServidor.sin_addr);

    bool activo = (connect(descriptorSocket, (sockaddr*)&direccionServidor, sizeof(direccionServidor)) != -1);
    close(descriptorSocket);
    return activo;
}

// Metodo para iniciar el servidor de load balancer
void ServidorLB::iniciar() {
    // Crear el socket del servidor
    descriptorServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (descriptorServidor == -1) {
        std::cerr << "Error al crear el socket del servidor.\n";
        return;
    }

    sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(12345);
    direccionServidor.sin_addr.s_addr = INADDR_ANY;

    // Asociar el socket a la dirección y puerto
    if (bind(descriptorServidor, (sockaddr*)&direccionServidor, sizeof(direccionServidor)) == -1) {
        std::cerr << "Error al hacer bind del socket del servidor.\n";
        return;
    }

    // Poner el servidor en modo escucha
    if (listen(descriptorServidor, 10) == -1) {
        std::cerr << "Error al poner el servidor en modo escucha.\n";
        return;
    }

    std::cout << "Servidor iniciado en el puerto " << 12345 << ". Esperando conexiones...\n";

    // Aceptar conexiones entrantes
    while (true) {
        sockaddr_in direccionCliente;
        socklen_t tamanoDireccionCliente = sizeof(direccionCliente);
        int descriptorCliente = accept(descriptorServidor, (sockaddr*)&direccionCliente, &tamanoDireccionCliente);

        if (descriptorCliente == -1) {
            std::cerr << "Error al aceptar la conexión de un cliente.\n";
            continue;
        }

        // Crear un hilo para manejar el cliente
        std::thread hiloCliente(&ServidorLB::manejarLB, this, descriptorCliente);
        hiloCliente.detach();
    }
}

// En este metodo se agrega el manejo de comandos para que el cliente pueda elegir la sala a la cual conectarse
void ServidorLB::manejarLB(int descriptorCliente){
    char buffer[1024];
    
    bool sala1Activa = salaActiva(puertosLB[0]);
    bool sala2Activa = salaActiva(puertosLB[1]);
    bool sala3Activa = salaActiva(puertosLB[2]);

    std::string MensajeBienvenida = "Bienvenido al chat. Elija una de estas salas:\n";
    if (sala1Activa) {
        MensajeBienvenida += "1. Sala 1\n";
    } else {
        MensajeBienvenida += "1. Sala 1 (Inactiva)\n";
    }
    if (sala2Activa) {
        MensajeBienvenida += "2. Sala 2\n";
    } else {
        MensajeBienvenida += "2. Sala 2 (Inactiva)\n";
    }
    if (sala3Activa) {
        MensajeBienvenida += "3. Sala 3\n";
    } else {
        MensajeBienvenida += "3. Sala 3 (Inactiva)\n";
    }
    std::string ComandoInvalido = "Por favor escriba 1, 2 o 3 para la sala que quiere entrar el comando que envio no es valido\n";
    send(descriptorCliente, MensajeBienvenida.c_str(), MensajeBienvenida.size(), 0);
    ssize_t bytesRecibidos;
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        bytesRecibidos = recv(descriptorCliente, buffer, 1024, 0);

        if (bytesRecibidos <= 0) {
            close(descriptorCliente);
            break;
        }

        std::string mensaje = std::string(buffer, bytesRecibidos);

        // Procesar comandos de la sala a elegir
        if (mensaje == "1") {
            std::string ComandoSala = "@nueva " + std::to_string(puertosLB[0]);
            send(descriptorCliente, ComandoSala.c_str(), ComandoSala.size(), 0);
        } else if (mensaje == "2") {
            std::string ComandoSala = "@nueva " + std::to_string(puertosLB[1]);
            send(descriptorCliente, ComandoSala.c_str(), ComandoSala.size(), 0);
        } else if (mensaje == "3") {
            std::string ComandoSala = "@nueva " + std::to_string(puertosLB[2]);
            send(descriptorCliente, ComandoSala.c_str(), ComandoSala.size(), 0);
        } else {
            send(descriptorCliente, ComandoInvalido.c_str(), ComandoInvalido.size(), 0);
        }
    }
}