import os
import pandas as pd
import random
import multiprocessing
import time
import json
import requests
from datetime import datetime
from faker import Faker

fake = Faker()

def criar_diretorio(diretorio):
    if not os.path.exists(diretorio):
        os.makedirs(diretorio)

def conta_verde():
    directory = "ContaVerde"
    criar_diretorio(f"mock/{directory}")

    usuarios = [fake.uuid4() for _ in range(10)]
    produtos = [fake.uuid4() for _ in range(5)]

    usuarios_data = [(u, fake.first_name(), fake.last_name(), fake.address().replace('\n', ', '), fake.date_this_year(), fake.date_of_birth()) for u in usuarios]
    produtos_data = [(p, fake.word(), fake.image_url(), fake.sentence(), fake.random_number(digits=3)) for p in produtos]

    pd.DataFrame(usuarios_data, columns=["ID", "Nome", "Sobrenome", "Endereço", "Data de Cadastro", "Data de Aniversário"]).to_csv(f"mock/{directory}/usuarios.csv", index=False)
    pd.DataFrame(produtos_data, columns=["ID", "Nome", "Imagem", "Descrição", "Preço"]).to_csv(f"mock/{directory}/produtos.csv", index=False)

    estoque_global = {p: 100 for p in produtos}

    while True:
        produto_id = random.choice(produtos)
        user_id = random.choice(usuarios)
        quantidade = random.randint(1, 10)
        estoque_global[produto_id] -= quantidade
        
        ordem = pd.DataFrame([[user_id, produto_id, quantidade, datetime.now(), datetime.now(), datetime.now()]],
                             columns=["ID do Usuário", "ID do Produto", "Quantidade", "Data de Criação", "Data do Pagamento", "Data da Entrega"])
        ordem.to_csv(f"mock/{directory}/ordens.csv", mode='a', header=not os.path.exists(f"mock/{directory}/ordens.csv"), index=False)
        
        estoque = pd.DataFrame([[produto_id, max(estoque_global[produto_id], 0)]],
                               columns=["ID do Produto", "Quantidade Disponível"])
        estoque.to_csv(f"mock/{directory}/estoque.csv", mode='a', header=not os.path.exists(f"mock/{directory}/estoque.csv"), index=False)

        time.sleep(random.randint(1,3))

def get_products_list():
    directory = "mock/ContaVerde"
    try:
        produtos_df = pd.read_csv(f"{directory}/produtos.csv")
        return produtos_df['Nome'].tolist()
    except FileNotFoundError:
        print("Produto CSV não encontrado. Certifique-se de que 'conta_verde' gerou dados antes de executar 'data_cat'.")
        return ["Laptop", "Smartphone", "Book", "Headphones", "Smartwatch"]  # default list

def data_cat():
    directory = "DataCat"
    criar_diretorio(f"mock/{directory}")

    log_types = ["auditoria", "comportamento do usuário", "notificação de falhas", "depuração"]
    column_headers = {
        "auditoria": "Timestamp | Type | Message",
        "comportamento do usuário": "Timestamp | Type | Message",
        "notificação de falhas": "Timestamp | Type | Message",
        "depuração": "Timestamp | Type | Message"
    }
    actions = ["login", "logout", "update profile", "change password"]
    ecommerce_actions = ["view product", "add to cart", "purchase"]
    products = get_products_list()

    while True:
        log_type = random.choice(log_types)
        user_id = fake.uuid4()
        action = random.choice(actions)
        ecommerce_action = random.choice(ecommerce_actions)
        product = random.choice(products)
        
        message = {
            "auditoria": f"{datetime.now()} | auditoria | User {user_id} performed {action}",
            "comportamento do usuário": f"{datetime.now()} | comportamento do usuário | User {user_id} {ecommerce_action} {product}",
            "notificação de falhas": f"{datetime.now()} | notificação de falhas | Failure in {ecommerce_action} {product} at {fake.file_name()}:{random.randint(1, 100)} severity {random.choice(['low', 'medium', 'high'])}",
            "depuração": f"{datetime.now()} | depuração | Debug error occurred in {fake.word()} module"
        }[log_type]

        filepath = f"mock/{directory}/{log_type}.txt"
        file_exists = os.path.exists(filepath)
        with open(filepath, "a") as file:
            if not file_exists:
                file.write(column_headers[log_type] + "\n")
            file.write(message + "\n")
        
        time.sleep(1)


def cade_analytics():
    directory = "CadeAnalytics"
    criar_diretorio(f"mock/{directory}")
    
    produtos = get_products_list()
    usuarios = pd.read_csv("mock/ContaVerde/usuarios.csv")['ID'].tolist()
    acumulado_eventos = []
    last_sent_time = time.time()
    
    while True:
        if (time.time() - last_sent_time) >= 5.0:
            if acumulado_eventos:
                # Enviar para o servidor
                try:
                    response = requests.post(
                        "http://localhost:8080",
                        json=acumulado_eventos,
                        headers={'Content-Type': 'application/json'}
                    )
                    print(f"Status Code: {response.status_code}, Response: {response.json()}")
                except requests.RequestException as e:
                    print(f"Failed to send data: {e}")
            
            # Reset dos eventos acumulados e atualização do último tempo de envio
            acumulado_eventos = []
            last_sent_time = time.time()
        
        usuario_id = random.choice(usuarios)
        produto = random.choice(produtos)
        eventos = [{
            "timestamp": datetime.now().isoformat(),
            "usuario_id": usuario_id,
            "evento": random.choice(["visualizou", "adicionou ao carrinho", "comprou"]),
            "produto": produto
        }]
        
        acumulado_eventos.append(eventos)

        filepath = f"mock/{directory}/eventos_{datetime.now().strftime('%Y%m%d%H%M%S')}.json"
        with open(filepath, 'w') as f:
            json.dump(eventos, f, indent=4)
        
        time.sleep(random.randint(1, 5))  # Simula intervalo para o próximo evento



if __name__ == '__main__':
    processo_conta_verde = multiprocessing.Process(target=conta_verde)
    processo_data_cat = multiprocessing.Process(target=data_cat)
    processo_cade_analytics = multiprocessing.Process(target=cade_analytics)
    
    processo_conta_verde.start()
    processo_data_cat.start()
    processo_cade_analytics.start()
    
    processo_conta_verde.join()
    processo_data_cat.join()
    processo_cade_analytics.join()