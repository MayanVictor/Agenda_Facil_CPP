#ifndef AGENDA_FACIL_AGENDA_H
#define AGENDA_FACIL_AGENDA_H

#include "Atendimento.h"
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

/** @file Agenda.h
 * @brief API pública de agendamento, remarcação e listagem em memória.
 */
namespace agendaFacil {
/** @brief Indica que o profissional já possui atendimento no início solicitado. */
class HorarioOcupado : public std::runtime_error {
public:
    /**
     * @brief Constrói uma exceção com os dados do conflito.
     * @param mensagem Descrição do profissional, da data e da hora em conflito.
     * @return Não se aplica: construtor.
     * @throw std::bad_alloc Se não houver memória para armazenar a mensagem.
     */
    explicit HorarioOcupado(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

/** @brief Agenda independente, sem impressão e sem exposição do vetor interno. */
class Agenda {
public:
    /**
     * @brief Cria uma agenda vazia.
     * @par Parâmetros
     * Nenhum.
     * @return Não se aplica: construtor.
     * @throw Nenhuma: construção sem lançamento de exceções.
     */
    Agenda() noexcept = default;

    /**
     * @brief Marca um atendimento válido, sem imprimir mensagens.
     * @param paciente Nome não vazio nem composto apenas de espaços.
     * @param profissional Nome não vazio nem composto apenas de espaços.
     * @param data Data de calendário válida no formato AAAA-MM-DD (0001–9999).
     * @param horaInicio Hora inteira entre 8 e 17, inclusive.
     * @param duracaoMinutos Duração de 15 a 120 minutos, múltipla de 15;
     *        o atendimento deve terminar até as 18h.
     * @return Nenhum valor (void). Insere um atendimento com estado MARCADO.
     * @throw std::invalid_argument Se qualquer entrada violar as pré-condições.
     * @throw HorarioOcupado Se o profissional já atender na mesma data e início.
     * @throw std::bad_alloc Se não houver memória; a agenda permanece inalterada.
     * @throw std::length_error Se o vetor atingir sua capacidade máxima.
     */
    void marcarAtendimento(const std::string& paciente,
                          const std::string& profissional,
                          const std::string& data,
                          int horaInicio, int duracaoMinutos);

    /**
     * @brief Altera data e hora de um único atendimento, mantendo seus outros dados.
     * @param paciente Nome não vazio que identifica o paciente.
     * @param dataAtual Data válida do atendimento, no formato AAAA-MM-DD.
     * @param horaAtual Hora inteira atual entre 8 e 17, inclusive.
     * @param novaData Nova data válida no formato AAAA-MM-DD (0001–9999).
     * @param novaHora Nova hora inteira entre 8 e 17; deve permitir terminar até 18h.
     * @return Nenhum valor (void). A mesma data/hora é aceita sem mudança efetiva.
     * @throw std::invalid_argument Se nome, datas ou horários forem inválidos,
     *        ou se a duração existente ultrapassar o expediente no novo horário.
     * @throw std::runtime_error Se não houver registro ou a identificação for ambígua.
     * @throw HorarioOcupado Se outro registro do profissional ocupar o novo início.
     * @throw std::bad_alloc Se não houver memória; a agenda permanece inalterada.
     */
    void remarcarAtendimento(const std::string& paciente,
                            const std::string& dataAtual, int horaAtual,
                            const std::string& novaData, int novaHora);

    /**
     * @brief Obtém os atendimentos de uma data, na ordem de inserção.
     * @param data Data de calendário válida no formato AAAA-MM-DD (0001–9999).
     * @return Cópias dos atendimentos da data; vetor vazio quando não houver nenhum.
     * @throw std::invalid_argument Se a data for inválida.
     * @throw std::bad_alloc Se não houver memória para o resultado.
     * @throw std::length_error Se o resultado exceder a capacidade máxima do vetor.
     */
    std::vector<Atendimento> listarAtendimentos(const std::string& data) const;

private:
    std::vector<Atendimento> atendimentos;

    void verificarDisponibilidade(const std::string& profissional,
                                 const std::string& data, int horaInicio,
                                 std::optional<std::size_t> indiceIgnorado = std::nullopt) const;
    std::size_t localizarAtendimento(const std::string& paciente,
                                    const std::string& data, int horaInicio) const;
};
}

#endif
