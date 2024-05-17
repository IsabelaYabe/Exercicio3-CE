import grpc
import analytics_pb2
import analytics_pb2_grpc
import json
import random
import time
from datetime import datetime
import pandas as pd
from multiprocessing import Process

def channel():
    # Estabelece a conexão com o servidor gRPC no endereço localhost na porta 50051
    channel = grpc.insecure_channel('localhost:50051')
    # Cria um stub para chamar métodos remotos no servidor
    stub = analytics_pb2_grpc.AnalyticsServiceStub(channel)
    return stub

def cade_analytics():
    # Cria um stub para chamar métodos remotos no servidor
    stub = channel()
    produtos = ["Laptop", "Smartphone", "Book", "Headphones", "Smartwatch"]
    usuarios = pd.read_csv("mock/ContaVerde/usuarios.csv")['ID'].tolist()
    acumulado_eventos = []

    # Armazena o tempo do último envio para controlar o intervalo entre envios
    last_sent_time = time.time()
    
    # Loop infinito para geração e envio contínuo de eventos
    while True:
        if (time.time() - last_sent_time) >= 5.0:
            
            if acumulado_eventos:
                # Converte a lista de eventos para JSON
                json_data = json.dumps(acumulado_eventos)
                # Cria uma requisição gRPC com os dados em JSON
                event_request = analytics_pb2.EventRequest(json_data=json_data)
                try:
                    print(f"Enviando {len(acumulado_eventos)} eventos...")
                    response = stub.SendEvent(event_request)
                    if response.success:
                        print("Dados enviados com sucesso.")
                    else:
                        print("Falha ao enviar dados.")
                except grpc.RpcError as e:
                    print(f"Falha ao enviar dados: {e}")
            
            # Reinicia a lista de eventos acumulados e atualiza o tempo de último envio
            acumulado_eventos = []
            last_sent_time = time.time()
        
        # Cria novo evento com dados aleatórios
        usuario_id = random.choice(usuarios)
        produto = random.choice(produtos)
        evento = {
            "timestamp": datetime.now().isoformat(),
            "usuario_id": usuario_id,  
            "evento": random.choice(["visualizou", "adicionou ao carrinho", "comprou"]),  
            "produto": produto  
        }
        acumulado_eventos.append(evento)
        time.sleep(random.randint(1, 2))

if __name__ == '__main__':
    cade_analytics()