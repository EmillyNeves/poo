#include "leitura.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

std::string Leitura::iso_8859_1_to_utf8(const std::string &str)
{
    std::string strOut;
    for (const unsigned char ch : str)
    {
        if (ch < 0x80)
        {
            strOut.push_back(ch);
        }
        else
        {
            strOut.push_back(0xc2 | (ch >> 6));
            strOut.push_back(0x80 | (ch & 0x3f));
        }
    }
    return strOut;
}

namespace ColunasCandidato
{
    const int CODIGO_MUNICIPIO = 11;
    const int CODIGO_CARGO = 13;
    const int NUMERO_CANDIDATO = 16;
    const int NOME_COMPLETO = 17;
    const int NOME_URNA = 18;
    const int NUMERO_PARTIDO = 25;
    const int SIGLA_PARTIDO = 26;
    const int NUMERO_FEDERACAO = 28;
    const int DATA_NASCIMENTO = 36;
    const int GENERO = 38;
    const int SITUACAO_TURNO = 48;
}

namespace ColunasVoto
{
    const int CODIGO_MUNICIPIO = 13;
    const int CODIGO_CARGO = 17;
    const int NUMERO_VOTAVEL = 19;
    const int NOME_VOTAVEL = 20;
    const int QTD_VOTOS = 21;
}

static const int COD_VEREADOR = 13;

std::vector<std::string> Leitura::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        if (!token.empty() && token.front() == '"')
            token.erase(0, 1);
        if (!token.empty() && token.back() == '"')
            token.pop_back();
        tokens.push_back(token);
    }
    return tokens;
}

std::map<int, std::string> Leitura::lerTodosOsPartidos(const std::string& caminhoArquivo) {
    std::ifstream file(caminhoArquivo);
    if (!file) {
        throw std::runtime_error("Erro: Arquivo de candidatos não encontrado para pre-carregar partidos: " + caminhoArquivo);
    }

    std::map<int, std::string> mapaDePartidos;
    std::string line;
    std::getline(file, line); 

    while (std::getline(file, line)) {
        auto fields = split(line, ';');
        if (fields.size() <= ColunasCandidato::SIGLA_PARTIDO) continue;

        try {
            int numeroPartido = std::stoi(fields[ColunasCandidato::NUMERO_PARTIDO]);
            if (mapaDePartidos.find(numeroPartido) == mapaDePartidos.end()) {
                mapaDePartidos[numeroPartido] = iso_8859_1_to_utf8(fields[ColunasCandidato::SIGLA_PARTIDO]);
            }
        } catch (const std::invalid_argument&) {
            continue;
        }
    }
    return mapaDePartidos;
}

std::vector<DadosCandidato> Leitura::lerArquivoCandidatos(const std::string &caminhoArquivo, int codigoMunicipioFiltro)
{
    std::ifstream file(caminhoArquivo);
    if (!file)
    {
        throw std::runtime_error("Erro: Arquivo de candidatos não encontrado em " + caminhoArquivo);
    }

    std::vector<DadosCandidato> dados;
    std::string line;
    std::getline(file, line); 

    while (std::getline(file, line))
    {
        auto fields = split(line, ';');
        if (fields.size() < 49)
            continue;

        try
        {
            int codigoMunicipioLinha = std::stoi(fields[ColunasCandidato::CODIGO_MUNICIPIO]);
            int codigoCargo = std::stoi(fields[ColunasCandidato::CODIGO_CARGO]);
            int situacaoTurno = std::stoi(fields[ColunasCandidato::SITUACAO_TURNO]);
            bool valido = (situacaoTurno != 1 && situacaoTurno != -1);

            if (codigoMunicipioLinha == codigoMunicipioFiltro && codigoCargo == COD_VEREADOR && valido)
            {
                DadosCandidato dado;
                dado.codigoMunicipio = codigoMunicipioLinha;
                dado.codigoCargo = codigoCargo;
                dado.situacaoTurno = situacaoTurno;
                dado.numeroCandidato = std::stoi(fields[ColunasCandidato::NUMERO_CANDIDATO]);
                dado.nomeCompleto = iso_8859_1_to_utf8(fields[ColunasCandidato::NOME_COMPLETO]);
                dado.nomeUrna = iso_8859_1_to_utf8(fields[ColunasCandidato::NOME_URNA]);
                dado.siglaPartido = iso_8859_1_to_utf8(fields[ColunasCandidato::SIGLA_PARTIDO]);
                dado.numeroPartido = std::stoi(fields[ColunasCandidato::NUMERO_PARTIDO]);
                dado.numeroFederacao = std::stoi(fields[ColunasCandidato::NUMERO_FEDERACAO]);
                dado.dataNascimento = fields[ColunasCandidato::DATA_NASCIMENTO];
                dado.genero = std::stoi(fields[ColunasCandidato::GENERO]);
                dados.push_back(dado);
            }
        }
        catch (const std::invalid_argument &)
        {
            continue;
        }
    }
    return dados;
}

std::vector<DadosVoto> Leitura::lerArquivoVotacao(const std::string &caminhoArquivo, int codigoMunicipioFiltro)
{
    std::ifstream file(caminhoArquivo);
    if (!file)
    {
        throw std::runtime_error("Erro: Arquivo de votação não encontrado em " + caminhoArquivo);
    }

    std::vector<DadosVoto> dados;
    std::string line;
    std::getline(file, line); 

    while (std::getline(file, line))
    {
        auto fields = split(line, ';');
        if (fields.size() < 22)
            continue;

        try
        {
            int codigoMunicipioLinha = std::stoi(fields[ColunasVoto::CODIGO_MUNICIPIO]);
            int codigoCargo = std::stoi(fields[ColunasVoto::CODIGO_CARGO]);
            int numVotavel = std::stoi(fields[ColunasVoto::NUMERO_VOTAVEL]);
            bool valido = (numVotavel < 95 || numVotavel > 98);

            if (codigoMunicipioLinha == codigoMunicipioFiltro && codigoCargo == COD_VEREADOR && valido)
            {
                DadosVoto dado;
                dado.codigoMunicipio = codigoMunicipioLinha;
                dado.codigoCargo = codigoCargo;
                dado.numeroVotavel = numVotavel;
                dado.nomeVotavel = iso_8859_1_to_utf8(fields[ColunasVoto::NOME_VOTAVEL]);
                std::string qtdStr = fields[ColunasVoto::QTD_VOTOS];
                qtdStr.erase(std::remove(qtdStr.begin(), qtdStr.end(), '.'), qtdStr.end());
                dado.qtdVotos = std::stoi(qtdStr);
                dados.push_back(dado);
            }
        }
        catch (const std::invalid_argument &)
        {
            continue;
        }
    }
    return dados;
}