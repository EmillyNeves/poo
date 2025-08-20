#ifndef VOTO_H
#define VOTO_H

#include "tipos.h"

class Voto {
private:
    int numeroVotavel;
    int qtdVotos;

public:
    Voto(const DadosVoto &dados);
    int getNumeroVotavel() const;
    int getQtdVotos() const;
    bool isLegenda() const;
};

#endif 