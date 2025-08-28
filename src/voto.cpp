#include "voto.hpp"

Voto::Voto(const DadosVoto &dados)
    : numeroVotavel(dados.numeroVotavel), qtdVotos(dados.qtdVotos), nomeVotavel(dados.nomeVotavel) {}

int Voto::getNumeroVotavel() const { return numeroVotavel; }
int Voto::getQtdVotos() const { return qtdVotos; }
const std::string& Voto::getNomeVotavel() const { return nomeVotavel; }
bool Voto::isLegenda() const { return numeroVotavel >= 10 && numeroVotavel <= 90; }
