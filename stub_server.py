import grpc
from concurrent import futures
import time
import pandas as pd
import json
from datetime import datetime
import multiprocessing

import analytics_pb2
import analytics_pb2_grpc

# Define uma classe que herda de analytics_pb2_grpc.AnalyticsServiceServicer
class AnalyticsServiceServicer(analytics_pb2_grpc.AnalyticsServiceServicer):
    def __init__(self):
        # Inicializa um DataFrame para armazenar eventos recebidos
        self.df_eventos = pd.DataFrame(columns=['timestamp', 'usuario_id', 'evento', 'produto'])

    def SendEvent(self, request):
        start_time = time.time()
        # Deserializa o JSON recebido em um DataFrame
        eventos = json.loads(request.json_data)
        df_novos_eventos = pd.DataFrame(eventos)

        # Converte strings de timestamp para datetime, para manipulação de data e hora
        df_novos_eventos['timestamp'] = pd.to_datetime(df_novos_eventos['timestamp'])

        # Anexa novos eventos ao DataFrame principal, para acumular histórico
        self.df_eventos = pd.concat([self.df_eventos, df_novos_eventos], ignore_index=True)

        # Define o momento atual para análises de tempo
        current_time = datetime.now()
        one_minute_ago = current_time - pd.Timedelta(minutes=1)
        one_hour_ago = current_time - pd.Timedelta(hours=1)

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
        print("Visualizações por minuto:\n", visualizados_por_minuto)
        print("Compras por minuto:\n", comprados_por_minuto)
        print("Usuários únicos por produto/minuto:\n", usuarios_unicos_por_produto)
        print("Ranking de produtos mais comprados na última hora:\n", ranking_compras.head())
        print("Ranking de produtos mais visualizados na última hora:\n", ranking_visualizados.head())

        # Retorna uma resposta indicando sucesso na operação
        time_taken = time.time() - start_time
        return analytics_pb2.EventResponse(success=True, processing_time=time_taken)

def serve():
    # Cria um servidor gRPC com um pool de threads para lidar com as solicitações
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=multiprocessing.cpu_count()))
    # Adiciona a classe do servicer ao servidor
    analytics_pb2_grpc.add_AnalyticsServiceServicer_to_server(AnalyticsServiceServicer(), server)
    # Define a porta para o servidor escutar
    server.add_insecure_port('[::]:50051')
    # Inicia o servidor
    server.start()
    # Executa o servidor por 180 segundos (3 minutos)
    try:
        time.sleep(30)  # Dorme por 180 segundos antes de parar o servidor
    finally:
        server.stop(0)
        print("Servidor parado após 3 minutos.")

if __name__ == '__main__':
    # Inicia a função serve quando o script é executado diretamente
    serve()
