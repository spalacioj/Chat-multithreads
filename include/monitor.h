#ifndef MONITOR_H
#define MONITOR_H

#include <string>

class Monitor {
public:
    Monitor(std::string direccionIP, int puerto1, int puerto2, int puerto3);
    void consultarEstadoSalas();
    
private:
    std::string direccionIP;
    int puerto1;
    int puerto2;
    int puerto3;
    int descriptorCliente;

    bool conectarAlServidor(int puerto);
    std::string recibirDatos();
    void desconectar();
};

#endif // MONITOR_H