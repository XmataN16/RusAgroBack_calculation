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
void calc_planned_date(initial_data init_data[], int culture, int region, int row)
{
    if (init_data[culture].noinput_deadline[row].has_value() && init_data[culture].regions[region][row].has_value())
    {
        std::optional<std::tm> date = init_data[culture].regions[region][row].value();
        int days = init_data[culture].noinput_deadline[row].value();
        std::optional<std::tm> temp = add_days(date, days);
        init_data[culture].planned_date[region].push_back(temp);
    }
    else
    {
        init_data[culture].planned_date[region].push_back(std::nullopt);
    }
}

//Вычисление вводной даты завершения
void calc_input_date(initial_data init_data[], int culture, int region, int row)
{
    if (init_data[culture].input_operation[row].has_value() && init_data[culture].deadline_input[row].has_value())
    {
        int input_operation_id = init_data[culture].input_operation[row].value();
        std::optional<std::tm> date;
        if (init_data[culture].minimal_date[region][input_operation_id - 1].has_value())
        {
            date = init_data[culture].minimal_date[region][input_operation_id - 1].value();
        }
        else
        {
            date = std::nullopt;
        }
        std::optional<int> days = init_data[culture].deadline_input[row].value();
        if (days.has_value() && date.has_value())
        {
            std::optional<std::tm> temp = add_days(date, days.value());
            init_data[culture].input_date[region].push_back(temp.value());
        }
        else
        {
            init_data[culture].input_date[region].push_back(std::nullopt);
        }
    }
    else
    {
        init_data[culture].input_date[region].push_back(std::nullopt);
    }
}

//Вычисление альтеранативной даты завершения
void calc_alternative_date(initial_data init_data[], int culture, int region, int row)
{
    if (init_data[culture].alternative_input[row].has_value() && init_data[culture].alternative_complete[row].has_value())
    {
        int alternative_input_id = init_data[culture].alternative_input[row].value();
        std::optional<std::tm> date;
        if (init_data[culture].minimal_date[region][alternative_input_id - 1].has_value())
        {
            date = init_data[culture].minimal_date[region][alternative_input_id - 1].value();
        }
        else
        {
            date = std::nullopt;
        }
        std::optional<int> days = init_data[culture].alternative_complete[row].value();
        if (days.has_value() && date.has_value())
        {
            std::optional<std::tm> temp = add_days(date, days.value());
            init_data[culture].alternative_date[region].push_back(temp.value());
        }
        else
        {
            init_data[culture].alternative_date[region].push_back(std::nullopt);
        }
    }
    else
    {
        init_data[culture].alternative_date[region].push_back(std::nullopt);
    }
}

void calc_minimal_date(unique_pairs uniq_pairs, std::vector<std::optional<std::tm>> minimal_date[CULTURES_COUNT][REGIONS_COUNT], initial_data init_data[])
{
    std::cout << uniq_pairs.row_count << std::endl;
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int i = 0; i < uniq_pairs.row_count; i++)
            {
                for (int j = 0; j < init_data[culture].row_count; j++)
                {
                    if (uniq_pairs.material_order[i].value() == init_data[culture].operation[j].value() and uniq_pairs.nzp_zp[i].value() == init_data[culture].season[j].value())
                    {
                        if ((!uniq_pairs.actual_input_data[i].has_value()) and (!uniq_pairs.actual_alternative_data[i].has_value()))
                        {
                            
                        }

                    }
                }
                /*
                if ((!uniq_pairs.actual_input_data[i].has_value()))
                {
                    for (int j = 0; j < init_data[culture].row_count; j++)
                    {
                        if (!init_data[culture].noinput_deadline[j].has_value())
                        {
                            std::cout << init_data[culture].operation[j].value() << " " << CULTURES[culture] << std::endl;
                        }
                        if (uniq_pairs.material_order[i].value() == init_data[culture].operation[j].value() and uniq_pairs.nzp_zp[i].value() == init_data[culture].season[j].value())
                        {
                            std::optional<std::tm> date = init_data[culture].regions[region][j].value();
                            int days = init_data[culture].noinput_deadline[j].value();
                            uniq_pairs.minimal_date[i] = add_days(date, days);
                        }
                    }
                }
                else if (uniq_pairs.actual_alternative_data[i].has_value())
                {
                    std::vector<std::optional<std::tm>> dates = { uniq_pairs.actual_data[i], uniq_pairs.actual_input_data[i], uniq_pairs.actual_alternative_data[i] };
                    uniq_pairs.minimal_date[i] = custom_max(dates);
                }
                else if (uniq_pairs.actual_input_data[i].has_value())
                {
                    std::vector<std::optional<std::tm>> dates = { uniq_pairs.actual_data[i], uniq_pairs.actual_input_data[i], uniq_pairs.actual_alternative_data[i] };
                    uniq_pairs.minimal_date[i] = custom_min(dates);
                }
                */
            }
        }
    }
}

//Вывод дат на экран консоли
void print_date(initial_data init_data[])
{
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        std::cout << CULTURES[culture] << std::endl;
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            std::cout << REGIONS[region] << ":" << std::endl;
            for (int row = 0; row < init_data[culture].minimal_date[region].size(); row++)
            {
                if (init_data[culture].minimal_date[region][row].has_value())
                {
                    std::cout << tm_to_str(init_data[culture].minimal_date[region][row].value()).value() << "\n";
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
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int row = 0; row < init_data[culture].row_count; row++)
            {
                calc_planned_date(init_data, culture, region, row);
                calc_input_date(init_data, culture, region, row);
                calc_alternative_date(init_data, culture, region, row);

                std::vector<std::optional<std::tm>> dates = { init_data[culture].planned_date[region][row], init_data[culture].input_date[region][row], init_data[culture].alternative_date[region][row] };

                if (init_data[culture].alternative_date[region][row].has_value())
                {
                    init_data[culture].minimal_date[region].push_back(custom_max(dates));
                }
                else
                {
                    init_data[culture].minimal_date[region].push_back(custom_min(dates));
                }
            }
        }
    }
    print_date(init_data);
}



