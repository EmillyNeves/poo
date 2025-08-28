#ifndef CANDIDATO_HPP
#define CANDIDATO_HPP

#include <string>
#include <optional>

enum class Genero { MASCULINO, FEMININO };

enum class SituacaoCandidato {
    INVALIDO = 1,
    ELEITO_QP = 2,
    ELEITO_MEDIA = 3
};

struct Date {
    int ano;
    int mes;
    int dia;
};

// estrutura para carregar os dados brutos de um candidato do CSV
struct DadosCandidato {
    int codigoMunicipio;
    int codigoCargo;
    int numeroCandidato;
    std::string nomeUrna;
    std::string nomeCompleto;
    int numeroPartido;
    std::string siglaPartido;
    int numeroFederacao;
    std::string dataNascimento;
    int genero;
    int situacaoTurno;
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