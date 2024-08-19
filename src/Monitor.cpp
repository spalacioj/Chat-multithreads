#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include "monitor.h"


Monitor::Monitor(std::string direccionIP, int puerto1, int puerto2, int puerto3)
    : direccionIP(direccionIP), puerto1(puerto1), puerto2(puerto2), puerto3(puerto3) {}

bool Monitor::conectarAlServidor(int puerto) {
    descriptorCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (descriptorCliente == -1) {
        std::cerr << "Error al crear el socket del cliente.\n";
        return false;
    }

    sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(puerto);
    inet_pton(AF_INET, direccionIP.c_str(), &direccionServidor.sin_addr);

    if (connect(descriptorCliente, (sockaddr*)&direccionServidor, sizeof(direccionServidor)) == -1) {
        std::cerr << "Error al conectar al servidor balanceador.\n";
        return false;
    }

    return true;
}

std::string Monitor::recibirDatos() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRecibidos = recv(descriptorCliente, buffer, 1024, 0);
    if (bytesRecibidos <= 0) {
        std::cerr << "Error al recibir datos del servidor.\n";
        return "Error al recibir datos";
    }
    std::string usuariosActivos = std::string(buffer, bytesRecibidos);
    return usuariosActivos;
}

void Monitor::desconectar() {
    if (descriptorCliente != -1) {
        close(descriptorCliente);
        descriptorCliente = -1;
    }
}

void Monitor::consultarEstadoSalas() {
    char buffer[1024];
    const int puertos[] = {puerto1, puerto2, puerto3};
    
    for (int i = 0; i < 3; i++){
        if(conectarAlServidor(puertos[i])){
            std::string comando = "monitor";
            ssize_t bytesRecibidos = recv(descriptorCliente, buffer, 1024, 0);
            send(descriptorCliente, comando.c_str(), comando.size(), 0);
            std::string usuariosEnSala = recibirDatos();
            std::cout << "Sala " << (i + 1) << " tiene " << usuariosEnSala << " usuarios\n";
        } else {
            std::cout << "La sala " << (i + 1) << "esta caida";
        }
        desconectar();
    }
    
    /*if (!conectarAlServidor()) {
        return;
    }

    for (int i = 1; i <= 3; ++i) {
        std::string comando = "monitor";
        send(descriptorCliente, comando.c_str(), comando.size(), 0);
        int usuariosEnSala = recibirDatos();
        if (usuariosEnSala >= 0) {
            std::cout << "Sala " << i << " tiene " << usuariosEnSala << " usuarios.\n";
        } else {
            std::cout << "No se pudo obtener la información de la Sala " << i << ".\n";
        }
    }*/

    desconectar();
}

/*int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <direccionIP> <puerto>\n";
        return 1;
    }

    std::string direccionIP = argv[1];
    int puertoLB = std::stoi(argv[2]);

    Monitor monitor(direccionIP, puertoLB);
    monitor.consultarEstadoSalas();
    return 0;
}*/