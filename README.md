# Configurando análise

Os arquivos principais para análise são: analytics.proto, analytics_pb2_grpc.py, analytics_pb2.py, run_multiple_clients.py, client.py e server.py.

No arquivo run_multiple_clients.py escolha o número de clientes rodando na variável num_clients.
No client.py escolha por quanto tempo gostaria de gerar dados, como não quero rodar infinitamente, quero apenas uma análise, coloquei essa limitação por tempo. Na função cade_analytics você muda o tempo na variável seg.

# Rodando a análise

Estabelaça a conexão do client e do server: em client.py altere o canal com o ip da máquina que irá execusar o server (exemplo === channel = grpc.insecure_channel('123.123.0.123:50051') deixe a porta 50051), em server.py
da mesma forma  (exemplo === server.add_insecure_port('123.123.0.123:50051')).

Na máquina onde o server irá ser executado no terminal execute: python server.py
Na máquina onde o client irá ser executado no terminal execute: python run_multiple_clients.py

As análises de execusão de tempo da criação de cada evento até a análise do df gerado pelo evento irá aparecer no terminal onde o server foi executado. 

As análises de execusão de todos os eventos que foram tratados numa instância do client irá aparecer.

Cuidado, quando for refazer as análises com um novo número de clientes, termine a execusão do server anterior e execute novamente, pois assim a latencies_list não terá mais dados, atrapalhando novas análises. 
