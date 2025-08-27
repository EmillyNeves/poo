#ifndef VOTO_HPP
#define VOTO_HPP

#include "tipos.hpp"

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