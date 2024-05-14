#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <typeinfo>
#include <ctime> 
#include <numeric> 
#include <functional>
#include <utility>  
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <algorithm>
#include "Row.h" 
#include "DataFrame.h"
#include "Time.h"
#include "Uteis.h"
#include "../handler/handler.h" 
#include "../../src/handler/handler.cpp" 



using namespace std;

void sep(int n=100) {
    cout << string(n, '-') << endl;
};
int main() {
    cout << "Testando Time" << endl;
    cout << "Testando compareTimer com " << endl;

        // Example date-time string
    string dateTimeStr = "2019-04-23 03:55:45";

    // Convert string to Time object
    auto maybeTime = Time::fromString(dateTimeStr);
    maybeTime->print();
    // Check if the conversion was successful
    if (maybeTime.has_value()) {
        cout << "Successfully parsed the date-time string." << endl;
        cout << "The date-time is: ";
        maybeTime->print();  // Use the print method of Time class to display the date-time
    } else {
        cout << "Failed to parse the date-time string." << endl;
    }

    Time tm1(2019, 9, 20, 3, 55, 45); 

    Time tm2(2019, 9, 20, 2, 55, 12);
    
    bool result = Time::compareTime(tm1, tm2, Time::TimeMeasure::Year, Time::CompareType::Equal);
    cout << "Comparison result: " << result << endl;
    cout << "tm1 é igual tm2? " << (result ? "Sim" : "Não") << endl;
    sep();
    cout<< "Testando Row" << endl;
    sep();
    cout<< "Testando RowMerge" << endl;
    Row row0;
    Row row00;

    // Add columns to row1
    row0.addColRow("ID", 1);
    row0.addColRow("Name", string("John Doe"));
    row0.addColRow("Age", 30);
    row0.addColRow("City", string("New York"));

    // Add columns to row2
    row00.addColRow("ID", 1);
    row00.addColRow("Name", string("Mary"));
    row00.addColRow("Address", string("1234 Elm Street"));
    row00.addColRow("Phone", string("555-1234"));
    row00.addColRow("City", string("New York"));

    // Print the rows before merging
    cout << "Row 0 before merging:" << endl;
    row0.printRow();
    cout << "Row 00 before merging:" << endl;
    row00.printRow();

    // Attempt to merge the rows
    try {
        row0.mergeRows(row00, "City");
        cout << "Row 0 after merging:" << endl;
        row0.printRow();
    } catch (const exception& e) {
        cout << "Exception occurred: " << e.what() << endl;
    }
    sep();
    // Create a Row instance with an ID of 1
    Row row;

    // Add various types of data to the row
    row.addColRow("Age", 23);                       // Add an int
    row.addColRow("Temperature", 36.6);             // Add a double
    row.addColRow("Name", string("John Doe"));      // Add a string
    row.addColRow("LastName", string("Doe"));       // Add a string	

    // Adding a time structure
    Time now_tm(23, 9, 2019, 3, 55, 45);
    row.addColRow("CurrentTime", now_tm);           // Add a tm structure

    // Adding a vector of integers
    vector<int> numbers = {1, 2, 3, 4, 5};
    row.addColRow("Numbers", numbers);              // Add a vector<int>

    // Printing the entire row
    cout<< "print Row" << endl;
    row.printRow();
    cout << "===================" << endl;

    cout << "Size: " << row.size() << endl;
    // Accessing a specific column and printing its type
    auto age = row.getCol("Age");
    cout << "Type of 'Age' column: " << row.getTypeCol("Age") << endl;
    cout << "Type of 'Temperature' column: " << row.getTypeCol("Temperature") << endl;
    cout << "Type of 'CurrentTime' column: " << row.getTypeCol("CurrentTime") << endl;
    cout << "Type of 'Numbers' column: " << row.getTypeCol("Numbers") << endl;
    cout << "Type of 'Name' column: " << row.getTypeCol("Name") << endl;
    cout << "Type of 'LastName' column: " << row.getTypeCol("LastName") << endl;

    cout << "Value of 'Age' column: " << get<int>(age) << endl;

    // Remove a column and try to access it
    row.removeCol("Temperature");
    try {
        auto temp = row.getCol("Temperature");
    } catch (const exception& e) {
        cout << "Caught an exception trying to access removed column: " << e.what() << endl;
    }

    // Accessing and modifying vector<int> column
    auto& numbersCol = get<vector<int>>(row.getCol("Numbers"));
    numbersCol.push_back(6);
    cout << "Updated Numbers column: ";
    for (int num : numbersCol) {
        cout << num << " ";
    }
    cout << endl;
/////////////////////////////////////////////////////////////////////////
    sep();
    cout << "Testando DataFrame" << endl;
    sep();
    //
///////////////////////////////////////////////////////////////////////////////

    // Create column names and types
    vector<string> colNames = {"Name", "Age", "Scores"};
    vector<string> colTypes = {"string", "int", "vector<int>"};

    // Initialize DataFrame with column names and types
    DataFrame df(colNames, colTypes);

    
    auto row1 = make_shared<Row>();
    row1->addColRow("Name", string("Alice"));
    row1->addColRow("Age", 30);
    row1->addColRow("Scores", vector<int>{85, 92, 88});

    auto row2 = make_shared<Row>();
    row2->addColRow("Name", string("Bob"));
    row2->addColRow("Age", 25);
    row2->addColRow("Scores", vector<int>{75, 84, 79});

    auto row3 = make_shared<Row>();
    row3->addColRow("Name", string("Charlie"));
    row3->addColRow("Age", 35);
    row3->addColRow("Scores", vector<int>{90, 95, 92});

    df.insertRow(row1);
    df.insertRow(row2);
    df.insertRow(row3);
    cout << "linhas inseridas" << endl;
    cout << "Colunas: "; 
    for (const auto& col : df.getColumns()) cout << col << endl;
    cout << "Tipos: ";
    df.printTypes();
    cout << "Mostrando 3 primeiras linhas:" << endl;
    df.head(3);
    cout << "Mostrando 2 últimas linhas:" << endl;
    df.tail(2);
    sep();

    cout << "Testando containsColumns" << endl;
    if(df.containsColumn("Name")) {
        cout << "Coluna 'Name' encontrada" << endl;
    } else {
        cout << "Coluna 'Name' não encontrada" << endl;
    }
    cout << "Testando removeRow e removebyIDEvent:" << endl;
    // Removendo a primeira linha do DataFrame
    try {
        df.removeRow(0);
    } catch (const out_of_range& e) {
        cerr << "Erro: " << e.what() << endl;
    }
    cout << "df com primeira linha removida" << endl;
    df.head(2);
        try {
        df.removeRow(0);
    } catch (const out_of_range& e) {
        cerr << "Erro: " << e.what() << endl;
    }
    cout << "df com a segunda linha removida" << endl;
    df.head(1);

    df.head(1);
    df.insertRow(row1);
    df.insertRow(row2);
    sep();
    cout << "Testando getRows:" << endl;
    cout<<  "Acessando a primeira linha do DataFrame " << endl;
    try {
        auto firstRow = df.getRows()[0];
        firstRow->printRow();  // Supondo que o método print() está definido em Row
    } catch (const out_of_range& e) {
        cerr << "Erro: " << e.what() << endl;
    }
    sep();

    cout << "Criando df2" << endl;
    // Initialize DataFrame with column names and types
    DataFrame df2(colNames, colTypes);

    // Create rows and add data
    auto row4 = make_shared<Row>();
    row4->addColRow("Name", string("Dudu"));
    row4->addColRow("Age", 26);
    row4->addColRow("Scores", vector<int>{55, 22, 18});

    auto row5 = make_shared<Row>();
    row5->addColRow("Name", string("Herus"));
    row5->addColRow("Age", 31);
    row5->addColRow("Scores", vector<int>{95, 83, 39});

    auto row6 = make_shared<Row>();
    row6->addColRow("Name", string("Elis"));
    row6->addColRow("Age", 5);
    row6->addColRow("Scores", vector<int>{100, 102, 13});
    
    df2.insertRow(row4);
    df2.insertRow(row5);
    df2.insertRow(row6);
    try {
        df.merge(df2);
        cout << "Dados em df1 após o merge:" << endl;
        df.head(df.getRowCount());
    } catch (const invalid_argument& e) {
        cerr << "Erro ao mesclar: " << e.what() << endl;
    }

    sep();
    cout << "Testando innerJoin" << endl;
    // Create column names and types
    vector<string> colNamesIJ = {"Name", "Age", "Scores" , "IDi"};
    vector<string> colTypesIJ = {"string", "int", "vector<int>", "int"};

    // Initialize DataFrame with column names and types
    DataFrame df11(colNamesIJ, colTypesIJ);

    // Create rows and add data
    auto row11 = make_shared<Row>();
    row11->addColRow("Name", string("Alice"));
    row11->addColRow("Age", 30);
    row11->addColRow("Scores", vector<int>{85, 92, 88});
    row11->addColRow("IDi", 1);

    auto row21 = make_shared<Row>();
    row21->addColRow("Name", string("Bob"));
    row21->addColRow("Age", 25);
    row21->addColRow("Scores", vector<int>{75, 84, 79});
    row21->addColRow("IDi", 2);

    auto row31 = make_shared<Row>();
    row31->addColRow("Name", string("Charlie"));
    row31->addColRow("Age", 35);
    row31->addColRow("Scores", vector<int>{90, 95, 92});
    row31->addColRow("IDi", 3);

    df11.insertRow(row11);
    df11.insertRow(row21);
    df11.insertRow(row31);

   // Initialize DataFrame with column names and types
    vector<string> colNamesIJ2 = {"City", "Cats", "IDi"};
    vector<string> colTypesIJ2 = {"string", "int", "int"};
    DataFrame df12(colNamesIJ2, colTypesIJ2);

    // Create rows and add data
    auto row12 = make_shared<Row>();
    row12->addColRow("City", string("Rio de Janeiro"));
    row12->addColRow("Cats", 3);
    row12->addColRow("IDi", 1);

    auto row22 = make_shared<Row>();
    row22->addColRow("City", string("Moscow"));
    row22->addColRow("Cats", 2);
    row22->addColRow("IDi", 2);

    auto row32 = make_shared<Row>();
    row32->addColRow("City", string("Sidney"));
    row32->addColRow("Cats", 5);
    row32->addColRow("IDi", 4);

    df12.insertRow(row12);
    df12.insertRow(row22);
    df12.insertRow(row32);

    // Realizando o inner join nos DataFrames
    df11.innerJoin(df12, "IDi");
    df11.printDataFrame();    

    sep();
    cout << "Testando groupBySum" << endl;
    // Create column names and types
    vector<string> colNamesGBS = {"Name", "Age", "Scores" , "ID"};
    vector<string> colTypesGBS = {"string", "int", "vector<int>", "int"};
    DataFrame dfGBS(colNamesGBS, colTypesGBS);

    // Create rows and add data
    auto rowGBS1 = make_shared<Row>();
    rowGBS1->addColRow("Name", string("Alice"));
    rowGBS1->addColRow("Age", 30);
    rowGBS1->addColRow("Scores", vector<int>{85, 92, 88});
    rowGBS1->addColRow("ID", 1);

    auto rowGBS2 = make_shared<Row>();
    rowGBS2->addColRow("Name", string("Bob"));
    rowGBS2->addColRow("Age", 25);
    rowGBS2->addColRow("Scores", vector<int>{75, 84, 79});
    rowGBS2->addColRow("ID", 2);

    auto rowGBS3 = make_shared<Row>();
    rowGBS3->addColRow("Name", string("Bob"));
    rowGBS3->addColRow("Age", 35);
    rowGBS3->addColRow("Scores", vector<int>{90, 95, 92});
    rowGBS3->addColRow("ID", 3);

    dfGBS.insertRow(rowGBS1);
    dfGBS.insertRow(rowGBS2);
    dfGBS.insertRow(rowGBS3);   

    sep();
    sep();
    // Definindo as colunas para o DataFrame 1
    vector<string> colNames1 = {"Date", "Name", "ID", "Price", "Scores"};
    vector<string> colTypes1 = {"string", "string", "int", "double", "vector<int>"};

    // Inicializando o DataFrame 1
    DataFrame salesDF(colNames1, colTypes1);

    // Criando e adicionando linhas ao DataFrame de vendas
    auto saleRow1 = make_shared<Row>();
    saleRow1->addColRow("Date", Time(20,04,2024));
    saleRow1->addColRow("Name", string("Smartphone X"));
    saleRow1->addColRow("ID", 101);
    saleRow1->addColRow("Price", 999.99);
    saleRow1->addColRow("Scores", vector<int>{88, 90, 85});

    auto saleRow2 = make_shared<Row>();
    saleRow2->addColRow("Date", Time(21,04,2024));
    saleRow2->addColRow("Name", string("Laptop Pro"));
    saleRow2->addColRow("ID", 102);
    saleRow2->addColRow("Price", 1200.00);
    saleRow2->addColRow("Scores", vector<int>{92, 89, 91});

    salesDF.insertRow(saleRow1);
    salesDF.insertRow(saleRow2);

    // Definindo as colunas para o DataFrame 2
    vector<string> colNames2 = {"Date", "Name", "ID", "Reviewer", "Rating"};
    vector<string> colTypes2 = {"string", "string", "int", "string", "int"};

    // Inicializando o DataFrame de avaliações
    DataFrame reviewsDF(colNames2, colTypes2);

    // Criando e adicionando linhas ao DataFrame de avaliações
    auto reviewRow1 = make_shared<Row>();
    reviewRow1->addColRow("Date", Time(20,04,2024));
    reviewRow1->addColRow("Name", string("Smartphone X"));
    reviewRow1->addColRow("ID", 101);
    reviewRow1->addColRow("Reviewer", string("John Doe"));
    reviewRow1->addColRow("Rating", 5);

    auto reviewRow2 = make_shared<Row>();
    reviewRow2->addColRow("Date", Time(21,04,2024));
    reviewRow2->addColRow("Name", string("Laptop Pro"));
    reviewRow2->addColRow("ID", 102);
    reviewRow2->addColRow("Reviewer", string("Jane Smith"));
    reviewRow2->addColRow("Rating", 4);

    reviewsDF.insertRow(reviewRow1);
    reviewsDF.insertRow(reviewRow2);

    // Imprimindo ambos os DataFrames
    cout << "Sales DataFrame:" << endl;
    salesDF.printDataFrame();
    cout << "Reviews DataFrame:" << endl;
    reviewsDF.printDataFrame();

    cout << "Realizando o inner join nos DataFrames de vendas e avaliações" << endl;
    salesDF.innerJoin(reviewsDF, "Date");
    salesDF.printDataFrame();
    //Realizando o groupBySum
    cout << "Realizando o groupBySum no DataFrame de vendas" << endl;
    DataFrame resultado = dfGBS.groupBySum("Name", "Age");
    resultado.printDataFrame();
    sep(); 
    cout << "Realizando o groupByAvg no DataFrame de vendas" << endl;
    DataFrame resultado1 = dfGBS.groupByAvg("Name", "Age");
    resultado1.printDataFrame();
    sep();  
    cout << "Realizando o groupByFreq no DataFrame de vendas" << endl;
    DataFrame resultado2 = df11.groupByFreq("Scores");
    resultado2.printDataFrame();
    sep();

    string log = "Timestamp | Type | Message\n2024-04-22 03:04:11.472839 | comportamento do usuário | User a3d53651-1ce2-4caf-9fc9-b12b37331182 purchase impact\n2024-04-22 03:04:15.487338 | comportamento do usuário | User 988aea36-45e2-426b-893e-a905abd0671d add to cart impact\n2024-04-22 03:04:16.489786 | comportamento do usuário | User bc63ffc7-c0fe-49ee-9f2d-96fd44efb2e0 view product expert\n2024-04-22 03:04:17.491797 | comportamento do usuário | User 555a9325-4e52-483e-ace6-0ff3b02c9dfd purchase impact\n2024-04-22 03:04:24.522401 | comportamento do usuário | User c0766b0d-89ae-4918-a6ae-cf37c9544bcf purchase expert";
    
    cout << "Testando splitLogEntries" << endl;
    vector<vector<string>> entriesLog = splitLogEntries(log);
    for (const auto& entry : entriesLog) {
        cout<< "entry: " << endl;	
        for (const auto& field : entry) {
            cout << field << ":--:" << " ";
        }
        cout << endl;
    }
    cout<< "Testando processLogData" << endl;
    DataFrame dfLog = processLogData(log);
    dfLog.printDataFrame();

    sep();
    sep();
    string CSV = "ID,Nome,Imagem,Descrição,Preço\ne4997d94-89c0-4b5a-87cb-d853613b9a89,child,https://picsum.photos/1016/883,Next structure me wait peace movie.,63355\n98e2c27d-8290-4266-a012-e7b90fff7b2e,man,https://placekitten.com/53/529,Edge six page ever conference easy.,18858\n6e779c19-3abd-454a-87ec-7b71a94becdd,develop,https://picsum.photos/797/311,Such happy bring garden.,28886";
    // Processar a string CSV através da função
    vector<vector<string>> csv = splitCsvEntries(CSV);

    cout << "Testando splitCsvEntries2" << endl;
    for (const auto& entry : csv) {
        cout << "entry: " << endl;
        for (const auto& field : entry) {
            cout << field << ":--:" << " ";
        }
        cout << endl;
    }

    sep();
    cout << "Testando processCsv" << endl;
    DataFrame dfsCsv = processCsvData(CSV);
    dfsCsv.printDataFrame();

    sep();
    sep();
    
    string Json= R"([{"data_notificacao": "2024-04-20 08:09:14", "id_usuario": "b306f3da-265f-4ea9-9028-c2f856480ae8", "estimulo": "hotel", "componente_alvo": "every"}, {"data_notificacao": "2024-04-20 08:09:38", "id_usuario": "aadc037f-8546-47c7-aff3-720130622c94", "estimulo": "west", "componente_alvo": "region"}, {"data_notificacao": "2024-04-20 08:09:43", "id_usuario": "5b9ba138-37da-4a21-98ce-a908db6622c1", "estimulo": "board", "componente_alvo": "standard"}, {"data_notificacao": "2024-04-20 08:09:45", "id_usuario": "e26a6fbe-e6f2-4c25-b192-d9a69ec4aeff", "estimulo": "account", "componente_alvo": "charge"}, {"data_notificacao": "2024-04-20 08:09:54", "id_usuario": "1c162f17-40e8-4ee0-93ab-a26dc0aed37b", "estimulo": "our", "componente_alvo": "house"}, {"data_notificacao": "2024-04-20 08:09:57", "id_usuario": "22c3907e-8ade-4d76-9498-52b8e77a3c3a", "estimulo": "direction", "componente_alvo": "every"}, {"data_notificacao": "2024-04-20 08:10:05", "id_usuario": "d7b35625-b5e4-4b3a-bf70-dbb2196ac71c", "estimulo": "test", "componente_alvo": "white"}, {"data_notificacao": "2024-04-20 08:10:09", "id_usuario": "f04e482d-b704-4420-8747-23ea70733321", "estimulo": "guy", "componente_alvo": "enjoy"}, {"data_notificacao": "2024-04-20 08:10:12", "id_usuario": "9feaa019-702c-4942-a2a7-7c479329b99d", "estimulo": "weight", "componente_alvo": "his"}])";
    sep();
    sep();
    cout<< "Testando processJson" << endl;
    DataFrame dfsJson = processJson(Json);
    dfsJson.printDataFrame();


    

    ///////////////////////////////////////
    /*
    cout << "Testando BalanceHandler" << endl;
    // Inicializa uma Queue para passar os DataFrames
    Queue<DataFrame*> input_queue;
    // Adiciona um DataFrame à fila de entrada
    input_queue.enqueue(&df);
    // Fila saída
    Queue<DataFrame*> output_queue;
    // Inicializar um vetor de queues de saída
    vector<Queue<DataFrame*>> output_queues;
    output_queues.push_back(output_queue);
    // Inicializar um BalanceHandler
    BalanceHandler balanceHandler(&input_queue, &output_queues);
    // Chamar a função BalancerFunction para processar os dados
    balanceHandler.BalancerFunction();
    cout << "DataFrames de saída:" << endl;
    for (size_t i = 0; i < output_queues.size(); ++i) {
        cout << "Fila " << i << ":" << endl;
        while (!output_queues[i].empty()) {
            DataFrame* df_ptr = *output_queues[i].dequeue();
            df_ptr->printDataFrame();
        }
    }

    sep();

    cout << "Testando CleanCache" << endl;
    // Inicializa uma Queue para passar os DataFrames
    Queue<DataFrame*> input_queueCC;
    // Adiciona um DataFrame à fila de entrada
    input_queueCC.enqueue(&df);
    // Inicializar um vetor de queues de saída
    vector<Queue<DataFrame*>> output_queuesCC;
    // Inicializar um CleanCache
    CleanCache cleanCache(&input_queueCC, &output_queuesCC);
    // Chamar a função CleanCacheFunction para processar os dados
    cleanCache.CleanCacheFunction();
    // Imprimir o dataframe de saída
    cout << "DataFrames de saída:" << endl;
    while (!output_queuesCC[0].empty()) {
        DataFrame* df_ptr = *output_queuesCC[0].dequeue();
        df_ptr->printDataFrame();
    }

    sep();

    cout << "Testando FilterHandler" << endl;
    
    // Inicializa uma Queue para passar os DataFrames
    Queue<DataFrame*> input_queueFH;
    // Adiciona um DataFrame à fila de entrada
    input_queueFH.enqueue(&df);
    // Inicializar uma fila de saída]
    Queue<DataFrame*> output_queueFH;
    // Inicializar um vetor de queues de saída
    vector<Queue<DataFrame*>> output_queuesFH;
    // Adiciona a fila de saída ao vetor de filas de saída
    output_queuesFH.push_back(output_queueFH);
    // Inicializar um FilterHandler
    FilterHandler filterHandler(&input_queueCC, &output_queuesFH);
    // Chamar a função FilterFunction para processar os dados
    filterHandler.FilterFunction(df);
    // Imprimir o dataframe de saída
    cout << "DataFrames de saída:" << endl;
    while (!output_queuesFH[0].empty()) {
        DataFrame* df_ptr = *output_queuesFH[0].dequeue();
        df_ptr->printDataFrame();
    }
    */


    return 0;
}
