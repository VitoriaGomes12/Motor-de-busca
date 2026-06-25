# Motor-de-busca
Projecto para o exame de EDII

# OBSERVAÇÕES DO SISTEMA

1. Relação entre localização do cliente e localização dos mercados

O sistema relaciona a localização escolhida pelo cliente com os mercados cadastrados por meio do nome do mercado. Atualmente, o nome do mercado corresponde à sua localização, por exemplo, "Kicolo" representa o mercado localizado no Kicolo.

Existe um arquivo contendo as localizações apresentadas ao cliente no menu. Essas localizações devem corresponder exatamente aos nomes dos mercados cadastrados. Dessa forma, quando o cliente selecionar uma localização e procurar mercados próximos ou determinados produtos, o sistema conseguirá realizar a associação corretamente.

Portanto, o nome do mercado deve ser idêntico à localização exibida ao cliente, caso contrário o programa não conseguirá relacionar a localização escolhida com os mercados disponíveis.

 2. Dados pré-cadastrados no sistema

O sistema já possui alguns dados definidos diretamente no código:

* Um administrador padrão, cujas credenciais foram previamente cadastradas.
* O administrador pode cadastrar novos usuários através do menu administrativo, utilizando listas ligadas.
* Existem dois mercados pré-cadastrados no sistema.
* Um dos mercados já possui uma lista de produtos carregada a partir do arquivo dataset presente na pasta do projeto.

Esses dados iniciais permitem testar as funcionalidades do sistema sem a necessidade de realizar todos os cadastros manualmente.

 3. Validações e melhorias pendentes

Algumas validações de entrada de dados ainda não foram implementadas. Atualmente, o programa não trata adequadamente situações em que o usuário insere um tipo de dado incorreto, como letras em campos que deveriam receber números inteiros ou números em campos destinados a texto.

A formatação das mensagens e dos textos apresentados ao usuário também ainda não foi concluída.

Além disso, em algumas opções dos menus, caso o usuário introduza um valor inválido, como uma letra em vez de um número correspondente ao ID de um mercado, o programa pode interpretar esse valor incorretamente e associá-lo a um ID de forma inesperada.

Dessa forma, ainda é necessário implementar mecanismos de validação de entrada, tratamento de erros e melhorias na apresentação das mensagens do sistema.

E por último, as credenciais do administrador: username "admin", senha "1234".

