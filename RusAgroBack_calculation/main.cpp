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
#include <boost/locale.hpp>
#include <chrono>
// 

//Общие функции
#include "general_functions/dates_func.h"

//Культуры и регионы
const int CULTURES_COUNT = 16;
const int REGIONS_COUNT = 7;

const std::string CULTURES[CULTURES_COUNT] = { "barley", "corn", "corn_silage", "flax", "lentil", "oats", "peas", "sorghum", "soy", "spring_wheat",
"sugar_beet", "sunflower", "triticale_yarov", "winter_rapeseed", "winter_triticale", "winter_wheat" };

const std::string CULTURES_RUS[CULTURES_COUNT] = { u8"Ячмень", u8"Кукуруза на зерно", u8"Кукуруза на силос", u8"Лен", u8"Чечевица", u8"Овес", u8"Горох", u8"Сорго", u8"Соя", u8"Пшеница яровая",
u8"Сахарная свекла", u8"Подсолнечник", u8"Тритикале", u8"Рапс озимый", u8"Тритикале озимая", u8"Пшеница озимая" };

const std::string REGIONS[REGIONS_COUNT] = { "regionbels", "regionbelc", "regionbelk", "regiontams", "regiontamn", "regionorel", "regionprim", };

const std::string REGIONS_RUS[REGIONS_COUNT] = { u8"Белгород Юг",  u8"Белгород Центр", u8"Белгород-Курск", u8"Тамбов-Юг", u8"Тамбов-Север", u8"Орел", u8"Приморье" };

std::string years[2] = { getYearAsString(-1), getYearAsString() };



//Модули
#include "module_calc_actual_execution_oper/functions.h"
#include "module_calc_actual_date_completion_entry_oper/functions.h"
#include "module_calc_minimal_planned_date/functions.h"

//Функции для дебага
//#include "DEBUG_functions.h"

void make_combined(unique_pairs(&uniq_pairs)[2][CULTURES_COUNT][REGIONS_COUNT], unique_pairs combined[CULTURES_COUNT][REGIONS_COUNT])
{
    for (int yeah = 0; yeah < 2; yeah++)
    {
        for (int i = 0; i < CULTURES_COUNT; i++)
        {
            for (int j = 0; j < REGIONS_COUNT; j++)
            {
                for (int row = 0; row < uniq_pairs[yeah][i][j].row_count; row++)
                {
                    combined[i][j].actual_alternative_data.push_back(uniq_pairs[yeah][i][j].actual_alternative_data[row]);
                    combined[i][j].actual_data.push_back(uniq_pairs[yeah][i][j].actual_data[row]);
                    combined[i][j].actual_input_data.push_back(uniq_pairs[yeah][i][j].actual_input_data[row]);
                    combined[i][j].business_dir.push_back(uniq_pairs[yeah][i][j].business_dir[row]);
                    combined[i][j].culture.push_back(uniq_pairs[yeah][i][j].culture[row]);
                    combined[i][j].higher_tm.push_back(uniq_pairs[yeah][i][j].higher_tm[row]);
                    combined[i][j].is_actual.push_back(uniq_pairs[yeah][i][j].is_actual[row]);
                    combined[i][j].is_completed.push_back(uniq_pairs[yeah][i][j].is_completed[row]);
                    combined[i][j].material_order.push_back(uniq_pairs[yeah][i][j].material_order[row]);
                    combined[i][j].minimal_date.push_back(uniq_pairs[yeah][i][j].minimal_date[row]);
                    combined[i][j].minimal_planned_date.push_back(uniq_pairs[yeah][i][j].minimal_planned_date[row]);
                    combined[i][j].nzp_zp.push_back(uniq_pairs[yeah][i][j].nzp_zp[row]);
                    combined[i][j].order.push_back(uniq_pairs[yeah][i][j].order[row]);
                    combined[i][j].pu.push_back(uniq_pairs[yeah][i][j].pu[row]);
                    combined[i][j].sawing_date.push_back(uniq_pairs[yeah][i][j].sawing_date[row]);
                    combined[i][j].status.push_back(uniq_pairs[yeah][i][j].status[row]);
                    combined[i][j].ten_percent.push_back(uniq_pairs[yeah][i][j].ten_percent[row]);
                    combined[i][j].t_material.push_back(uniq_pairs[yeah][i][j].t_material[row]);
                    combined[i][j].year.push_back(uniq_pairs[yeah][i][j].year[row]);

                    combined[i][j].row_count++;
                }
                
            }
        }
    }
    
}

int main()
{
    SetConsoleOutputCP(65001);

    try 
    {
        std::cout << years[0] << " " << years[1] << std::endl;
        int num_threads = omp_get_max_threads(); // Узнаем максимальное число потоков
        omp_set_num_threads(num_threads); // Устанавливаем число потоков

        //подключение к БД PostgreSQL
        soci::session sql(soci::postgresql, "dbname=agro_system user=xmatan16 password=matematic16 hostaddr=127.0.0.1 port=5432");

        //считывание initial_data из всех таблиц по всем культурам
        initial_data init_data[2][CULTURES_COUNT];

        data data_shbn[2][CULTURES_COUNT][REGIONS_COUNT];

        unique_pairs uniq_pairs[2][CULTURES_COUNT][REGIONS_COUNT];

        //data_shbn[10][0].print();
        clock_t start = clock();

        for (int i = 0; i < 2; i++)
        {
            read_table_initial_data(sql, init_data[i], years[i]);
            //считывание data
            read_table_data(sql, data_shbn[i], years[i]);

            //считывания данных для формирования уникальных пар
            read_table_unique_pairs(sql, uniq_pairs[i], years[i]);

            summarize(sql, init_data[i], data_shbn[i], uniq_pairs[i]);

            calc_minimal_planned_date(sql, init_data[i]);

            calc_minimal_date(init_data[i], uniq_pairs[i]);
        }

        unique_pairs combined[CULTURES_COUNT][REGIONS_COUNT];

        make_combined(uniq_pairs, combined);

        clock_t end = clock();
        //uniq_pairs[10][1].print();
        //uniq_pairs.to_json_file("data.json");
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        truncate_table(sql, "platform_shbn_control_operations");
        export_to_platform_shbn(sql, combined);
        //save_jsons_uniq_pairs(combined);
        //to_json_all_file(combined, "./json/all.json");
        printf("The time: %f seconds\n", seconds);
        //DEBUG_get_status_count_rows_on_material_order_and_higher_tm(uniq_pairs, u8"Боронование (ГА)", u8"BL-08-08-21-0002");
        //DEBUG_data_get_count_rows_on_material_order_and_higher_tm(data_shbn, u8"Посев с внесением удобрений (ГА)", u8"TM-06-21-05-0008");
        //DEBUG_uniq_pairs_get_count_rows_on_material_order_and_higher_tm(uniq_pairs, u8"Посев с внесением удобрений (ГА)", u8"TM-06-21-05-0008");
        //DEBUG_get_count_rows_on_status(uniq_pairs);
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}