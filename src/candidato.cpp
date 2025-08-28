#include "candidato.hpp"
#include <sstream>

Candidato::Candidato(const DadosCandidato &dados)
    : numeroCandidato(dados.numeroCandidato),
    nomeUrna(dados.nomeUrna),
    numeroPartido(dados.numeroPartido),
    siglaPartido(dados.siglaPartido),
    numeroFederacao(dados.numeroFederacao),
    votosNominais(0)
{
    eleito = (dados.situacaoTurno == 2 || dados.situacaoTurno == 3);
    valido = (dados.situacaoTurno != 1 && dados.situacaoTurno != -1);
    genero = (dados.genero == 2) ? Genero::MASCULINO : Genero::FEMININO;

    // data nascimento
    if (!dados.dataNascimento.empty())
    {
        std::stringstream dss(dados.dataNascimento);
        int dia, mes, ano;
        char discard;
        if (dss >> dia >> discard >> mes >> discard >> ano && discard == '/')
        {
            dataNascimento = Date{ano, mes, dia};
        }
    }
}

void Candidato::adicionarVotos(int qtd)
{
    this->votosNominais += qtd;
}

int Candidato::getIdade(const Date &dataEleicao) const
{
    if (!dataNascimento.has_value())
        return -1;

    int diff = dataEleicao.ano - dataNascimento->ano;
    if (dataEleicao.mes < dataNascimento->mes ||
        (dataEleicao.mes == dataNascimento->mes && dataEleicao.dia < dataNascimento->dia))
    {
        diff--;
    }
    return diff;
}

bool Candidato::isEleito() const { return eleito; }
bool Candidato::isFederacao() const { return numeroFederacao != -1; }
bool Candidato::ehValido() const { return valido; }
int Candidato::getNumeroCandidato() const { return numeroCandidato; }
const std::string &Candidato::getNomeUrna() const { return nomeUrna; }
int Candidato::getNumeroPartido() const { return numeroPartido; }
const std::string &Candidato::getSiglaPartido() const { return siglaPartido; }
int Candidato::getVotosNominais() const { return votosNominais; }
const std::optional<Date> &Candidato::getDataNascimento() const { return dataNascimento; }
Genero Candidato::getGenero() const { return genero; }

// mais votado primeiro, mais velho como desempate
bool Candidato::operator>(const Candidato &other) const
{
    if (this->votosNominais != other.votosNominais)
    {
        return this->votosNominais > other.votosNominais;
    }
    if (this->dataNascimento.has_value() && other.dataNascimento.has_value())
    {
        if (this->dataNascimento->ano != other.dataNascimento->ano)
        {
            return this->dataNascimento->ano < other.dataNascimento->ano;
        }
        if (this->dataNascimento->mes != other.dataNascimento->mes)
        {
            return this->dataNascimento->mes < other.dataNascimento->mes;
        }
        return this->dataNascimento->dia < other.dataNascimento->dia;
    }
    return false; // caso nao haja datas
}
