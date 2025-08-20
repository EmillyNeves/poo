#include "partido.h"
#include <numeric>
#include <algorithm>

Partido::Partido(int numero, const std::string &sigla)
    : numero(numero), sigla(sigla), votosLegenda(0) {}

void Partido::adicionarCandidato(const std::shared_ptr<Candidato> &c)
{
    candidatos.push_back(c);
}

void Partido::adicionarVotosLegenda(int qtd)
{
    this->votosLegenda += qtd;
}

int Partido::getVotosLegenda() const
{
    return votosLegenda;
}

int Partido::getVotosNominais() const
{
    return std::accumulate(candidatos.begin(), candidatos.end(), 0, [](int sum, const auto &c)
                           { return sum + c->getVotosNominais(); });
}

int Partido::getVotosTotaisDoPartido() const
{
    return getVotosNominais() + this->votosLegenda;
}

int Partido::getCandidatosEleitos() const
{
    return std::count_if(candidatos.begin(), candidatos.end(),
                         [](const auto &c)
                         { return c->isEleito(); });
}

const std::string &Partido::getSiglaPartido() const { return sigla; }
int Partido::getNumeroPartido() const { return numero; }
void Partido::setSigla(const std::string &novaSigla) { this->sigla = novaSigla; }

std::shared_ptr<Candidato> Partido::getCandidatoMaisVotado() const
{
    if (candidatos.empty())
        return nullptr;

    auto it = std::max_element(candidatos.begin(), candidatos.end(),
                               [](const auto &a, const auto &b)
                               {
                                   return *b > *a; // usa o operator> de Candidato
                               });
    return *it;
}

std::shared_ptr<Candidato> Partido::getCandidatoMenosVotado() const
{
    if (candidatos.empty())
        return nullptr;

    auto it = std::min_element(candidatos.begin(), candidatos.end(),
                               [](const auto &a, const auto &b)
                               {
                                   if (a->getVotosNominais() != b->getVotosNominais())
                                   {
                                       return a->getVotosNominais() < b->getVotosNominais();
                                   }
                                   // desempate-> mais novo primeiro (data de nascimento maior)
                                   if (a->getDataNascimento().has_value() && b->getDataNascimento().has_value())
                                   {
                                       const auto &da = a->getDataNascimento().value();
                                       const auto &db = b->getDataNascimento().value();
                                       if (da.year != db.year)
                                           return da.year > db.year;
                                       if (da.month != db.month)
                                           return da.month > db.month;
                                       return da.day > db.day;
                                   }
                                   return false;
                               });
    return *it;
}

bool Partido::operator>(const Partido &other) const
{
    if (getVotosTotaisDoPartido() != other.getVotosTotaisDoPartido())
    {
        return getVotosTotaisDoPartido() > other.getVotosTotaisDoPartido();
    }
    return numero < other.numero; // desempate por menor numero
}