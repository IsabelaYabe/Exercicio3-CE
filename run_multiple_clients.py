from multiprocessing import Process
from stub_client import cade_analytics  # Importando a função do outro arquivo

def run_client():
    cade_analytics()  # Chama a função do cliente

def main():
    num_clients = 20  # Número de instâncias do cliente que você deseja executar

    # Criar e iniciar processos
    processes = []
    for _ in range(num_clients):
        process = Process(target=run_client)
        process.start()
        processes.append(process)
        print(f"Processo de numero {_} iniciado")
    
    # Esperar todos os processos terminarem
    for process in processes:
        process.join()

if __name__ == '__main__':
    main()