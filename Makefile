############
# Makefile personalizado para organizacao em diretorios
# Paula e Emilly
############

# nome do compilador
CPP = g++

# opções de compilação
CFLAGS = -Wall -g

# opções do pré-processador: padrão C++17 e diretório de includes
CPPFLAGS = -std=c++17 -Iinclude

# nome do arquivo executável
EXECUTAVEL = vereadores

# define lista de arquivos-fonte, buscando no diretório 'src'
FONTES = $(wildcard src/*.cpp) 

# define lista dos arquivos-objeto. Eles serão criados no mesmo diretório dos fontes (src/)
OBJETOS = $(FONTES:.cpp=.o)

############ alvos
#
# use @ antes de todos os comandos, pois é necessário no script de teste
#

# alvo principal é o executável  
all: $(EXECUTAVEL)

# arquivos-objetos
# os caminhos para os .o (ex: src/arquivo.o)
$(EXECUTAVEL): $(OBJETOS)
	@echo "Linkando o executável..."
	@$(CPP) -o $@ $^ 

# alvo para cada arquivo-objeto depende do código fonte
src/%.o: src/%.cpp
	@echo "Compilando $<..."
	@$(CPP) $(CPPFLAGS) -c $(CFLAGS) $< -o $@

# comandos para execução    
run: $(EXECUTAVEL)
	@./$(EXECUTAVEL) $(CDMUN) candidatos.csv votacao.csv 06/10/2024

# comando para limpeza
clean: 
	@echo "Limpando arquivos gerados..."
	@rm -f src/*.o $(EXECUTAVEL)