import grpc
import analytics_pb2
import analytics_pb2_grpc
import json
import random
import time
from datetime import datetime
import pandas as pd

def channel():
    # Estabelece a conexão com o servidor gRPC no endereço localhost na porta 50051
    channel = grpc.insecure_channel('localhost:50051')
    # Cria um stub para chamar métodos remotos no servidor
    stub = analytics_pb2_grpc.AnalyticsServiceStub(channel)
    return stub

def cade_analytics():
    
    stub = channel()

    event = {"timestamp": time.time()}

    # Obtém a lista de produtos a partir da função definida
    produtos = ["Laptop", "Smartphone", "Book", "Headphones", "Smartwatch"]
    # Lê os IDs dos usuários a partir de um arquivo CSV
    usuarios = pd.read_csv("mock/ContaVerde/usuarios.csv")['ID'].tolist()
    # Lista para acumular eventos antes do envio
    acumulado_eventos = []
    # Armazena o tempo do último envio para controlar o intervalo entre envios
    last_sent_time = time.time()
    
    # Loop infinito para geração e envio contínuo de eventos
    while True:
        # Verifica se o tempo desde o último envio é maior ou igual a 5 segundos
        if (time.time() - last_sent_time) >= 5.0:
            # Se houver eventos acumulados, prepara para enviar
            if acumulado_eventos:
                # Converte a lista de eventos para JSON
                json_data = json.dumps(acumulado_eventos)
                # Cria uma requisição gRPC com os dados em JSON
                event_request = analytics_pb2.EventRequest(json_data=json_data)
                try:
                    print(f"Enviando {len(acumulado_eventos)} eventos...")
                    # Tenta enviar o evento via gRPC e aguarda resposta
                    response = stub.SendEvent(event_request)
                    # Verifica se o servidor respondeu com sucesso
                    if response.success:
                        print("Dados enviados com sucesso.")
                    else:
                        print("Falha ao enviar dados.")
                except grpc.RpcError as e:
                    # Trata erros na comunicação gRPC
                    print(f"Falha ao enviar dados: {e}")
            
            # Reinicia a lista de eventos acumulados e atualiza o tempo de último envio
            acumulado_eventos = []
            last_sent_time = time.time()
        
        # Seleciona um usuário e produto aleatoriamente para criar um evento
        usuario_id = random.choice(usuarios)
        produto = random.choice(produtos)
        evento = {
            "timestamp": datetime.now().isoformat(),  # Define o timestamp do evento
            "usuario_id": usuario_id,  # ID do usuário para o evento
            "evento": random.choice(["visualizou", "adicionou ao carrinho", "comprou"]),  # Tipo do evento
            "produto": produto  # Produto envolvido no evento
        }
        
        # Adiciona o evento criado à lista de eventos acumulados
        acumulado_eventos.append(evento)
        # Pausa por um intervalo aleatório entre 1 a 2 segundos antes de criar o próximo evento
        time.sleep(random.randint(1, 2))
        print(f"Time taken: {time.time() - event['timestamp']} seconds")

# Ponto de entrada do script quando executado diretamente
if __name__ == '__main__':
    # Chama a função principal para iniciar o envio de eventos
    cade_analytics()
