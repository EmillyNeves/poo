#ifndef CANDIDATO_HPP
#define CANDIDATO_HPP

#include <string>
#include <optional>
#include "tipos.hpp"

enum class Genero { MASCULINO, FEMININO };

struct Date {
    int ano;
    int mes;
    int dia;
};

class Candidato {
private:
    int numeroCandidato;
    std::string nomeUrna;
    int numeroPartido;
    std::string siglaPartido;
    int numeroFederacao;
    std::optional<Date> dataNascimento;
    Genero genero;
    bool eleito;
    bool valido;
    int votosNominais;

public:
    Candidato(const DadosCandidato &dados);

    void adicionarVotos(int qtd);
    int getIdade(const Date &dataEleicao) const;

    bool isEleito() const;
    bool isFederacao() const;
    bool ehValido() const;
    int getNumeroCandidato() const;
    const std::string &getNomeUrna() const;
    int getNumeroPartido() const;
    const std::string &getSiglaPartido() const;
    int getVotosNominais() const;
    const std::optional<Date> &getDataNascimento() const;
    Genero getGenero() const;

    bool operator>(const Candidato &other) const;
};

#endif 