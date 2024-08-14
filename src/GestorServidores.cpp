#include "ServidorChat.h"
#include "GestorServidores.h"
#include "ServidorLB.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <cstring>

GestorServidores::GestorServidores(int puerto1, int puerto2, int puerto3, std::string ip)
    : puerto1(puerto1), puerto2(puerto2), puerto3(puerto3), direccionIP(ip), puertos{puerto1, puerto2, puerto3} {
        iniciarServidores(puerto1, puerto2, puerto3);
    }

//Funcion para iniciar un solo servidor
void GestorServidores::iniciarServidor(int puerto){
    // iniciando primer servidor
    ServidorChat servidor(puerto);
    servidor.iniciar();
}

void GestorServidores::iniciarLB(){
    ServidorLB servidorLB(puerto1, puerto2, puerto3);
    servidorLB.iniciar();
}

//Funcion para iniciar 3 servidores y ponerlos en hilos independientes
void GestorServidores::iniciarServidores(int puerto1, int puerto2, int puerto3){
    //Creando los hilos
    
    std::thread LB(&GestorServidores::iniciarLB, this);
    std::thread servidor1(&GestorServidores::iniciarServidor, this, puerto1);
    std::thread servidor2(&GestorServidores::iniciarServidor, this, puerto2);
    std::thread servidor3(&GestorServidores::iniciarServidor, this, puerto3);

    //liberar los hilos del hilo principal
    LB.detach();
    servidor1.detach();
    servidor2.detach();
    servidor3.detach();

    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(24));  // Duerme el hilo principal durante 24 horas
    }
}

