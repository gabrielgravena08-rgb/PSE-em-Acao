# PSE em Ação

## Sistema de Planejamento e Acompanhamento de Ações de Saúde Mental no Programa Saúde na Escola

Projeto acadêmico desenvolvido para a Atividade Extensionista de Prática (AEP) do segundo semestre de 2026 da UniCesumar, nos cursos de Análise e Desenvolvimento de Sistemas e Engenharia de Software.

### Objetivo

Desenvolver uma aplicação em linguagem C para auxiliar equipes escolares e profissionais de saúde no planejamento, registro e acompanhamento de ações coletivas de promoção da saúde mental no âmbito do Programa Saúde na Escola (PSE).

### Funcionalidades

- Cadastro de ações com informações sobre escola, tema, data, público-alvo, responsável e participantes previstos.
- Listagem das ações cadastradas.
- Pesquisa de ações por código, escola ou tema.
- Atualização da situação para planejada, realizada ou cancelada.
- Geração de resumo estatístico das ações e participantes.
- Validação de dados, evitando códigos duplicados, valores inválidos e campos obrigatórios vazios.

### Tecnologias utilizadas

- Linguagem C
- Estruturas condicionais e de repetição
- Funções
- Vetores e structs
- Interface em terminal

### Como compilar e executar

É necessário possuir um compilador C, como GCC.

No terminal, dentro da pasta do projeto, execute:

```bash
gcc main.c -o pse_em_acao
```

Em seguida, no macOS ou Linux:

```bash
./pse_em_acao
```

No Windows, execute o programa compilado conforme o ambiente utilizado.

### Limitações

O sistema armazena os registros temporariamente em memória, durante sua execução. Não utiliza banco de dados nem armazena informações clínicas individuais dos estudantes.

### Integrantes

- Adriano Arevalo
- Gabriel Gravena
- Jhonatan Alves Fagunde

**Instituição:** UniCesumar  
**Ano:** 2026
