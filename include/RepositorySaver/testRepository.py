import sqlite3

def query_database(db_path):
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    # Consultar todos os registros na tabela Products
    cursor.execute("SELECT * FROM Products")
    
    # Recuperar e imprimir todos os registros
    records = cursor.fetchall()
    if records:
        print("Data fetched from database:")
        for record in records:
            print(record)
    else:
        print("No data found in the database.")
    
    # Fechar a conexão com o banco de dados
    conn.close()

# Caminho para o banco de dados SQLite
db_path = 'test.db'
query_database(db_path)
