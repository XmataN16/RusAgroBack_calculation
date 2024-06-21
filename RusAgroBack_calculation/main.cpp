#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
#include <vector>
#include <string>
#include <optional>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <cstdio>
#include <windows.h>
#include <time.h>

//Культуры и регионы
const int CULTURES_COUNT = 18;
const int REGIONS_COUNT = 7;

const std::string CULTURES[CULTURES_COUNT] = { "barley", "corn_cfo", "corn_dfo", "corn_silage", "flax", "lentil", "oats", "peas", "sorghum", "soy_cfo", "soy_dfo", "spring_wheat",
"sugar_beet", "sunflower", "triticale_yarov", "winter_rapeseed", "winter_triticale", "winter_wheat" };

const std::string REGIONS[REGIONS_COUNT] = { "regionbels", "regionbelc", "regionbelk", "regiontams", "regiontamn", "regionorel", "regionprim", };

//Общие функции
#include "general_functions/dates_func.h"

//Модули
#include "module_calc_minimal_planned_date/functions.h"
#include "module_calc_actual_execution_oper/functions.h"
#include "module_calc_actual_date_completion_entry_oper/functions.h"

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
        read_tables(sql, init_data);

        //считывание data
        soci::rowset<soci::row> rs = (sql.prepare << "SELECT * FROM platform_shbn_data");
        data data_shbn(rs);

        clock_t start = clock();

        calc_minimal_planned_date(sql, init_data);
        unique_pairs uniq_pairs = get_unique_higher_tm_material_order(sql, data_shbn);
        uniq_pairs = calc_actual_input_date_completion_entry_oper(uniq_pairs, init_data);
        uniq_pairs = calc_actual_alternative_date_completion_entry_oper(uniq_pairs, init_data);
        uniq_pairs = calc_ten_percent(uniq_pairs, data_shbn);

        clock_t end = clock();

        uniq_pairs.print();
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        printf("The time: %f seconds\n", seconds);
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}