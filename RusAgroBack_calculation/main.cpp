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

const int CULTURES_COUNT = 18;
const int REGIONS_COUNT = 7;

const std::string CULTURES[CULTURES_COUNT] = { "barley", "corn_cfo", "corn_dfo", "corn_silage", "flax", "lentil", "oats", "peas", "sorghum", "soy_cfo", "soy_dfo", "spring_wheat",
"sugar_beet", "sunflower", "triticale_yarov", "winter_rapeseed", "winter_triticale", "winter_wheat" };

const std::string REGIONS[REGIONS_COUNT] = { "regionbels", "regionbelc", "regionbelk", "regiontams", "regiontamn", "regionorel", "regionprim" };

//модуль с функциями для расчета минимальных плановых дат
#include "module_calc_minimal_planned_date/functions.h"

int main()
{
    SetConsoleOutputCP(65001);
    try 
    {
        soci::session sql(soci::postgresql, "dbname=agro_system user=xmatan16 password=matematic16 hostaddr=127.0.0.1 port=5432");

        calc_minimal_planned_date(sql);
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}