#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
#include <vector>
#include <map>
#include <string>
#include <optional>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <cstdio>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <nlohmann/json.hpp>

//Культуры и регионы
const int CULTURES_COUNT = 16;
const int REGIONS_COUNT = 7;

const std::string CULTURES[CULTURES_COUNT] = { "barley", "corn", "corn_silage", "flax", "lentil", "oats", "peas", "sorghum", "soy", "spring_wheat",
"sugar_beet", "sunflower", "triticale_yarov", "winter_rapeseed", "winter_triticale", "winter_wheat" };

const std::string CULTURES_RUS[CULTURES_COUNT] = { u8"Ячмень", u8"Кукуруза на зерно", u8"Кукуруза на силос", u8"Лен", u8"Чечевица", u8"Овес", u8"Горох", u8"Сорго", u8"Соя", u8"Пшеница яровая",
u8"Сахарная свекла", u8"Подсолнечник", u8"Тритикале", u8"Рапс озимый", u8"Тритикале озимая", u8"Пшеница озимая" };

const std::string REGIONS[REGIONS_COUNT] = { "regionbels", "regionbelc", "regionbelk", "regiontams", "regiontamn", "regionorel", "regionprim", };

const std::string REGIONS_RUS[REGIONS_COUNT] = { u8"Белгород Юг",  u8"Белгород Центр", u8"Белгород-Курск", u8"Тамбов-Юг", u8"Тамбов-Север", u8"Орел", u8"Приморье" };

//Общие функции
#include "general_functions/dates_func.h"

//Модули
#include "module_calc_actual_execution_oper/functions.h"
#include "module_calc_actual_date_completion_entry_oper/functions.h"
#include "module_calc_minimal_planned_date/functions.h"

int main()
{
    SetConsoleOutputCP(65001);
    try 
    {
        int num_threads = omp_get_max_threads(); // Узнаем максимальное число потоков
        omp_set_num_threads(num_threads); // Устанавливаем число потоков

        
        //подключение к БД PostgreSQL
        soci::session sql(soci::postgresql, "dbname=agro_system user=xmatan16 password=matematic16 hostaddr=127.0.0.1 port=5432");

        //считывание initial_data из всех таблиц по всем культурам
        initial_data init_data[CULTURES_COUNT];
        read_table_initial_data(sql, init_data);

        //считывание data
        data data_shbn[CULTURES_COUNT][REGIONS_COUNT];
        read_table_data(sql, data_shbn);
        
        //считывания данных для формирования уникальных пар
        unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT];
        read_table_unique_pairs(sql, uniq_pairs);

        //data_shbn[10][0].print();
        clock_t start = clock();
        
        summarize(sql, init_data, data_shbn, uniq_pairs);
        calc_minimal_planned_date(sql, init_data);
        calc_minimal_date(init_data, uniq_pairs);

        
        clock_t end = clock();
        //uniq_pairs[10][1].print();
        //uniq_pairs.to_json_file("data.json");
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        save_jsons_uniq_pairs(uniq_pairs);
        to_json_all_file(uniq_pairs, "json/all.json");
        printf("The time: %f seconds\n", seconds);
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}