#include "Agenda.h"
#include <iostream>

/** @file main.cpp
 * @brief Demonstração original, preservando sua ordem e saída.
 */

/**
 * @brief Executa os mesmos cinco passos da demonstração do código-base.
 * @par Parâmetros
 * Nenhum.
 * @return Zero quando a demonstração termina normalmente.
 * @throw std::exception Propaga falhas inesperadas das operações de domínio;
 *        somente o conflito esperado de Bruno é capturado.
 */
int main() {
    agendaFacil::Agenda agenda;

    agenda.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", 9, 30);
    std::cout << "Atendimento marcado para Ana\n";

    try {
        agenda.marcarAtendimento("Bruno", "Dr. Silva", "2026-10-05", 9, 30);
    } catch (const agendaFacil::HorarioOcupado&) {
        std::cout << "Horario ocupado\n";
    }

    agenda.marcarAtendimento("Carla", "Dra. Souza", "2026-10-05", 14, 60);
    std::cout << "Atendimento marcado para Carla\n";

    agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-06", 10);
    std::cout << "Remarcado\n";

    for (const auto& atendimento : agenda.listarAtendimentos("2026-10-05")) {
        std::cout << atendimento.horaInicio << ' ' << atendimento.paciente << ' '
                  << atendimento.profissional << ' ' << atendimento.status << '\n';
    }
    return 0;
}
