#include "Agenda.h"
#include "Validacao.h"
#include <type_traits>
#include <utility>

namespace agendaFacil {
void Agenda::verificarDisponibilidade(const std::string& profissional,
                                     const std::string& data, int horaInicio,
                                     std::optional<std::size_t> indiceIgnorado) const {
    // A regra fornecida compara os inicios; ampliar para sobreposicao de
    // intervalos alteraria a regra de negocio especificada neste projeto.
    for (std::size_t indice = 0; indice < atendimentos.size(); ++indice) {
        if (indiceIgnorado && indice == *indiceIgnorado) {
            continue;
        }
        const Atendimento& atendimento = atendimentos[indice];
        if (atendimento.profissional == profissional &&
            atendimento.data == data && atendimento.horaInicio == horaInicio) {
            throw HorarioOcupado("Horario ocupado: profissional = '" + profissional +
                "', data = '" + data + "', hora = " + std::to_string(horaInicio) + "h.");
        }
    }
}

std::size_t Agenda::localizarAtendimento(const std::string& paciente,
                                       const std::string& data, int horaInicio) const {
    std::optional<std::size_t> encontrado;
    const std::string identificacao = "paciente = '" + paciente + "', data = '" +
        data + "', hora = " + std::to_string(horaInicio) + "h";

    for (std::size_t indice = 0; indice < atendimentos.size(); ++indice) {
        const Atendimento& atendimento = atendimentos[indice];
        if (atendimento.paciente != paciente || atendimento.data != data ||
            atendimento.horaInicio != horaInicio) {
            continue;
        }
        if (encontrado) {
            throw std::runtime_error("Identificacao ambigua: " + identificacao +
                                     ". Mais de um atendimento corresponde aos dados.");
        }
        encontrado = indice;
    }

    if (!encontrado) {
        throw std::runtime_error("Atendimento nao encontrado: " + identificacao + ".");
    }
    return *encontrado;
}

void Agenda::marcarAtendimento(const std::string& paciente,
                              const std::string& profissional,
                              const std::string& data,
                              int horaInicio, int duracaoMinutos) {
    detalhe::validarNome(paciente, "Paciente");
    detalhe::validarNome(profissional, "Profissional");
    detalhe::validarData(data);
    detalhe::validarHoraInicio(horaInicio);
    detalhe::validarDuracao(duracaoMinutos);
    detalhe::validarTermino(horaInicio, duracaoMinutos);
    verificarDisponibilidade(profissional, data, horaInicio);

    atendimentos.push_back({paciente, profissional, data, horaInicio, duracaoMinutos,
                            STATUS_MARCADO});
}

void Agenda::remarcarAtendimento(const std::string& paciente,
                                const std::string& dataAtual, int horaAtual,
                                const std::string& novaData, int novaHora) {
    detalhe::validarNome(paciente, "Paciente");
    detalhe::validarData(dataAtual);
    detalhe::validarHoraInicio(horaAtual);
    detalhe::validarData(novaData);
    detalhe::validarHoraInicio(novaHora);

    const std::size_t indice = localizarAtendimento(paciente, dataAtual, horaAtual);
    const Atendimento& atual = atendimentos[indice];
    detalhe::validarTermino(novaHora, atual.duracaoMinutos);
    verificarDisponibilidade(atual.profissional, novaData, novaHora, indice);

    // Preparar uma copia antes da substituicao preserva o registro original
    // caso a alocacao da nova data falhe.
    Atendimento remarcado = atual;
    remarcado.data = novaData;
    remarcado.horaInicio = novaHora;
    static_assert(std::is_nothrow_move_assignable_v<Atendimento>);
    atendimentos[indice] = std::move(remarcado);
}

std::vector<Atendimento> Agenda::listarAtendimentos(const std::string& data) const {
    detalhe::validarData(data);

    std::vector<Atendimento> resultado;
    for (const Atendimento& atendimento : atendimentos) {
        if (atendimento.data == data) {
            resultado.push_back(atendimento);
        }
    }
    return resultado;
}
}
