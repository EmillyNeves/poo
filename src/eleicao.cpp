#include "eleicao.hpp"

// recebe o mapa e cria TODOS os partidos antecipadamente
Eleicao::Eleicao(const std::map<int, std::string>& mapaDePartidos) {
    for(const auto& par : mapaDePartidos){
        int numero = par.first;
        std::string sigla = par.second;
        this->partidos[numero] = std::make_shared<Partido>(numero, sigla);
    }
}

void Eleicao::addCandidato(const std::shared_ptr<Candidato>& candidato) {
    if (!candidato) return;

    //mapa geral de candidatos
    candidatos[candidato->getNumeroCandidato()] = candidato;

    //add candidato ao seu respectivo partido
    auto it = partidos.find(candidato->getNumeroPartido());
    if (it != partidos.end()) {
        it->second->adicionarCandidato(candidato);
    }
}

void Eleicao::computaVotos(const Voto& voto) {
    if (voto.isLegenda()) {
        auto it = partidos.find(voto.getNumeroVotavel());
        if (it != partidos.end()) {
            it->second->adicionarVotosLegenda(voto.getQtdVotos());
        }
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