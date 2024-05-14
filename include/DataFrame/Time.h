#ifndef TIME_H
#define TIME_H

#include <iomanip>
#include <sstream>
#include <string>
#include <optional>

using namespace std;

/**
 * @class Time
 * @brief Classe para representar e manipular datas e horas.
 *
 * Esta classe permite criar e manipular objetos de data e hora, fornecendo
 * métodos para comparação, entrada e saída formatadas. Ela suporta operações comuns
 * como comparação de tempo e conversão de strings para objetos Time.
 */
class Time {
private:
    int day;  
    int month;  
    int year;  
    int hour;   
    int minute; 
    int second; 

 public:
    enum class TimeMeasure { Year, Month, Day, Hour, Minute, Second };  
    enum class CompareType { Less, LessOrEqual, Equal, Greater, GreaterOrEqual };   

    /**
    * @brief Construtor que inicializa um objeto Time com data e hora específicas.
    * @param day Dia do mês
    * @param month Mês
    * @param year Ano
    * @param hour Hora (padrão 0)
    * @param minute Minuto (padrão 0)
    * @param second Segundo (padrão 0)
    */
    Time(int day, int month, int year, int hour=0, int minute=0, int second=0)
        : day(day), month(month), year(year), hour(hour), minute(minute), second(second) {} 
    /**
     * @brief Cria um objeto Time a partir de uma string formatada.
     * @param dateTimeString String contendo a data e hora no formato "YYYY-MM-DD HH:MM:SS"
     * @return optional<Time> Um objeto Time se bem-sucedido, nullopt caso contrário.
     */
    static std::optional<Time> fromString(const std::string& dateTimeString) {
        if(dateTimeString.size() != 10 && dateTimeString.size() != 19)
        {
            return std::nullopt;
        }
        int y, m, d, hr = 0, min = 0, sec = 0;

        // Verifica se é apenas a data (YYYY-MM-DD)
        if (dateTimeString.size() == 10) {
            y = std::stoi(dateTimeString.substr(0, 4));
            m = std::stoi(dateTimeString.substr(5, 2));
            d = std::stoi(dateTimeString.substr(8, 2));
        }
        // Verifica se é data e hora (YYYY-MM-DD HH:MM:SS)
        else if (dateTimeString.size() == 19) {
            y = std::stoi(dateTimeString.substr(0, 4));
            m = std::stoi(dateTimeString.substr(5, 2));
            d = std::stoi(dateTimeString.substr(8, 2));
            hr = std::stoi(dateTimeString.substr(11, 2));
            min = std::stoi(dateTimeString.substr(14, 2));
            sec = std::stoi(dateTimeString.substr(17, 2));
        } else {
            // Se o tamanho não corresponde a nenhum formato conhecido
            return std::nullopt;
        }

        try {
            // Cria e retorna um objeto Time se todos os valores forem convertidos corretamente
            return Time(d, m, y, hr, min, sec);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Erro: String inválida - " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Erro: Número fora do intervalo de int - " << e.what() << std::endl;
        }

        // Retorna nullopt se ocorrer qualquer erro
        return std::nullopt;
    }

    // Getters
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    int getHour() const { return hour; }
    int getMinute() const { return minute; }
    int getSecond() const { return second; }

    // Setters
    void setDay(int newDay) { day = newDay; }
    void setMonth(int newMonth) { month = newMonth; }
    void setYear(int newYear) { year = newYear; }
    void setHour(int newHour) { hour = newHour; }
    void setMinute(int newMinute) { minute = newMinute; }
    void setSecond(int newSecond) { second = newSecond; } 

    /**
     * @brief Imprime a data e hora no formato "DD-MM-YYYY HH:MM:SS".
     */
    void print() const {
      cout << setfill('0') << setw(2) << day << "-"
                << setfill('0') << setw(2) << month << "-"
                << year << " "
                << setfill('0') << setw(2) << hour << ":"
                << setfill('0') << setw(2) << minute << ":"
                << setfill('0') << setw(2) << second << endl;
    }

    /**
    * @brief Compara duas instâncias de Time de acordo com uma unidade de tempo e tipo de comparação especificados.
    * @param a Primeira instância de Time para comparação.
    * @param b Segunda instância de Time para comparação.
    * @param measure Unidade de medida para comparação (ex., Ano, Mês, Dia).
    * @param cmpType Tipo de comparação (ex., Menor, Maior).
    * @return bool Resultado da comparação conforme especificado pelo tipo.
    */
    static bool compareTime(const Time& a, const Time& b, TimeMeasure measure, CompareType cmpType) {
        auto compare = [&](int lhs, int rhs) -> bool {
            switch (cmpType) {
                case CompareType::Less: return lhs < rhs;
                case CompareType::LessOrEqual: return lhs <= rhs;
                case CompareType::Equal: return lhs == rhs;
                case CompareType::Greater: return lhs > rhs;
                case CompareType::GreaterOrEqual: return lhs >= rhs;
                default: throw invalid_argument("Invalid comparison type.");
            }
        };     
        if (measure >= TimeMeasure::Year && a.getYear() != b.getYear())
            return compare(a.getYear(), b.getYear());
        if (measure >= TimeMeasure::Month && a.getMonth() != b.getMonth())
            return compare(a.getMonth(), b.getMonth());
        if (measure >= TimeMeasure::Day && a.getDay() != b.getDay())
            return compare(a.getDay(), b.getDay());
        if (measure >= TimeMeasure::Hour && a.getHour() != b.getHour())
            return compare(a.getHour(), b.getHour());
        if (measure >= TimeMeasure::Minute && a.getMinute() != b.getMinute())
            return compare(a.getMinute(), b.getMinute());
        if (measure >= TimeMeasure::Second && a.getSecond() != b.getSecond())
            return compare(a.getSecond(), b.getSecond());     
        return cmpType == CompareType::Equal || cmpType == CompareType::LessOrEqual || cmpType == CompareType::GreaterOrEqual;
    }
  
    // Operadores de comparação
    friend bool operator!=(const Time& lhs, const Time& rhs) {
        return !(lhs.getSecond() == rhs.getSecond() &&
                 lhs.getMinute() == rhs.getMinute() &&
                 lhs.getHour() == rhs.getHour() &&
                 lhs.getDay() == rhs.getDay() &&
                 lhs.getMonth() == rhs.getMonth() &&
                 lhs.getYear() == rhs.getYear());
    }

    friend bool operator==(const Time& lhs, const Time& rhs) {
        return lhs.getSecond() == rhs.getSecond() &&
               lhs.getMinute() == rhs.getMinute() &&
               lhs.getHour() == rhs.getHour() &&
               lhs.getDay() == rhs.getDay() &&
               lhs.getMonth() == rhs.getMonth() &&
               lhs.getYear() == rhs.getYear();
    }

    friend bool operator<(const Time& lhs, const Time& rhs) {
        if (lhs.getYear() != rhs.getYear()) return lhs.getYear() < rhs.getYear();
        if (lhs.getMonth() != rhs.getMonth()) return lhs.getMonth() < rhs.getMonth();
        if (lhs.getDay() != rhs.getDay()) return lhs.getDay() < rhs.getDay();
        if (lhs.getHour() != rhs.getHour()) return lhs.getHour() < rhs.getHour();
        if (lhs.getMinute() != rhs.getMinute()) return lhs.getMinute() < rhs.getMinute();
        return lhs.getSecond() < rhs.getSecond();
    }

    friend bool operator>(const Time& lhs, const Time& rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const Time& lhs, const Time& rhs) {
        return !(lhs > rhs);
    }

    friend bool operator>=(const Time& lhs, const Time& rhs) {
        return !(lhs < rhs);
    }

};


#endif // TIME_H