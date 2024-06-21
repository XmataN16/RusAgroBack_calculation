#pragma once
#include "class_DB/initial_data_class.h"

// Функции для расчета кратчайших плановых дат завершения

// Cчитывание таблицы по каждой культуре в массив в PostgreSQL
void read_tables(soci::session& sql, initial_data init_data[])
{

    for (int i = 0; i < CULTURES_COUNT; i++)
    {
        soci::rowset<soci::row> rs = (sql.prepare << "SELECT * FROM platform_shbn_initial_data_" << CULTURES[i] << " ORDER BY id");
        init_data[i] = initial_data(rs);
    }
}

//Вычисление плановой даты завершения
void calc_planned_date(initial_data init_data[], std::vector<std::optional<std::tm>>& planned_date, int culture, int region, int row)
{
    if (init_data[culture].noinput_deadline[row].has_value() && init_data[culture].regions[region][row].has_value())
    {
        std::optional<std::tm> date = init_data[culture].regions[region][row].value();
        int days = init_data[culture].noinput_deadline[row].value();
        std::optional<std::tm> temp = add_days(date, days);
        planned_date.push_back(temp);
    }
    else
    {
        planned_date.push_back(std::nullopt);
    }
}

//Вычисление вводной даты завершения
void calc_input_date(initial_data init_data[], std::vector<std::optional<std::tm>>& input_date, std::vector<std::optional<std::tm>>& minimal_date, int culture, int region, int row)
{
    if (init_data[culture].input_operation[row].has_value() && init_data[culture].deadline_input[row].has_value())
    {
        int input_operation_id = init_data[culture].input_operation[row].value();
        std::optional<std::tm> date;
        if (minimal_date[input_operation_id - 1].has_value())
        {
            date = minimal_date[input_operation_id - 1].value();
        }
        else
        {
            date = std::nullopt;
        }
        std::optional<int> days = init_data[culture].deadline_input[row].value();
        if (days.has_value() && date.has_value())
        {
            std::optional<std::tm> temp = add_days(date, days.value());
            input_date.push_back(temp.value());
        }
        else
        {
            input_date.push_back(std::nullopt);
        }
    }
    else
    {
        input_date.push_back(std::nullopt);
    }
}

//Вычисление альтеранативной даты завершения
void calc_alternative_date(initial_data init_data[], std::vector<std::optional<std::tm>>& alternative_date, std::vector<std::optional<std::tm>>& minimal_date, int culture, int region, int row)
{
    if (init_data[culture].alternative_input[row].has_value() && init_data[culture].alternative_complete[row].has_value())
    {
        int alternative_input_id = init_data[culture].alternative_input[row].value();
        std::optional<std::tm> date;
        if (minimal_date[alternative_input_id - 1].has_value())
        {
            date = minimal_date[alternative_input_id - 1].value();
        }
        else
        {
            date = std::nullopt;
        }
        std::optional<int> days = init_data[culture].alternative_complete[row].value();
        if (days.has_value() && date.has_value())
        {
            std::optional<std::tm> temp = add_days(date, days.value());
            alternative_date.push_back(temp.value());
        }
        else
        {
            alternative_date.push_back(std::nullopt);
        }
    }
    else
    {
        alternative_date.push_back(std::nullopt);
    }
}

//Вывод дат на экран консоли
void print_date(const std::vector<std::optional<std::tm>>(&date)[CULTURES_COUNT][REGIONS_COUNT])
{
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        std::cout << CULTURES[culture] << std::endl;
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            std::cout << REGIONS[region] << ":" << std::endl;
            for (int row = 0; row < date[culture][region].size(); row++)
            {
                if (date[culture][region][row].has_value())
                {
                    std::cout << tm_to_str(date[culture][region][row].value()).value() << "\n";
                }
                else
                {
                    std::cout << "NULL\n";
                }
            }
        }
    }
}

//Вычисление кратчайших плановых дат
void calc_minimal_planned_date(soci::session& sql, initial_data init_data[])
{
    std::vector<std::optional<std::tm>> planned_date[CULTURES_COUNT][REGIONS_COUNT];
    std::vector<std::optional<std::tm>> input_date[CULTURES_COUNT][REGIONS_COUNT];
    std::vector<std::optional<std::tm>> alternative_date[CULTURES_COUNT][REGIONS_COUNT];
    std::vector<std::optional<std::tm>> minimal_date[CULTURES_COUNT][REGIONS_COUNT];

    #pragma omp parallel for
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        #pragma omp parallel for
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int row = 0; row < init_data[culture].row_count; row++)
            {
                calc_planned_date(init_data, planned_date[culture][region], culture, region, row);
                calc_input_date(init_data, input_date[culture][region], minimal_date[culture][region], culture, region, row);
                calc_alternative_date(init_data, alternative_date[culture][region], minimal_date[culture][region], culture, region, row);

                long long int item_id = init_data[culture].id[row];
                std::vector<std::optional<std::tm>> dates = { planned_date[culture][region][item_id - 1], input_date[culture][region][item_id - 1], alternative_date[culture][region][item_id - 1] };

                if (alternative_date[culture][region][item_id - 1].has_value())
                {
                    minimal_date[culture][region].push_back(custom_max(dates));
                }
                else
                {
                    minimal_date[culture][region].push_back(custom_min(dates));
                }
            }
        }
    }
    //print_date(minimal_date);
}



