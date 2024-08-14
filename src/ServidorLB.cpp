#include "ServidorLB.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <cstring>

ServidorLB::ServidorLB(int puerto1, int puerto2, int puerto3)
    : puertosLB{puerto1, puerto2, puerto3} {}

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

void ServidorLB::manejarLB(int descriptorCliente){
    char buffer[1024];

    std::string MensajeBienvenida = "Bienvenido al chat escoga una de estas salas:\n1. Sala 1\n2. Sala 2\n3. Sala 3.";
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

        // Procesar comandos del protocolo
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