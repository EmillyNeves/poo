#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "leitura.hpp"
#include "eleicao.hpp"
#include "candidato.hpp"
#include "voto.hpp"
#include "relatorio.hpp"
#include "tipos.hpp"

// funcao auxiliar para converter string de data (dd/mm/aaaa) para a struct Date
Date parseData(const std::string& strData) {
    Date data;
    sscanf(strData.c_str(), "%d/%d/%d", &data.dia, &data.mes, &data.ano);
    return data;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Erro: Número incorreto de argumentos." << std::endl;
        std::cerr << "Uso: ./vereadores <cd_municipio> <caminho_candidatos> <caminho_votacao> <data>" << std::endl;
        return 1;
    }

    try {
        int codigoMunicipio = std::stoi(argv[1]);
        std::string caminhoCandidatos = argv[2];
        std::string caminhoVotacao = argv[3];
        Date dataEleicao = parseData(argv[4]);

        // leitura dos arquivos
        Leitura leitor;
        std::vector<DadosCandidato> dadosCandidatos = leitor.lerArquivoCandidatos(caminhoCandidatos, codigoMunicipio);
        std::vector<DadosVoto> dadosVotos = leitor.lerArquivoVotacao(caminhoVotacao, codigoMunicipio);

        // faz a eleicão
        Eleicao eleicao;

        // add candidatos
        for (const auto& dc : dadosCandidatos) {
            eleicao.addCandidato(std::make_shared<Candidato>(dc));
        }

        // computa os votos
        for (const auto& dv : dadosVotos) {
            eleicao.computaVotos(Voto(dv));
        }

        // geracao dos Relatórios
        Relatorio::gerarRelatorios(eleicao, dataEleicao);

    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro: Argumento inválido. Verifique o código do município.\n" << std::endl;
        return 1;
    }

    return 0;
}