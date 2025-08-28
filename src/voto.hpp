#ifndef VOTO_HPP
#define VOTO_HPP

#include <string>

// estrutura para carregar os dados brutos de um voto do CSV
struct DadosVoto {
    int codigoMunicipio;
    int codigoCargo;
    int numeroVotavel;
    int qtdVotos;
    std::string nomeVotavel; // sigla do partido ou nome do candidato
};

class Voto {
private:
    int numeroVotavel;
    int qtdVotos;
    std::string nomeVotavel;

public:
    Voto(const DadosVoto &dados);
    int getNumeroVotavel() const;
    int getQtdVotos() const;
    const std::string& getNomeVotavel() const;
    bool isLegenda() const;
};

#endif 
