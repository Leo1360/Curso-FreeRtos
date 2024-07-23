# Curso RTOS
Arquivos de exercício do curso de FreeRtos do Professor Fabio Souza.

## Descrição dos exercícios
1. Criação de Tasks - Duas tasks rodando ao memo tempo;
2. Deleção de Task - Uma task deletando outra;
3. Suspensão de task - Uma task suspendendo e resumindo a execução de outra;
4. Seleção de nucleo de execução - 3 tasks rodando ao mesmo tempo, em um core a task de comunicação com a serial e no outro core duas tasks de IO;
5. Alteração do exercício 4 para pegar o nome da task;
6. Verificar consumo de memoria com High Water Mark;
7. Utilizando fila para comunicação entre tarefas - Uma tarefa imprime e envia um dado pela fila, a outra recebe e imprime o valor recebido;
8. Interrupção enviado dado para a fila e liberando uma task;
9. Utilização de Semaforo binario - Uma task aguarda o loop liberar uma semaforo para sua execução;
10. Liberação de semaforo a partir de interrupção permitindo a execução de uma task;
11. Utilização de semaforo contador para tratamento de interrupção;
12. Utilização de mutex em tasks concorrentes - Duas tasks competindo por tempo no serial sendo gerenciadas utilizando um mutex;
13. Utilização de timer - Um timer piscando o led da placa e outro timer sendo chamado por botão e  apagando um led apó determinado tempo de seu inicio;
14. Utilizando EventGroups - Eventos são lançados de acordo com um timer e as task correspondentes reagem aos eventos;
15. Acordando task por meio de notificação - uma interrupção gera uma notificação para uma task ser executada;