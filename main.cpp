#include <iostream>
#include "ClienteChat.h"
#include "ServidorChat.h"
#include "GestorServidores.h"
#include "monitor.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <modo> <direccionIP> <puerto>\n";
        std::cerr << "Modos disponibles: servidor, cliente\n";
        return 1;
    }

    std::string modo = argv[1];

    if (modo == "servidor") {
        if (argc < 3) {
            std::cerr << "Uso: " << argv[0] << " servidor <puerto>\n";
            return 1;
        }
        int puerto = std::stoi(argv[2]);
        ServidorChat servidor(puerto);  // Inicializa el servidor con el puerto proporcionado
        servidor.iniciar();  // Inicia el servidor
    } else if (modo == "cliente") {
        if (argc < 4) {
            std::cerr << "Uso: " << argv[0] << " cliente <direccionIP> <puerto>\n";
            return 1;
        }
        std::string direccionIP = argv[2];
        int puerto = std::stoi(argv[3]);
        ClienteChat cliente(direccionIP, puerto);  // Inicializa el cliente con la dirección IP y puerto proporcionados
        cliente.conectarAlServidor();  // Conecta al servidor

        std::string mensaje;
        while (std::getline(std::cin, mensaje)) {
            cliente.manejarComando(mensaje);  // Envía el mensaje al servidor
        }

        cliente.desconectar();  // Desconecta del servidor
    } else if (modo == "servidores"){
        if (argc < 5){
            std::cerr << "Uso: " << argv[0] << " servidores <ip> <puerto1> <puerto2> <puerto3>\n";
        }
        std::string direccionIP = argv[2];
        int puerto1 = std::stoi(argv[3]);
        int puerto2 = std::stoi(argv[4]);
        int puerto3 = std::stoi(argv[5]);
        GestorServidores gestor(puerto1, puerto2, puerto3, direccionIP);
    } else if (modo == "monitor") {
        if (argc < 5){
            std::cerr << "Uso: " << argv[0] << " servidores <ip> <puerto1> <puerto2> <puerto3>\n";
        }
        try
        {
        std::string direccionIP = argv[2];
        int puerto1 = std::stoi(argv[3]);
        int puerto2 = std::stoi(argv[4]);
        int puerto3 = std::stoi(argv[5]);
        Monitor monitor(direccionIP, puerto1, puerto2, puerto3);
        monitor.consultarEstadoSalas();
        }
        catch (const std:: invalid_argument& e)
        {
            std::cerr << "Error: argumento inválido. Detalles: " << e.what() << '\n';
        }
    } else {
        std::cerr << "Modo desconocido: " << modo << "\n";
        return 1;
    }

    return 0;
}
