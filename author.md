# Descrição
Essa é uma implementação de uma Estrutura de Dados Abstrata, mais especificamente uma Lista Dinamica (chamada aqui de vector).

# Identificação do grupo

Preencha os dados abaixo para identificar a autoria do trabalho.

- Nome: *Ianco Soares Oliveira*
- Email: *ianco.soares.112@ufrn.edu.br*
- Turma: *01*

- Nome: *Lucas Vinícius Góis Nogueira*
- Email: *lucas.nogueira.072@ufrn.edu.br*
- Turma: *01*

# Como rodar

Certifique-se de ter o `cmake` instaldo em sua máquina e/ou o `g++`

## Usando `cmake`

Usando o Linux ou o Windows com o Power Shell, a partir da página principal do repositório local (onde está localizado este arquivo) faça:

```
cmake -S source -B build
cmake --build build
```
Logo após a compilação, para executar os testes gerados use os comandos:

```
cd build
make run_tests
```

## Usando compilação manual com `g++`

É possível compilar o programa usando apenas o `g++`. Para isso use os comandos abaixo a partir da pasta principal do repositório local no Linux ou no Windows com o Power Shell.

```
mkdir -p build
g++ -Wall -std=c++11 -I source/include -I source/tests/include/tm source/tests/main.cpp source/tests/include/tm/test_manager.cpp -o build/run_tests
```

Após a compilação, para executar os testes gerados, use os comandos:

No Linux:
```
./build/run_tests
```

No Windows com Power Shell:
```
./build/run_tests.exe
```
# Limitações encontradas

Como todos os testes foram feitos com inteiros e/ou com caracteres é possível que o vector quebre com outros tipos de dados.

# Indique quais métodos das classes abaixo foram implementados

## Implementação do Vector

**Special members (21 credits)**
- [x] Regular constructor (3 credits)
- [x] Destructor (3 credits)
- [x] Copy constructor (3 credits)
- [x] Constructor from range (3 credits)
- [x] Constructor from initialize list (3 credits)
- [x] Assignment operator (x 2) (6 credits)

**Iterator methods (4 credits)**
- [x] `begin()` (1 credits)
- [x] `end()` (1 credits)
- [x] `cbegin()` (1 credits)
- [x] `cend()` (1 credits)

**Capacity methods (3 credits)**
- [x] `empty()` (1 credits);
- [x] `size()` (1 credits);
- [x] `capacity()` (1 credits);

**Modifiers methods (52 credits)**
- [x] `clear()` (1 credits)
- [x] `push_back()` (3 credits)
- [x] `pop_back()` (3 credits)
- [x] `insert()` \times 6 (18 credits)
- [x] `reserve()` (3 credits)
- [x] `shrink_to_fit()` (3 credits)
- [x] `assign()` \times 3 (9 credits)
- [x] `erase()` \times 4 (12 credits)

**Element access methods (10 credits)**
- [x] `front()` (1 credits)
- [x] `back()` (1 credits)
- [x] `operator[]()` \times 2 (4 credits)
- [x] `at()` \times 2 (4 credits)

**Operators (2 credits)**
- [x] `operator==()` (1 credits)
- [x] `operator!=()` (1 credits)

## Implementação do Iterator

**Special members (3 credits)**
- [x] Regular constructor (1 credits)
- [x] Copy constructor (1 credits)
- [x] Assignment operator (1 credits)

**Navigation methods (17 credits)**
- [x] increment operator `++it` and `it++` (2 credits)
- [x] decrement operator `--it` and `it--` (2 credits)
- [x] forward jump operator `it + n` or `n + it`   (2 credits)
- [x] backward jump operator `it - n`  (1 credits)
- [x] forward self jump operator `it += n`  (2 credits);
- [x] backward self jump operator `it -= n`   (2 credits);
- [x] difference between iterators `it1-it2` (1 credits)
- [x] dereference operator `*it` (1 credits)
- [x] equality/difference operators `it1==it2` and `it1!=it2` (2 credits)
- [x] greater/less than operators `it1<it2` and `it1>it2` (2 credits);

--------
&copy; DIMAp/UFRN 2021.