#ifndef RELATORIO_H
#define RELATORIO_H

#include "eleicao.h"

struct Date;

class Relatorio {
public:
    static void gerarRelatorios(const Eleicao& eleicao, const Date& dataEleicao);

private:
    static void imprimirLinhaCandidato(const Candidato& candidato, int posicao);
    static void gerarRelatorioVagas(size_t numVagas);
    static void gerarRelatorioEleitos(const std::vector<std::shared_ptr<Candidato>>& eleitos);
    static void gerarRelatorioMaisVotados(const std::vector<std::shared_ptr<Candidato>>& maisVotados, size_t numVagas);
    static void gerarRelatorioNaoEleitosMajoritaria(const std::vector<std::shared_ptr<Candidato>>& maisVotados, const std::vector<std::shared_ptr<Candidato>>& eleitos, size_t numVagas);
    static void gerarRelatorioEleitosProporcional(const std::vector<std::shared_ptr<Candidato>>& maisVotados, const std::vector<std::shared_ptr<Candidato>>& eleitos, size_t numVagas);
    static void gerarRelatorioPartidos(const std::vector<std::shared_ptr<Partido>>& partidos);
    static void gerarRelatorioPrimeiroUltimo(const std::vector<std::shared_ptr<Partido>>& partidos);
    static void gerarRelatorioFaixaEtaria(const std::vector<std::shared_ptr<Candidato>>& eleitos, const Date& dataEleicao);
    static void gerarRelatorioGenero(const std::vector<std::shared_ptr<Candidato>>& eleitos);
    static void gerarRelatorioTotalVotos(const std::vector<std::shared_ptr<Partido>>& partidos);
};

#endif 