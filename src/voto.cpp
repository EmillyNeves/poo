#include "voto.hpp"

Voto::Voto(const DadosVoto &dados)
    : numeroVotavel(dados.numeroVotavel), qtdVotos(dados.qtdVotos) {}

int Voto::getNumeroVotavel() const { return numeroVotavel; }
int Voto::getQtdVotos() const { return qtdVotos; }
bool Voto::isLegenda() const { return numeroVotavel >= 10 && numeroVotavel <= 90; }