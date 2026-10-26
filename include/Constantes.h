#ifndef AGENDA_FACIL_CONSTANTES_H
#define AGENDA_FACIL_CONSTANTES_H

/** @file Constantes.h
 * @brief Limites e valores definidos pelas regras de negócio da clínica.
 */
namespace agendaFacil {
inline constexpr int HORA_ABERTURA = 8;
inline constexpr int HORA_ULTIMO_INICIO = 17;
inline constexpr int HORA_FECHAMENTO = 18;
inline constexpr int MINUTOS_POR_HORA = 60;
inline constexpr int DURACAO_MINIMA_MINUTOS = 15;
inline constexpr int DURACAO_MAXIMA_MINUTOS = 120;
inline constexpr int INTERVALO_DURACAO_MINUTOS = 15;
inline constexpr int ANO_MINIMO = 1;
inline constexpr int ANO_MAXIMO = 9999;
inline constexpr char STATUS_MARCADO[] = "MARCADO";
}

#endif
