#ifndef GESTORSERVIDORES_H
#define GESTORSERVIDORES_H

#include <string>

class GestorServidores {
public:
    GestorServidores(int puerto1, int puerto2, int puerto3, std::string direccionIP);
    void iniciarServidores(int puerto1, int puerto2, int puerto3);
    int descriptorServidor;

private:
    void iniciarServidor(int puerto);
    void iniciarLB();

    int puerto1;
    int puerto2;
    int puerto3;
    std::string direccionIP;
    std::vector<int> puertos;
};

#endif // GESTORSERVIDORES_H