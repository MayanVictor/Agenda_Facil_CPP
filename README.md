# Agenda_Facil_CPP
trabalho da disciplina de Boas Praticas de Programação na UFRN com professor Marcus Vinicius

# AgendaFácil — Unidade 1

Agenda de atendimentos em memória, refatorada em C++17 para consolidar organização, documentação e programação defensiva. Permite marcar, remarcar e listar atendimentos por data. Mantém a demonstração do enunciado e utiliza somente a biblioteca padrão de C++.

**Responsável:** Mayan Victor. 20250055372. Assistência de IA declarada no `RELATORIO.md`.

## Compilar e executar

Requisito: compilador com suporte a C++17, por exemplo GCC/g++. Abra o terminal na pasta `AgendaFacil`.

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/Agenda.cpp src/Validacao.cpp src/main.cpp -o agenda
./agenda
```

No Windows, com g++ disponível no PATH, use:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/Agenda.cpp src/Validacao.cpp src/main.cpp -o agenda.exe
.\agenda.exe
```

Saída esperada:

```text
Atendimento marcado para Ana
Horario ocupado
Atendimento marcado para Carla
Remarcado
14 Carla Dra. Souza MARCADO
```

## Executar os testes

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/Agenda.cpp src/Validacao.cpp tests/testes.cpp -o testes
./testes
```

No PowerShell, use `-o testes.exe` e execute `.\testes.exe`. Não compile `src/main.cpp` junto com os testes, pois cada programa tem seu próprio `main`.

## Gerar a documentação de API

Instale o **Doxygen** e deixe o comando `doxygen` disponível no PATH. Exemplos: Ubuntu/Debian, `sudo apt install doxygen`; Windows, instalador oficial de [doxygen.nl](https://www.doxygen.nl/download.html), com a pasta do executável no PATH. Essa ferramenta é necessária apenas para gerar a documentação; o programa não depende dela. Graphviz não é necessário.

Na raiz do projeto, execute:

```bash
doxygen Doxyfile
```

Abra `docs/html/index.html` no navegador. O arquivo `Doxyfile` já define as entradas, os comentários de API e a geração de HTML. A pasta `docs/` contém também estas instruções; o comando acima gera a documentação exigida pelo enunciado.

## Estrutura

| Caminho | Responsabilidade |
| --- | --- |
| `include/Atendimento.h` | Tipo com campos descritivos. |
| `include/Constantes.h` | Limites e estado de negócio. |
| `include/Agenda.h` | Contratos públicos e exceção de conflito. |
| `src/Agenda.cpp` | Operações, estado privado e busca de conflito. |
| `src/Validacao.h` e `src/Validacao.cpp` | Validações internas sem impressão. |
| `src/main.cpp` | Demonstração e apresentação da saída. |
| `tests/testes.cpp` | Testes automatizados com a biblioteca padrão. |
| `RELATORIO.md` | Diagnóstico, decisões, mudanças e dívidas técnicas. |
| `Doxyfile` | Configuração para gerar a documentação. |

## Regras e escopo

Início em horas inteiras de 8h a 17h, término até 18h e duração de 15 a 120 minutos em múltiplos de 15. Datas no formato AAAA-MM-DD, com calendário válido. Conflito significa mesmo profissional, data e hora de início, conforme o enunciado. A API lança exceções para operações inválidas e não imprime mensagens.

Leia o relatório antes da entrega, complete a matrícula e revise os trechos para conseguir explicar a implementação. A entrega pode ser feita por este diretório compactado ou por um repositório Git acessível ao professor.
