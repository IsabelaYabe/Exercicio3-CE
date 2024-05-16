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
        self.processing_thread = threading.Thread(target=self.process_events)
        self.processing_thread.start()
        self.latencies = []

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
            
            # Usa o lock durante a atualização do DataFrame
            with self.lock:
                self.df_eventos = pd.concat([self.df_eventos, df_novos_eventos], ignore_index=True)

            # Realiza análises baseadas nos dados atualizados
            self.perform_analysis()

    def perform_analysis(self):

        # Garante que a leitura do DataFrame seja thread-safe
        with self.lock:
            current_time = datetime.now()
            one_minute_ago = current_time - pd.Timedelta(minutes=1)
            one_hour_ago = current_time - pd.Timedelta(hours=1)

            analysis_time = time.time()  # Momento da análise

            df_last_minute = self.df_eventos[(self.df_eventos['timestamp'] >= one_minute_ago) & (self.df_eventos['timestamp'] <= current_time)]
            df_last_hour = self.df_eventos[(self.df_eventos['timestamp'] >= one_hour_ago) & (self.df_eventos['timestamp'] <= current_time)]

        # Filtra eventos ocorridos no último minuto para análises em tempo real
        df_last_minute = self.df_eventos[(self.df_eventos['timestamp'] >= one_minute_ago) & (self.df_eventos['timestamp'] <= current_time)]
        visualizados_por_minuto = df_last_minute[df_last_minute['evento'] == 'visualizou'].groupby('produto').count()['evento']
        comprados_por_minuto = df_last_minute[df_last_minute['evento'] == 'comprou'].groupby('produto').count()['evento']
        usuarios_unicos_por_produto = df_last_minute[df_last_minute['evento'] == 'visualizou'].groupby('produto')['usuario_id'].nunique()

        # Filtra eventos da última hora para criar rankings de produtos
        df_last_hour = self.df_eventos[(self.df_eventos['timestamp'] >= one_hour_ago) & (self.df_eventos['timestamp'] <= current_time)]
        ranking_compras = df_last_hour[df_last_hour['evento'] == 'comprou'].groupby('produto').count()['evento'].sort_values(ascending=False)
        ranking_visualizados = df_last_hour[df_last_hour['evento'] == 'visualizou'].groupby('produto').count()['evento'].sort_values(ascending=False)

        # Imprime resultados das análises para verificar a eficácia do serviço
        print("="*30)
        print("Visualizações por minuto:\n", visualizados_por_minuto)
        print("Compras por minuto:\n", comprados_por_minuto)
        print("Usuários únicos por produto/minuto:\n", usuarios_unicos_por_produto)
        print("Ranking de produtos mais comprados na última hora:\n", ranking_compras.head())
        print("Ranking de produtos mais visualizados na última hora:\n", ranking_visualizados.head())
            # Calcular a latência para cada evento analisado
        for index, row in df_last_minute.iterrows():
            event_latency = analysis_time - row['created_time']
            self.latencies.append(event_latency)
    
        # Periodicamente (ou no final), calcular e imprimir o tempo médio de latência
        if self.latencies:
            average_latency = sum(self.latencies) / len(self.latencies)
            print("Tempo médio de latência: {:.2f} segundos".format(average_latency))

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
        servicer.event_queue.put(None) 
        servicer.processing_thread.join()
        server.stop(0)
        print("Servidor interrompido.")

if __name__ == '__main__':
    serve()
