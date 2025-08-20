#include "eleicao.h"

void Eleicao::addCandidato(const std::shared_ptr<Candidato>& candidato) {
    if (!candidato) return;

    candidatos[candidato->getNumeroCandidato()] = candidato;

    int numPartido = candidato->getNumeroPartido();
    if (partidos.find(numPartido) == partidos.end()) {
        partidos[numPartido] = std::make_shared<Partido>(numPartido, candidato->getSiglaPartido());
    }
    partidos[numPartido]->adicionarCandidato(candidato);
}

void Eleicao::computaVotos(const Voto& voto) {
    if (voto.isLegenda()) {
        int numPartido = voto.getNumeroVotavel();
        if (partidos.find(numPartido) == partidos.end()) {
            partidos[numPartido] = std::make_shared<Partido>(numPartido, "");
        }
        partidos[numPartido]->adicionarVotosLegenda(voto.getQtdVotos());
    } else {
        auto it = candidatos.find(voto.getNumeroVotavel());
        if (it != candidatos.end()) {
            it->second->adicionarVotos(voto.getQtdVotos());
        }
    }
}

std::vector<std::shared_ptr<Candidato>> Eleicao::getTodosCandidatos() const {
    std::vector<std::shared_ptr<Candidato>> vec;
    vec.reserve(candidatos.size());
    for (const auto& pair : candidatos) {
        vec.push_back(pair.second);
    }
    return vec;
}

std::vector<std::shared_ptr<Partido>> Eleicao::getTodosPartidos() const {
    std::vector<std::shared_ptr<Partido>> vec;
    vec.reserve(partidos.size());
    for (const auto& pair : partidos) {
        vec.push_back(pair.second);
    }
    return vec;
}