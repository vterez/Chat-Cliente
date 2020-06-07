# Chat-Cliente
Juntamente com o Chat-Server, usa a biblioteca SFML para implementar um chat multiusuário.

Inicialize o servidor. No console, será informado os IP's público e privado. Na pasta do cliente, edite o arquivo com o IP adequado: se o cliente estiver na mesma rede do servidor, coloque o IP privado, caso contrário, coloque o público (lembrando de configurar o redirecionamento de porta).
Inicialize o cliente.
Será solicitado um nome de usuário.
Após inserir o nome, a tela do chat se abrirá, contendo uma conversa com o próprio usuário (para lembretes, notas, etc).
Para adicionar um usuário, basta clicar na barra branca de pesquisa e digitar #ID, sendo ID o id do usuário desejado (o id se encontra no canto superior esquerdo, embaixo do nome).
Para conversar, basta clicar na aba da conversa desejada e digitar as mensagens. É possível enviar áudios e imagens (utilizando ctrl+v).
A ordem das conversas se reorganiza conforme envios ou recebimentos de mensagens. É possível pesquisar um usuário já adicionado clicando na barra de pesquisa e digitando @Nome.
