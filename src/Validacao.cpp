#include "Validacao.h"
#include "Constantes.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <stdexcept>

namespace agendaFacil::detalhe {
namespace {
bool ehAnoBissexto(int ano) {
    return ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0);
}

bool temFormatoDeData(const std::string& data) {
    if (data.size() != 10 || data[4] != '-' || data[7] != '-') {
        return false;
    }
    for (std::size_t indice = 0; indice < data.size(); ++indice) {
        if (indice == 4 || indice == 7) {
            continue;
        }
        if (data[indice] < '0' || data[indice] > '9') {
            return false;
        }
    }
    return true;
}
}

void validarNome(const std::string& nome, const std::string& campo) {
    const bool somenteEspacos = std::all_of(nome.begin(), nome.end(),
        [](unsigned char caractere) { return std::isspace(caractere) != 0; });
    if (somenteEspacos) {
        throw std::invalid_argument(campo + " invalido: '" + nome +
                                    "'. Informe um nome que nao seja vazio ou so espacos.");
    }
}

void validarData(const std::string& data) {
    if (!temFormatoDeData(data)) {
        throw std::invalid_argument("Data invalida: '" + data +
                                    "'. Formato esperado: AAAA-MM-DD.");
    }

    const int ano = std::stoi(data.substr(0, 4));
    const int mes = std::stoi(data.substr(5, 2));
    const int dia = std::stoi(data.substr(8, 2));
    if (ano < ANO_MINIMO || ano > ANO_MAXIMO || mes < 1 || mes > 12) {
        throw std::invalid_argument("Data invalida: '" + data +
                                    "'. Ano deve estar entre 0001 e 9999 e mes entre 01 e 12.");
    }

    const std::array<int, 12> diasPorMes = {
        31, ehAnoBissexto(ano) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    if (dia < 1 || dia > diasPorMes[static_cast<std::size_t>(mes - 1)]) {
        throw std::invalid_argument("Data invalida: '" + data +
                                    "'. Dia inexistente no mes e ano informados.");
    }
}

void validarHoraInicio(int horaInicio) {
    if (horaInicio < HORA_ABERTURA || horaInicio > HORA_ULTIMO_INICIO) {
        throw std::invalid_argument("Hora de inicio invalida: " + std::to_string(horaInicio) +
            ". Informe uma hora inteira entre " + std::to_string(HORA_ABERTURA) +
            " e " + std::to_string(HORA_ULTIMO_INICIO) + ".");
    }
}

void validarDuracao(int duracaoMinutos) {
    if (duracaoMinutos < DURACAO_MINIMA_MINUTOS ||
        duracaoMinutos > DURACAO_MAXIMA_MINUTOS ||
        duracaoMinutos % INTERVALO_DURACAO_MINUTOS != 0) {
        throw std::invalid_argument("Duracao invalida: " + std::to_string(duracaoMinutos) +
            " minutos. Informe entre " + std::to_string(DURACAO_MINIMA_MINUTOS) +
            " e " + std::to_string(DURACAO_MAXIMA_MINUTOS) +
            " minutos, em multiplos de " + std::to_string(INTERVALO_DURACAO_MINUTOS) + ".");
    }
}

void validarTermino(int horaInicio, int duracaoMinutos) {
    const int fimEmMinutos = horaInicio * MINUTOS_POR_HORA + duracaoMinutos;
    if (fimEmMinutos > HORA_FECHAMENTO * MINUTOS_POR_HORA) {
        throw std::invalid_argument("Fim fora do expediente: inicio = " +
            std::to_string(horaInicio) + "h, duracao = " + std::to_string(duracaoMinutos) +
            " minutos. O atendimento deve terminar ate " + std::to_string(HORA_FECHAMENTO) + "h.");
    }
}
}
