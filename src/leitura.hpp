#ifndef LEITURA_HPP
#define LEITURA_HPP

#include "candidato.hpp"
#include "voto.hpp"
#include <vector>
#include <string>
#include <map>

// Nova estrutura para retornar os dois resultados da leitura de candidatos de uma só vez
struct ResultadoLeituraCandidatos {
    std::map<int, std::string> mapaDePartidos;
    std::vector<DadosCandidato> candidatosDoMunicipio;
};

class Leitura {
public:
    static ResultadoLeituraCandidatos processarArquivoCandidatos(const std::string& caminhoArquivo, int codigoMunicipioFiltro);
    
    static std::vector<DadosVoto> lerArquivoVotacao(const std::string& caminhoArquivo, int codigoMunicipioFiltro);

private:
    static std::string iso_8859_1_to_utf8(const std::string &str);
    static std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif