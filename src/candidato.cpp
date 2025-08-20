#include "candidato.h"
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
        int day, month, year;
        char discard;
        if (dss >> day >> discard >> month >> discard >> year && discard == '/')
        {
            dataNascimento = Date{year, month, day};
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

    int diff = dataEleicao.year - dataNascimento->year;
    if (dataEleicao.month < dataNascimento->month ||
        (dataEleicao.month == dataNascimento->month && dataEleicao.day < dataNascimento->day))
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
        if (this->dataNascimento->year != other.dataNascimento->year)
        {
            return this->dataNascimento->year < other.dataNascimento->year;
        }
        if (this->dataNascimento->month != other.dataNascimento->month)
        {
            return this->dataNascimento->month < other.dataNascimento->month;
        }
        return this->dataNascimento->day < other.dataNascimento->day;
    }
    return false; // caso nao haja datas
}