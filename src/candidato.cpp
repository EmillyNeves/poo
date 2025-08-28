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
    SituacaoCandidato sit = static_cast<SituacaoCandidato>(dados.situacaoTurno);
    eleito = (sit == SituacaoCandidato::ELEITO_QP || sit == SituacaoCandidato::ELEITO_MEDIA);
    valido = (sit != SituacaoCandidato::INVALIDO && dados.situacaoTurno != -1);
    
    genero = (dados.genero == 2) ? Genero::MASCULINO : Genero::FEMININO;

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

bool Candidato::operator>(const Candidato &other) const
{
    if (this->votosNominais != other.votosNominais)
    {
        return this->votosNominais > other.votosNominais;
    }

    const auto& d1 = this->dataNascimento;
    const auto& d2 = other.dataNascimento;

    if (d1.has_value() && d2.has_value())
    {
        const auto& date1 = d1.value();
        const auto& date2 = d2.value();
        if (date1.ano != date2.ano) return date1.ano < date2.ano;
        if (date1.mes != date2.mes) return date1.mes < date2.mes;
        return date1.dia < date2.dia;
    }
    
    if (d1.has_value() && !d2.has_value()) return true;
    if (!d1.has_value() && d2.has_value()) return false;

    return false;
}