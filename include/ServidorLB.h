#ifndef SERVIDORLB_H
#define SERVIDORLB_H

#include <vector>

class ServidorLB {
public:
    ServidorLB(int puerto1, int puerto2, int puerto3);
    void iniciar();

private:
    void manejarLB(int descriptorCliente);

    int descriptorServidor;
    std::vector<int> puertosLB;
};

#endif // SERVIDORLB_H