#ifndef PARTIDO_H
#define PARTIDO_H

#include "candidato.h"
#include <memory>
#include <string>
#include <vector>

class Partido {
private:
    int numero;
    std::string sigla;
    int votosLegenda;
    std::vector<std::shared_ptr<Candidato>> candidatos;

public:
    Partido(int numero, const std::string& sigla);
    void adicionarCandidato(const std::shared_ptr<Candidato>& c);
    void adicionarVotosLegenda(int qtd);
    int getVotosLegenda() const;
    int getVotosNominais() const;
    int getVotosTotaisDoPartido() const;
    int getCandidatosEleitos() const;
    const std::string& getSiglaPartido() const;
    int getNumeroPartido() const;
    void setSigla(const std::string& novaSigla);
    std::shared_ptr<Candidato> getCandidatoMaisVotado() const;
    std::shared_ptr<Candidato> getCandidatoMenosVotado() const;

    bool operator>(const Partido& other) const;
};

#endif 