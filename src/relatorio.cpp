#include "relatorio.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <unordered_set>
#include <numeric>

// classe para formatar numeros com separador de milhar '.' e decimal ','
struct Punct_pt_BR : std::numpunct<char> {
protected:
    char do_thousands_sep() const override { return '.'; }
    std::string do_grouping() const override { return "\3"; }
    char do_decimal_point() const override { return ','; }
};

// funcao principal que orquestra a geração de todos os relatorios
void Relatorio::gerarRelatorios(const Eleicao& eleicao, const Date& dataEleicao) {
    std::locale old_locale = std::cout.imbue(std::locale(std::cout.getloc(), new Punct_pt_BR));

    auto todosCandidatos = eleicao.getTodosCandidatos();
    auto todosPartidos = eleicao.getTodosPartidos();

    std::vector<std::shared_ptr<Candidato>> eleitos;
    for (const auto& c : todosCandidatos) {
        if (c->isEleito()) eleitos.push_back(c);
    }
    std::sort(eleitos.begin(), eleitos.end(), [](const auto& a, const auto& b) { return *a > *b; });

    std::vector<std::shared_ptr<Candidato>> maisVotados = todosCandidatos;
    std::sort(maisVotados.begin(), maisVotados.end(), [](const auto& a, const auto& b) { return *a > *b; });
    
    std::sort(todosPartidos.begin(), todosPartidos.end(), [](const auto& a, const auto& b) { return *a > *b; });

    gerarRelatorioVagas(eleitos.size());
    gerarRelatorioEleitos(eleitos);
    gerarRelatorioMaisVotados(maisVotados, eleitos.size());
    gerarRelatorioNaoEleitosMajoritaria(maisVotados, eleitos, eleitos.size());
    gerarRelatorioEleitosProporcional(maisVotados, eleitos, eleitos.size());
    gerarRelatorioPartidos(todosPartidos);
    gerarRelatorioPrimeiroUltimo(todosPartidos);
    gerarRelatorioFaixaEtaria(eleitos, dataEleicao);
    gerarRelatorioGenero(eleitos);
    gerarRelatorioTotalVotos(todosPartidos);

    std::cout.imbue(std::locale("C")); // restaura o locale para evitar vazamento de memoria
}

// funcao auxiliar para imprimir um candidato
void Relatorio::imprimirLinhaCandidato(const Candidato& candidato, int posicao) {
    std::cout << posicao << " - " 
              << (candidato.isFederacao() ? "*" : "") 
              << candidato.getNomeUrna() << " (" 
              << candidato.getSiglaPartido() << ", " 
              << candidato.getVotosNominais() << " votos)" << std::endl;
}

// implementacoes de cada relatorio em funcoes separadas
void Relatorio::gerarRelatorioVagas(size_t numVagas) {
    std::cout << "Número de vagas: " << numVagas << std::endl;
}

void Relatorio::gerarRelatorioEleitos(const std::vector<std::shared_ptr<Candidato>>& eleitos) {
    std::cout << "\nVereadores eleitos:" << std::endl;
    int pos = 1;
    for (const auto& c : eleitos) {
        imprimirLinhaCandidato(*c, pos++);
    }
}

void Relatorio::gerarRelatorioMaisVotados(const std::vector<std::shared_ptr<Candidato>>& maisVotados, size_t numVagas) {
    std::cout << "\nCandidatos mais votados (em ordem decrescente de votação e respeitando número de vagas):" << std::endl;
    for (size_t i = 0; i < numVagas && i < maisVotados.size(); ++i) {
        imprimirLinhaCandidato(*maisVotados[i], i + 1);
    }
}

void Relatorio::gerarRelatorioNaoEleitosMajoritaria(const std::vector<std::shared_ptr<Candidato>>& maisVotados, const std::vector<std::shared_ptr<Candidato>>& eleitos, size_t numVagas) {
    std::unordered_set<std::shared_ptr<Candidato>> eleitosSet(eleitos.begin(), eleitos.end());
    std::cout << "\nTeriam sido eleitos se a votação fosse majoritária, e não foram eleitos:" << std::endl;
    std::cout << "(com sua posição no ranking de mais votados)" << std::endl;
    for (size_t i = 0; i < numVagas && i < maisVotados.size(); ++i) {
        if (eleitosSet.find(maisVotados[i]) == eleitosSet.end()) {
            imprimirLinhaCandidato(*maisVotados[i], i + 1);
        }
    }
}

void Relatorio::gerarRelatorioEleitosProporcional(const std::vector<std::shared_ptr<Candidato>>& maisVotados, const std::vector<std::shared_ptr<Candidato>>& eleitos, size_t numVagas) {
    std::unordered_set<std::shared_ptr<Candidato>> topVotadosSet(maisVotados.begin(), maisVotados.begin() + std::min(numVagas, maisVotados.size()));
    std::cout << "\nEleitos, que não seriam eleitos se a votação fosse majoritária:" << std::endl;
    std::cout << "(com sua posição no ranking de mais votados)" << std::endl;
    for (const auto& c : eleitos) {
        if (topVotadosSet.find(c) == topVotadosSet.end()) {
            auto it = std::find(maisVotados.begin(), maisVotados.end(), c);
            int pos = (it != maisVotados.end()) ? (std::distance(maisVotados.begin(), it) + 1) : 0;
            imprimirLinhaCandidato(*c, pos);
        }
    }
}

void Relatorio::gerarRelatorioPartidos(const std::vector<std::shared_ptr<Partido>>& partidos) {
    std::cout << "\nVotação dos partidos e número de candidatos eleitos:" << std::endl;
    int pos = 1;
    for (const auto& p : partidos) {
        int votosTotais = p->getVotosTotaisDoPartido();
        if (votosTotais == 0) continue;
        int votosNominais = p->getVotosNominais();
        int votosLegenda = p->getVotosLegenda();
        int eleitos = p->getCandidatosEleitos();
        std::cout << pos++ << " - " << p->getSiglaPartido() << " - " << p->getNumeroPartido() << ", "
                  << votosTotais << " voto" << (votosTotais == 1 ? "" : "s")
                  << " (" << votosNominais << " nominai" << (votosNominais == 1 ? "" : "s") << " e "
                  << votosLegenda << " de legenda), " << eleitos << " candidato"
                  << (eleitos == 1 ? "" : "s") << " eleito" << (eleitos == 1 ? "" : "s") << std::endl;
    }
}

void Relatorio::gerarRelatorioPrimeiroUltimo(const std::vector<std::shared_ptr<Partido>>& partidos) {
    std::cout << "\nPrimeiro e último colocados de cada partido:" << std::endl;
    int pos = 1;
    for (const auto& p : partidos) {
        if (p->getVotosNominais() == 0) continue;
        auto maisVotado = p->getCandidatoMaisVotado();
        auto menosVotado = p->getCandidatoMenosVotado();
        if (maisVotado && menosVotado) {
            std::cout << pos++ << " - " << p->getSiglaPartido() << " - " << p->getNumeroPartido() << ", "
                      << maisVotado->getNomeUrna() << " (" << maisVotado->getNumeroCandidato() << ", " 
                      << maisVotado->getVotosNominais() << (maisVotado->getVotosNominais() == 1 ? " voto" : " votos") << ") / "
                      << menosVotado->getNomeUrna() << " (" << menosVotado->getNumeroCandidato() << ", " 
                      << menosVotado->getVotosNominais() << (menosVotado->getVotosNominais() == 1 ? " voto" : " votos") << ")" << std::endl;
        }
    }
}

void Relatorio::gerarRelatorioFaixaEtaria(const std::vector<std::shared_ptr<Candidato>>& eleitos, const Date& dataEleicao) {
    if (eleitos.empty()) return;
    int menor30=0, de30a39=0, de40a49=0, de50a59=0, maior60=0;
    for(const auto& c : eleitos){
        int idade = c->getIdade(dataEleicao);
        if(idade < 30) menor30++;
        else if(idade < 40) de30a39++;
        else if(idade < 50) de40a49++;
        else if(idade < 60) de50a59++;
        else maior60++;
    }
    std::cout << "\nEleitos, por faixa etária (na data da eleição):" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    double totalEleitos = eleitos.size();
    std::cout << "      Idade < 30: " << menor30 << " (" << (menor30 * 100.0 / totalEleitos) << "%)" << std::endl;
    std::cout << "30 <= Idade < 40: " << de30a39 << " (" << (de30a39 * 100.0 / totalEleitos) << "%)" << std::endl;
    std::cout << "40 <= Idade < 50: " << de40a49 << " (" << (de40a49 * 100.0 / totalEleitos) << "%)" << std::endl;
    std::cout << "50 <= Idade < 60: " << de50a59 << " (" << (de50a59 * 100.0 / totalEleitos) << "%)" << std::endl;
    std::cout << "60 <= Idade     : " << maior60 << " (" << (maior60 * 100.0 / totalEleitos) << "%)" << std::endl;
}

void Relatorio::gerarRelatorioGenero(const std::vector<std::shared_ptr<Candidato>>& eleitos) {
    if (eleitos.empty()) return;
    long feminino = std::count_if(eleitos.begin(), eleitos.end(), [](const auto&c){ return c->getGenero() == Genero::FEMININO;});
    double totalEleitos = eleitos.size();
    std::cout << "\nEleitos, por gênero:" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Feminino:  " << feminino << " (" << (feminino * 100.0 / totalEleitos) << "%)" << std::endl;
    std::cout << "Masculino: " << (long(totalEleitos) - feminino) << " (" << ((totalEleitos - feminino) * 100.0 / totalEleitos) << "%)" << std::endl;
}

void Relatorio::gerarRelatorioTotalVotos(const std::vector<std::shared_ptr<Partido>>& partidos) {
    int totalVotosValidos = std::accumulate(partidos.begin(), partidos.end(), 0, [](int sum, const auto& p){ return sum + p->getVotosTotaisDoPartido(); });
    int totalNominais = std::accumulate(partidos.begin(), partidos.end(), 0, [](int sum, const auto& p){ return sum + p->getVotosNominais(); });
    int totalLegenda = totalVotosValidos - totalNominais;

    std::cout << "\nTotal de votos válidos:    " << totalVotosValidos << std::endl;
    if (totalVotosValidos > 0) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Total de votos nominais:   " << totalNominais << " (" << (totalNominais * 100.0 / totalVotosValidos) << "%)" << std::endl;
        std::cout << "Total de votos de legenda: " << totalLegenda << " (" << (totalLegenda * 100.0 / totalVotosValidos) << "%)" << std::endl;
    } else {
        std::cout << "Total de votos nominais:   0 (0,00%)" << std::endl;
        std::cout << "Total de votos de legenda: 0 (0,00%)" << std::endl;
    }
}