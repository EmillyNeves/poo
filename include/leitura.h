#ifndef LEITURA_H
#define LEITURA_H

#include "tipos.h"
#include <vector>
#include <string>

class Leitura {
public:
    static std::vector<DadosCandidato> lerArquivoCandidatos(const std::string& caminhoArquivo, int codigoMunicipioFiltro);
    static std::vector<DadosVoto> lerArquivoVotacao(const std::string& caminhoArquivo, int codigoMunicipioFiltro);

private:
    static std::string iso_8859_1_to_utf8(const std::string &str);
    static std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif 