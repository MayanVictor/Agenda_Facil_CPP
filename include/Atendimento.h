#ifndef AGENDA_FACIL_ATENDIMENTO_H
#define AGENDA_FACIL_ATENDIMENTO_H

#include "Constantes.h"
#include <string>

/** @file Atendimento.h
 * @brief Representação dos dados de um atendimento.
 */
namespace agendaFacil {
/** @brief Dados de um atendimento; a Agenda valida os registros que armazena. */
struct Atendimento {
    std::string paciente;      ///< Nome do paciente.
    std::string profissional;  ///< Nome do profissional responsável.
    std::string data;          ///< Data no formato AAAA-MM-DD.
    int horaInicio;            ///< Hora inteira de início, entre 8 e 17.
    int duracaoMinutos;        ///< Duração em minutos, entre 15 e 120.
    std::string status = STATUS_MARCADO; ///< Estado do agendamento.
};
}

#endif
