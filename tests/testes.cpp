#include "Agenda.h"
#include <climits>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
int verificacoes = 0;

void exigir(bool condicao, const std::string& descricao) {
    if (!condicao) {
        throw std::runtime_error("Teste falhou: " + descricao);
    }
    ++verificacoes;
}

template <typename Excecao, typename Acao>
void exigirExcecao(Acao acao, const std::string& trechoEsperado) {
    try {
        acao();
    } catch (const Excecao& erro) {
        exigir(std::string(erro.what()).find(trechoEsperado) != std::string::npos,
               "mensagem deve conter '" + trechoEsperado + "', recebida: " + erro.what());
        return;
    }
    throw std::runtime_error("Teste falhou: excecao esperada nao foi lancada.");
}

void testarDemonstracao() {
    agendaFacil::Agenda agenda;
    agenda.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", 9, 30);
    exigirExcecao<agendaFacil::HorarioOcupado>([&] {
        agenda.marcarAtendimento("Bruno", "Dr. Silva", "2026-10-05", 9, 30);
    }, "Dr. Silva");
    agenda.marcarAtendimento("Carla", "Dra. Souza", "2026-10-05", 14, 60);
    agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-06", 10);

    const auto diaOriginal = agenda.listarAtendimentos("2026-10-05");
    exigir(diaOriginal.size() == 1 && diaOriginal[0].paciente == "Carla" &&
           diaOriginal[0].horaInicio == 14 && diaOriginal[0].status == "MARCADO",
           "listagem original deve conter somente Carla as 14h");
    const auto novoDia = agenda.listarAtendimentos("2026-10-06");
    exigir(novoDia.size() == 1 && novoDia[0].paciente == "Ana" &&
           novoDia[0].horaInicio == 10 && novoDia[0].duracaoMinutos == 30,
           "Ana deve ser remarcada preservando a duracao");
}

void testarHorariosEDuracoes() {
    for (int hora : {INT_MIN, -1, 7, 18, 20, INT_MAX}) {
        agendaFacil::Agenda agenda;
        exigirExcecao<std::invalid_argument>([&] {
            agenda.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", hora, 30);
        }, std::to_string(hora));
        exigir(agenda.listarAtendimentos("2026-10-05").empty(), "hora invalida nao insere");
    }
    for (int duracao : {INT_MIN, -15, 0, 5, 14, 16, 25, 121, 150, INT_MAX}) {
        agendaFacil::Agenda agenda;
        exigirExcecao<std::invalid_argument>([&] {
            agenda.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", 9, duracao);
        }, std::to_string(duracao));
        exigir(agenda.listarAtendimentos("2026-10-05").empty(), "duracao invalida nao insere");
    }

    agendaFacil::Agenda agenda;
    agenda.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", 8, 15);
    agenda.marcarAtendimento("Bruno", "Dr. Silva", "2026-10-05", 16, 120);
    agenda.marcarAtendimento("Carla", "Dra. Souza", "2026-10-05", 17, 60);
    exigir(agenda.listarAtendimentos("2026-10-05").size() == 3,
           "inicio as 8h, duracao minima e fim exatamente as 18h sao validos");
    exigirExcecao<std::invalid_argument>([&] {
        agenda.marcarAtendimento("Daniel", "Dr. Lima", "2026-10-05", 17, 75);
    }, "75");

    for (int duracao = 15; duracao <= 120; duracao += 15) {
        agendaFacil::Agenda outra;
        outra.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", 8, duracao);
        exigir(outra.listarAtendimentos("2026-10-05")[0].duracaoMinutos == duracao,
               "todos os multiplos de 15 na faixa sao aceitos");
    }
}

void testarDatasENomes() {
    for (const std::string data : {"", "2026-1-05", "05/10/2026", "2026/10/05",
         "2026-10-05x", "20a6-10-05", "2026-00-05", "2026-13-05", "2026-01-00",
         "2026-04-31", "2026-02-29", "1900-02-29", "0000-01-01", "2026-10- 5"}) {
        agendaFacil::Agenda agenda;
        exigirExcecao<std::invalid_argument>([&] {
            agenda.marcarAtendimento("Ana", "Dr. Silva", data, 9, 30);
        }, "'" + data + "'");
        exigirExcecao<std::invalid_argument>([&] {
            agenda.listarAtendimentos(data);
        }, "'" + data + "'");
    }

    for (const std::string data : {"0001-01-01", "2000-02-29", "2024-02-29",
                                  "2026-02-28", "9999-12-31"}) {
        agendaFacil::Agenda agenda;
        agenda.marcarAtendimento("Ana", "Dr. Silva", data, 9, 30);
        exigir(agenda.listarAtendimentos(data).size() == 1, "data valida " + data);
    }

    for (const std::string nome : {"", "   ", "\t\n"}) {
        agendaFacil::Agenda agenda;
        exigirExcecao<std::invalid_argument>([&] {
            agenda.marcarAtendimento(nome, "Dr. Silva", "2026-10-05", 9, 30);
        }, "Paciente");
        exigirExcecao<std::invalid_argument>([&] {
            agenda.marcarAtendimento("Ana", nome, "2026-10-05", 9, 30);
        }, "Profissional");
    }
}

void testarConflitos() {
    agendaFacil::Agenda agenda;
    agenda.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", 9, 120);
    agenda.marcarAtendimento("Bruno", "Dra. Souza", "2026-10-05", 9, 30);
    agenda.marcarAtendimento("Carla", "Dr. Silva", "2026-10-06", 9, 30);
    agenda.marcarAtendimento("Daniel", "Dr. Silva", "2026-10-05", 10, 30);
    exigir(agenda.listarAtendimentos("2026-10-05").size() == 3,
           "conflito usa profissional, data e inicio; nao testa sobreposicao");
    exigirExcecao<agendaFacil::HorarioOcupado>([&] {
        agenda.marcarAtendimento("Eva", "Dr. Silva", "2026-10-05", 9, 30);
    }, "2026-10-05");
    exigir(agenda.listarAtendimentos("2026-10-05").size() == 3,
           "conflito nao deve inserir um atendimento");
}

void testarRemarcacoes() {
    agendaFacil::Agenda agenda;
    agenda.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", 9, 120);
    agenda.marcarAtendimento("Bruno", "Dr. Silva", "2026-10-06", 10, 30);
    agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-05", 9);
    exigir(agenda.listarAtendimentos("2026-10-05").size() == 1,
           "remarcar para o proprio inicio deve funcionar");

    exigirExcecao<agendaFacil::HorarioOcupado>([&] {
        agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-06", 10);
    }, "10h");
    exigirExcecao<std::invalid_argument>([&] {
        agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-06", 17);
    }, "120");
    exigirExcecao<std::invalid_argument>([&] {
        agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-06", 20);
    }, "20");
    exigirExcecao<std::invalid_argument>([&] {
        agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-02-30", 10);
    }, "2026-02-30");
    exigirExcecao<std::invalid_argument>([&] {
        agenda.remarcarAtendimento("Ana", "data", 9, "2026-10-06", 10);
    }, "data");
    exigirExcecao<std::invalid_argument>([&] {
        agenda.remarcarAtendimento("Ana", "2026-10-05", 20, "2026-10-06", 10);
    }, "20");
    exigirExcecao<std::invalid_argument>([&] {
        agenda.remarcarAtendimento(" ", "2026-10-05", 9, "2026-10-06", 10);
    }, "Paciente");
    exigirExcecao<std::runtime_error>([&] {
        agenda.remarcarAtendimento("Inexistente", "2026-10-05", 9, "2026-10-06", 10);
    }, "Inexistente");
    const auto anterior = agenda.listarAtendimentos("2026-10-05");
    exigir(anterior.size() == 1 && anterior[0].horaInicio == 9 &&
           anterior[0].paciente == "Ana", "falhas preservam o registro original");

    agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-07", 16);
    exigir(agenda.listarAtendimentos("2026-10-05").empty(), "remarcacao remove da data antiga");
    const auto remarcado = agenda.listarAtendimentos("2026-10-07");
    exigir(remarcado.size() == 1 && remarcado[0].horaInicio == 16 &&
           remarcado[0].duracaoMinutos == 120 && remarcado[0].profissional == "Dr. Silva" &&
           remarcado[0].status == "MARCADO", "remarcacao conserva os outros campos");
}

void testarAmbiguidadeEEncapsulamento() {
    agendaFacil::Agenda agenda;
    agenda.marcarAtendimento("Ana", "Dr. Silva", "2026-10-05", 9, 30);
    agenda.marcarAtendimento("Ana", "Dra. Souza", "2026-10-05", 9, 30);
    exigirExcecao<std::runtime_error>([&] {
        agenda.remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-06", 10);
    }, "ambigua");
    exigir(agenda.listarAtendimentos("2026-10-05").size() == 2 &&
           agenda.listarAtendimentos("2026-10-06").empty(),
           "ambiguidade nao deve alterar nenhum registro");
    auto copia = agenda.listarAtendimentos("2026-10-05");
    copia[0].paciente = "Alterado";
    exigir(agenda.listarAtendimentos("2026-10-05")[0].paciente == "Ana",
           "alterar uma listagem nao altera a agenda");
    agendaFacil::Agenda outra;
    exigir(outra.listarAtendimentos("2026-10-05").empty(), "agendas sao independentes");
    exigir(agenda.listarAtendimentos("2026-10-08").empty(), "dia sem atendimentos e valido");
}
}

/**
 * @brief Executa verificações de regras e preservação do estado.
 * @par Parâmetros
 * Nenhum.
 * @return Zero se todas as verificações passarem.
 * @throw std::exception Propaga falhas inesperadas e reprovações dos testes.
 */
int main() {
    testarDemonstracao();
    testarHorariosEDuracoes();
    testarDatasENomes();
    testarConflitos();
    testarRemarcacoes();
    testarAmbiguidadeEEncapsulamento();
    std::cout << verificacoes << " verificacoes passaram.\n";
    return 0;
}
