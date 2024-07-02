#pragma once
#include "class_DB/initial_data_class.h"

// Функции для расчета кратчайших плановых дат завершения

// Cчитывание таблицы по каждой культуре в массив в PostgreSQL
void read_table_initial_data(soci::session& sql, initial_data init_data[])
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

std::string set_status(std::optional<std::tm> actual_date, std::optional<std::tm> minimal_date)
{
    // Получаем текущее время в формате time_t
    std::time_t t = std::time(nullptr);

    // Преобразуем time_t в локальное время и сохраняем его в структуре std::tm
    std::tm* current_time = std::localtime(&t);
    if (!minimal_date.has_value())
    {
        return u8"Операция не отслеживается";
    }
    else if (!actual_date.has_value() and *current_time < minimal_date.value())
    {
        return u8"Не завершено";
    }
    else if (!actual_date.has_value() and *current_time > minimal_date.value())
    {
        return u8"Просрочено";
    }
    else if (actual_date.has_value() and actual_date.value() <= minimal_date.value())
    {
        return u8"Выполнено в срок";
    }
    else if (actual_date.has_value() and actual_date.value() > minimal_date.value())
    {
        return u8"Выполнено не в срок";
    }
}


std::string set_is_actual(std::optional<std::string> status, std::optional<std::tm> actual_data, std::optional<std::tm> actual_input_data, std::optional<std::tm> actual_alternative_data)
{
    if (status == u8"Операция не отслеживается")
    {
        return u8"Статус отсутствует";
    }
    else if (actual_input_data.has_value() or actual_alternative_data.has_value() or actual_data.has_value())
    {
        return u8"Актуально";
    }
    else if (!actual_input_data.has_value() or !actual_alternative_data.has_value() or !actual_data.has_value())
    {
        return u8"Ориентировочно";
    }
}

void calc_minimal_date(initial_data init_data[], unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int i = 0; i < uniq_pairs[culture][region].row_count; i++)
            {
                for (int j = 0; j < init_data[culture].row_count; j++)
                {
                    if (uniq_pairs[culture][region].material_order[i].value() == init_data[culture].operation[j].value() and uniq_pairs[culture][region].nzp_zp[i].value() == init_data[culture].season[j].value())
                    {
                        std::optional<std::tm> min_plan_date = init_data[culture].input_operation[j].has_value() ? init_data[culture].minimal_date[region][j] : add_days(uniq_pairs[culture][region].ten_percent[i], init_data[culture].noinput_deadline[j].value());
                        uniq_pairs[culture][region].minimal_planned_date[i] = init_data[culture].minimal_date[region][j];
                        if ((!init_data[culture].input_operation[j].has_value()) and (!init_data[culture].alternative_input[j].has_value()))
                        {
                            if (uniq_pairs[culture][region].ten_percent[i].has_value())
                            {
                                std::tm end_ten_percent = add_days(uniq_pairs[culture][region].ten_percent[i], init_data[culture].noinput_deadline[j].value()).value();
                                uniq_pairs[culture][region].minimal_date[i] = end_ten_percent;
                            }
                            else
                            {
                                uniq_pairs[culture][region].minimal_date[i] = min_plan_date;
                            }
                        }
                        else if (!init_data[culture].alternative_input[j].has_value())
                        {
                            if (!uniq_pairs[culture][region].actual_input_data[i].has_value())
                            {
                                uniq_pairs[culture][region].minimal_date[i] = min_plan_date;
                            }
                            else
                            {
                                uniq_pairs[culture][region].minimal_date[i] = min_date(uniq_pairs[culture][region].actual_input_data[i], min_plan_date);
                            }
                        }
                        else
                        {
                            std::vector<std::optional<std::tm>> dates = { uniq_pairs[culture][region].actual_input_data[i], uniq_pairs[culture][region].actual_alternative_data[i], min_plan_date };
                            uniq_pairs[culture][region].minimal_date[i] = custom_max(dates);
                        }
                    }
                    uniq_pairs[culture][region].status[i] = set_status(uniq_pairs[culture][region].actual_data[i], uniq_pairs[culture][region].minimal_date[i]);
                    uniq_pairs[culture][region].is_actual[i] = set_is_actual(uniq_pairs[culture][region].status[i], uniq_pairs[culture][region].actual_data[i], uniq_pairs[culture][region].actual_input_data[i], uniq_pairs[culture][region].actual_alternative_data[i]);
                }
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
                    std::cout << tm_to_str(init_data[culture].minimal_date[region][row].value()).value() << " " << init_data[culture].operation[row].value() << "\n";
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
    #pragma omp parallel for
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        #pragma omp parallel for
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
    //print_date(init_data);
}



