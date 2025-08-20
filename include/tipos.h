#pragma once

#include <string>
#include <vector>

// carregar os dados brutos de um candidato do CSV
struct DadosCandidato {
    int codigoMunicipio;
    int codigoCargo;
    int numeroCandidato;
    std::string nomeUrna;
    std::string nomeCompleto;
    int numeroPartido;
    std::string siglaPartido;
    int numeroFederacao;
    std::string dataNascimento; // lida como string, processada pelo construtor de Candidato
    int genero;
    int situacaoTurno;
};

// carregar os dados brutos de um voto do CSV
struct DadosVoto {
    int codigoMunicipio;
    int codigoCargo;
    int numeroVotavel;
    int qtdVotos;
};