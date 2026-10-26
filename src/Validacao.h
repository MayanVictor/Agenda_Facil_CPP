#ifndef AGENDA_FACIL_VALIDACAO_H
#define AGENDA_FACIL_VALIDACAO_H

#include <string>

namespace agendaFacil::detalhe {
void validarNome(const std::string& nome, const std::string& campo);
void validarData(const std::string& data);
void validarHoraInicio(int horaInicio);
void validarDuracao(int duracaoMinutos);
void validarTermino(int horaInicio, int duracaoMinutos);
}

#endif
