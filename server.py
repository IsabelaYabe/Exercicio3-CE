import sys
sys.path.append("grpcio-1.63.0") 
import grpc
from concurrent import futures
import time
import pandas as pd
import json
from datetime import datetime
import multiprocessing
import threading
from queue import Queue

import analytics_pb2
import analytics_pb2_grpc

class AnalyticsServiceServicer(analytics_pb2_grpc.AnalyticsServiceServicer):
    def __init__(self):
        self.df_eventos = pd.DataFrame(columns=['timestamp', 'usuario_id', 'evento', 'produto'])
        self.event_queue = Queue()  
        self.lock = threading.Lock()  
        self.num_processing_threads = 4 
        self.threads = []
        for _ in range(self.num_processing_threads):
            t = threading.Thread(target=self.process_events)
            t.start()
            self.threads.append(t)

    def SendEvent(self, request, context):
        print("Recebendo eventos...")
        # Enfileira o evento recebido para ser processado posteriormente
        self.event_queue.put(request.json_data)
        return analytics_pb2.EventResponse(success=True)

    def process_events(self):
        while True:
            json_data = self.event_queue.get()
            if json_data is None:
                break
            
            eventos = json.loads(json_data)
            df_novos_eventos = pd.DataFrame(eventos)
            df_novos_eventos['timestamp'] = pd.to_datetime(df_novos_eventos['timestamp'])
    
            # Atualiza o DataFrame principal dentro de um lock
            with self.lock:
                self.df_eventos = pd.concat([self.df_eventos, df_novos_eventos], ignore_index=True)
                df_copy = self.df_eventos.copy()
    
            # Analisa os dados fora do lock, uma vez que df_copy é agora um objeto separado e seguro
            self.perform_analysis(df_copy)

    def perform_analysis(self, df):

        current_time = datetime.now()
        one_minute_ago = current_time - pd.Timedelta(minutes=1)
        df_last_minute = df[(df['timestamp'] >= one_minute_ago) & (df['timestamp'] <= current_time)]

        # Filtra eventos ocorridos no último minuto para análises em tempo real
        visualizados_por_minuto = df_last_minute[df_last_minute['evento'] == 'visualizou'].groupby('produto').count()['evento']
        comprados_por_minuto = df_last_minute[df_last_minute['evento'] == 'comprou'].groupby('produto').count()['evento']
        usuarios_unicos_por_produto = df_last_minute[df_last_minute['evento'] == 'visualizou'].groupby('produto')['usuario_id'].nunique()

        # Imprime resultados das análises para verificar a eficácia do serviço
        print("="*30)
        print("Visualizações por minuto:\n", visualizados_por_minuto)
        print("Compras por minuto:\n", comprados_por_minuto)
        print("Usuários únicos por produto/minuto:\n", usuarios_unicos_por_produto)

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=multiprocessing.cpu_count()))
    servicer = AnalyticsServiceServicer()
    analytics_pb2_grpc.add_AnalyticsServiceServicer_to_server(servicer, server)
    server.add_insecure_port('[::]:50051')
    server.start()
    try:
        while True:
            time.sleep(86400)  # Mantém o servidor rodando indefinidamente
    except KeyboardInterrupt:
        for _ in range(servicer.num_processing_threads):
            servicer.event_queue.put(None)  # Encerra cada thread de processamento
        for t in servicer.threads:
            t.join()
        server.stop(0)

if __name__ == '__main__':
    serve()
