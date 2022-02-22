# PSPD: Lab02 – Construindo aplicações distribuídas usando sockets TCP/UDP

## Descrição

Experimento realizado na disciplina de PSPD com o objetivo de compreender as características inerentes à construção de aplicações distribuídas, incluindo passagem de parâmetros, envolvendo módulos cliente e servidor usando sockets TCP/UDP.

## Solução adotada

Optamos por utilizar o TCP devido à maior quantidade de bytes disponíveis para a troca de mensagens. Dessa forma, foi possível enviar os dados do vetor com 10000 posições sem perda de performance e sem aumento na complexidade da solução utilizada.

## Um cliente e um servidor

### Requisitos e instruções para execução

É necessário possuir _makefile_ instalado em sua máquina.

Para executar deve-se:

`cd 1worker`
Para acessar o diretório da versão em questão.

`make all`
Para compilar ambos os arquivos _.c_.

`make runS`
Para habilitar o servidor TCP.

`make runC`
Para realizar a requisição via cliente.

IMPORTANTE:

- Servidor deve ser habilitado primeiro.
- Cliente deve ser executado em outro terminal.
- Caso venha a receber a mensagem: `Ligação Falhou!`, deve-se alterar as portas utilizadas no Makefile.
