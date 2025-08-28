#ifndef ELEICAO_HPP
#define ELEICAO_HPP

#include "candidato.hpp"
#include "partido.hpp"
#include "voto.hpp"
#include <map>
#include <memory>
#include <vector>

class Eleicao {
private:
    std::map<int, std::shared_ptr<Candidato>> candidatos;
    std::map<int, std::shared_ptr<Partido>> partidos;

public:
    void addCandidato(const std::shared_ptr<Candidato>& candidato);
    void computaVotos(const Voto& voto);
    std::vector<std::shared_ptr<Candidato>> getTodosCandidatos() const;
    std::vector<std::shared_ptr<Partido>> getTodosPartidos() const;
};

#endif 