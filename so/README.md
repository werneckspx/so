# Sistemas Operacionais: Simulador da Arquitetura de Von Neumann e Pipeline MIPS

<div style="display: inline_block">
  <img align="center" alt="VS" src="https://img.shields.io/badge/Visual_Studio_Code-0078D4?style=for-the-badge&logo=visual%20studio%20code&logoColor=white" />
  <img align="center" alt="Linux" src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" />
  <img align="center" alt="C++" src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
</div><br/>

# Instruções para Execução

Para o correto funcionamento do programa, é necessário incluir as instruções inicais em dois arquivos. Um arquivo nomeado de [`Instructions.txt`](https://github.com/peudias/so/blob/main/data/instructions.txt) e outro nomeado de [`setRegisters.txt`](https://github.com/peudias/so/blob/main/data/setRegisters.txt). Ambos dentro da pasta [`data`](https://github.com/peudias/so/tree/main/data), devendo seguir o seguinte formato:

### [Instructions.txt](https://github.com/peudias/so/blob/main/data/instructions.txt)

Este arquivo contém as instruções a serem simuladas, com cada linha no seguinte formato: `OPERAÇÃO`, `REGISTRADOR_DESTINO`, `REGISTRADOR_1`, `REGISTRADOR_2`. O registrador de destino armazena o resultado da operação realizada com os valores dos dois registradores especificados. Exemplo:

```
ADD, 1, 2, 3
SUB, 4, 1, 3
STORE, 3, 2, 0
LOAD, 0, 2, 0
ENQ, 7, 3, 4
IF_igual, 8, 4, 7
```

### [setRegisters.txts](https://github.com/peudias/so/blob/main/data/setRegisters.txt)

Especifica os valores iniciais dos registradores no formato: `REGISTRADOR`, `VALOR`. Exemplo:

```
2,10
3,5
4,7
```

# Compilação e Execução

Esse pequeno exemplo possui um arquivo [`makefile`](https://github.com/peudias/so/blob/main/makefile) que realiza todo o procedimento de compilação e execução. Para tanto, temos as seguintes diretrizes de execução:

| Comando      | Função                                                                                  |
| ------------ | --------------------------------------------------------------------------------------- |
| `make clean` | Apaga a última compilação realizada contida na pasta build                              |
| `make`       | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build |
| `make run`   | Executa o programa da pasta build após a realização da compilação                       |
| `make c`     | make clean + make + make run (recomendado)                                              |
| `make run`   | make + make run                                                                         |

## Contatos

| Participante     | Contato                                                                                                                                                                                    |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Celso Vinícius   | <a href="https://github.com/celzin"><img align="center" height="20px" width="90px" src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white"/>      |
| Felipe Parreiras | <a href="https://github.com/Parreirass"><img align="center" height="20px" width="90px" src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white"/>  |
| Felipe Werneck   | <a href="https://github.com/werneckspx"><img align="center" height="20px" width="90px" src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white"/>  |
| José Marconi     | <a href="https://github.com/josemarconi"><img align="center" height="20px" width="90px" src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white"/> |
| Lucas Fernandes  | <a href="https://github.com/LucasMagras"><img align="center" height="20px" width="90px" src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white"/> |
| Pedro Dias       | <a href="https://github.com/peudias"><img align="center" height="20px" width="90px" src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white"/>     |
