# Relatório — AgendaFácil

## 1. Diagnóstico do código-base

Este diagnóstico foi elaborado a partir do código-base, antes da implementação da versão refatorada. As cinco categorias do checklist estão contempladas.

| Categoria | Anomalia e localização | Por que é um problema | Encaminhamento |
| --- | --- | --- | --- |
| Adequação e lógica | `add` verifica apenas `dur > 0`. | Aceita durações fora de 15–120 minutos ou sem múltiplos de 15. | Validar a faixa e o múltiplo. |
| Adequação e lógica | `add` verifica o início, mas não o fim. | Um atendimento pode ultrapassar as 18h. | Calcular o fim em minutos antes de inserir. |
| Adequação e lógica | `rem` não valida o novo horário. | Permite remarcar para fora do expediente. | Aplicar as mesmas regras do agendamento. |
| Adequação e lógica | `rem` compara o atendimento com ele mesmo. | Remarcar para a mesma data e hora acusa conflito. | Excluir o próprio registro da busca de conflitos. |
| Adequação e lógica | `rem` continua procurando após encontrar um paciente. | Pode alterar vários registros se a chave paciente/data/hora for ambígua. | Rejeitar ambiguidades antes de alterar o estado. |
| Manutenibilidade | `A`, `p`, `pr`, `d`, `h`, `dur`, `st`, `ag`, `add`, `rem` e `lst`. | Os nomes escondem o significado dos dados e das operações. | Usar tipos e identificadores descritivos. |
| Manutenibilidade | Horários, duração e estado aparecem como literais. | As regras ficam espalhadas e difíceis de modificar. | Centralizar constantes de negócio. |
| Manutenibilidade | A verificação de conflito é repetida em `add` e `rem`. | Uma correção pode ser aplicada em apenas um dos lugares. | Manter uma única função de conflito. |
| Manutenibilidade | Cabeçalho manual de autores, TODO vago e código de cancelamento comentado. | Acumulam informação sem manutenção e funcionalidades inexistentes. | Remover; usar o histórico do Git quando o projeto for versionado. |
| Organização e estética | Validação, alteração do vetor e impressão em `add` e `rem`. | Mistura responsabilidades e dificulta testes sem terminal. | Separar validações, operações e apresentação. |
| Organização e estética | Muitos `if` aninhados em `add`. | O fluxo normal fica escondido e os erros são difíceis de localizar. | Usar cláusulas de guarda. |
| Organização e estética | Comentário de `lst` diz “atendimentos do mês”. | A função filtra uma data exata, não um mês. | Documentar corretamente a listagem por data. |
| Programação defensiva e segurança | `catch (...)` vazio em `add`. | Esconde exceções e impede o chamador de saber o que falhou. | Remover e propagar erros; capturar somente conflitos esperados na demonstração. |
| Programação defensiva e segurança | Horário inválido em `add` e registro inexistente em `rem`. | Operações falham em silêncio. | Lançar exceções com os valores recebidos. |
| Programação defensiva e segurança | Datas são aceitas sem validação em todas as operações. | Textos arbitrários podem virar datas e filtros inconsistentes. | Exigir formato e data de calendário válidos. |
| Programação defensiva e segurança | Paciente e profissional não são validados. | Permite registros sem identificação. | Rejeitar textos vazios ou compostos só por espaços. |
| Desempenho e recursos | Strings são recebidas por valor. | Provoca cópias desnecessárias nas chamadas. | Receber textos por referência constante. |
| Desempenho e recursos | Vetor global `ag`. | Expõe o estado, acopla operações e impede agendas independentes. | Encapsular o vetor em uma instância de `Agenda`. |
| Desempenho e recursos | Laços usam `int` contra `vector::size()`. | Mistura tipos com e sem sinal e limita a representação dos índices. | Usar `std::size_t` e laços por intervalo. |
| Desempenho e recursos | Busca de conflito continua mesmo depois de localizar uma colisão. | Faz trabalho desnecessário e prejudica a clareza do resultado. | Retornar assim que encontrar o primeiro conflito. |

## 2. Organização e responsabilidades

O tipo `Atendimento` possui campos descritivos, e `Agenda` mantém o vetor privado. As validações ficam em um módulo próprio, enquanto `Agenda` coordena as operações e mantém suas invariantes. Apenas `main.cpp` imprime a demonstração; nenhuma operação de domínio escreve no terminal. Os nomes seguem camelCase para funções e variáveis, PascalCase para tipos e MAIUSCULAS_COM_UNDERSCORE para constantes. Os textos são recebidos por referência constante.

## 3. Validação e tratamento de erros

Todas as entradas de cada operação pública são verificadas antes da alteração do estado. Durações devem estar entre 15 e 120 minutos, em múltiplos de 15, com início entre 8h e 17h e término até 18h. Entradas inválidas geram `std::invalid_argument`; atendimento inexistente ou identificação ambígua gera `std::runtime_error`; conflito gera `HorarioOcupado`, derivada de `std::runtime_error`. Não há captura genérica nem vazia. Erros de alocação também se propagam. Na demonstração, somente o conflito esperado de Bruno é capturado e exibido.

## 4. Datas e identificação

Além do formato exato AAAA-MM-DD, a implementação verifica o calendário gregoriano, incluindo anos bissextos, e aceita anos de 0001 a 9999. Essa decisão evita aceitar textos como `2026-02-30`. Nomes vazios ou só com espaços são inválidos. A remarcação conserva a identificação por paciente/data/hora do código-base; se essa combinação identificar mais de um registro, a operação falha explicitamente. Não foram adicionados IDs nem novos parâmetros à demonstração.

## 5. Conflitos e preservação do comportamento

A regra expressa do enunciado considera conflito o mesmo profissional, na mesma data e hora de início; a implementação preserva esse critério em uma única função. Ela não amplia a regra para qualquer sobreposição de intervalos. Profissionais diferentes podem atender no mesmo horário. Na remarcação, o próprio registro é ignorado na busca de conflito, permitindo solicitar a mesma data/hora sem erro. A listagem mantém a ordem de inserção e retorna cópias para impedir alterações externas no estado da agenda.

## 6. Consistência da remarcação

A remarcação localiza um único registro, verifica os novos dados e o conflito, e prepara um atendimento substituto antes de modificar o vetor. Assim, uma falha de validação, conflito ou alocação durante a preparação preserva os dados anteriores. A substituição final usa movimentação sem lançamento de exceção, verificada em compilação.

## 7. Documentação e comentários

As funções públicas têm contratos Doxygen com propósito, parâmetros, retorno e exceções. Funções sem parâmetros usam uma seção explícita “Parâmetros: nenhum”, sem inventar um parâmetro para `@param`. Foram removidos o histórico manual, os comentários de ruído, o código morto e o TODO vago. Comentários de decisão explicam a comparação por hora de início e a preparação da remarcação. `Doxyfile` e README permitem gerar a API em `docs/html/` sem bibliotecas externas no programa.

## 8. Mudanças intencionais para entradas inválidas

| Situação | Antes | Depois |
| --- | --- | --- |
| Agendar às 20h | Não faz nada. | Exceção com o horário recebido. |
| Duração de 5, 25 ou 150 minutos | Aceita qualquer valor positivo. | Exceção com a duração e regra esperada. |
| Início às 17h com duração de 120 minutos | Aceita e ultrapassa o expediente. | Exceção indicando início, duração e limite de término. |
| Data inválida ou impossível | Aceita o texto. | Exceção com a data recebida. |
| Paciente ou profissional sem nome | Aceita. | Exceção identificando o campo e o texto recebido. |
| Remarcar atendimento inexistente | Não informa nada. | Exceção com paciente, data e hora. |
| Remarcar para o próprio horário | Acusa conflito com ele mesmo. | Mantém o agendamento, sem erro. |
| Chave de remarcação ambígua | Pode alterar mais de um registro. | Exceção antes de alterar qualquer registro. |
| Conflito | Imprime diretamente. | Lança `HorarioOcupado`; a apresentação decide como mostrar. |

## 9. Validação realizada

O projeto compilou com C++17, `-Wall -Wextra -Werror -pedantic`, sem avisos. Foram aprovadas 104 verificações automatizadas em `tests/testes.cpp` para a demonstração, limites de expediente e duração, datas, conflitos, remarcações, listagem e preservação do estado após falhas. A suíte também foi executada com AddressSanitizer e UndefinedBehaviorSanitizer, sem erros detectados; a detecção de vazamentos foi desativada por uma restrição do ambiente de execução. As instruções completas para executar os testes estão no README. A saída da demonstração foi comparada integralmente com o texto esperado:

```text
Atendimento marcado para Ana
Horario ocupado
Atendimento marcado para Carla
Remarcado
14 Carla Dra. Souza MARCADO
```

## 10. Dívidas técnicas assumidas

A busca permanece linear, adequada ao escopo de uma clínica pequena, e a listagem copia seus resultados para preservar o encapsulamento. Não há persistência, cancelamento, interface interativa ou identificador único, pois seriam funcionalidades adicionais. Sobreposição de intervalos com inícios distintos, conflitos do próprio paciente e nomes que diferem apenas por acentuação, caixa ou espaços não são tratados como equivalentes: precisam de regras futuras. Horários continuam representados por horas inteiras, como no programa original. A aplicação é de uma única thread e não possui controle de acesso, fora do escopo proposto.

## 11. Autoria e uso de IA

Responsável pela entrega: Mayan Victor. Matrícula: 20250055372.

Foi utilizada a ferramenta ChatGPT (OpenAI) para apoiar o diagnóstico, propor a organização, gerar a implementação e a documentação e executar verificações. O estudante deve revisar o material e conseguir explicar cada trecho entregue, conforme a regra de integridade do enunciado. A declaração registra a assistência recebida, sem atribuir ao estudante uma implementação independente que não ocorreu.
