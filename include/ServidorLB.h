#ifndef SERVIDORLB_H
#define SERVIDORLB_H

#include <vector>
#include <string>

class ServidorLB {
public:
    ServidorLB(int puerto1, int puerto2, int puerto3, std::string direccionIP);
    void iniciar();

private:
    void manejarLB(int descriptorCliente);
    bool salaActiva(int puerto);

    std::string direccionIP;
    int descriptorServidor;
    std::vector<int> puertosLB;
};

#endif // SERVIDORLB_H