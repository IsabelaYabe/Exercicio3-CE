import grpc
import analytics_pb2
import analytics_pb2_grpc
import json
import random
import time
from datetime import datetime
import pandas as pd

def cade_analytics():
    # Estabelece a conexão com o servidor gRPC no endereço localhost na porta 50051
    channel = grpc.insecure_channel('localhost:50051')
    # Cria um stub para chamar métodos remotos no servidor
    stub = analytics_pb2_grpc.AnalyticsServiceStub(channel)

    # Obtém a lista de produtos a partir da função definida
    produtos = ["Laptop", "Smartphone", "Book", "Headphones", "Smartwatch"]
    # Lê os IDs dos usuários a partir de um arquivo CSV
    usuarios = pd.read_csv("mock/ContaVerde/usuarios.csv")['ID'].tolist()
    # Lista para acumular eventos antes do envio
    acumulado_eventos = []
    # Armazena o tempo do último envio para controlar o intervalo entre envios
    last_sent_time = time.time()

    start_time = time.time()  # Marca o tempo de início

    # Loop que continua até que 2 minutos tenham passado
    while time.time() - start_time < 120:  # 120 segundos = 2 minutos
        # Verifica se o tempo desde o último envio é maior ou igual a 5 segundos
        if (time.time() - last_sent_time) >= 5.0:
            if acumulado_eventos:
                json_data = json.dumps(acumulado_eventos)
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
            
            acumulado_eventos = []
            last_sent_time = time.time()
        
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
