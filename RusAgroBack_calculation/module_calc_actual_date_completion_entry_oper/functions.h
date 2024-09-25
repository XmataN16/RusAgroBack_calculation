#pragma once
#include "class_DB/initial_data_class.h"
// Функции для вычисления фактических дат завершения операций и вычисления 10%

//Вычисление фактических дат завершения операции от вводной
void calc_actual_input_date_completion_entry_oper(initial_data init_data[], unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    std::string field;
    int input_number;
    int days;
    std::string current_operation;

    #pragma omp parallel for private(field, input_number, days, current_operation) shared(uniq_pairs, init_data)
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int record_uniq = 0; record_uniq < uniq_pairs[culture][region].row_count; record_uniq++)
            {
                field = uniq_pairs[culture][region].higher_tm[record_uniq].value();
                for (int record_init = 0; record_init < init_data[culture].row_count; record_init++)
                {
                    if (uniq_pairs[culture][region].material_order[record_uniq].value() == init_data[culture].operation[record_init].value() and uniq_pairs[culture][region].nzp_zp[record_uniq].value() == init_data[culture].season[record_init].value())
                    {

                        if (!init_data[culture].input_operation[record_init].has_value())
                        {
                            uniq_pairs[culture][region].actual_input_data[record_uniq] = std::nullopt;
                        }
                        else
                        {
                            input_number = init_data[culture].input_operation[record_init].value();
                            if (init_data[culture].deadline_input[record_init].has_value())
                            {
                                days = init_data[culture].deadline_input[record_init].value();
                            }
                            for (int el = 0; el < init_data[culture].row_count; el++)
                            {
                                if (input_number == init_data[culture].id[el])
                                {
                                    current_operation = init_data[culture].operation[el].value();
                                    for (int i = 0; i < uniq_pairs[culture][region].row_count; i++)
                                    {
                                        if (uniq_pairs[culture][region].material_order[i] == current_operation and field == uniq_pairs[culture][region].higher_tm[i])
                                        {
                                            if (uniq_pairs[culture][region].actual_data[i].has_value())
                                            {
                                                std::tm temp = uniq_pairs[culture][region].actual_data[i].value();
                                                uniq_pairs[culture][region].actual_input_data[record_uniq] = add_days(temp, days);
                                            }
                                            else
                                            {
                                                uniq_pairs[culture][region].actual_input_data[record_uniq] = std::nullopt;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//Вычисление фактических дат завершения операции от альтернативы
void calc_actual_alternative_date_completion_entry_oper(initial_data init_data[], unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    std::string field;
    int alternative_number;
    int days;
    std::string current_operation;

    #pragma omp parallel for private(field, alternative_number, days, current_operation) shared(uniq_pairs, init_data)
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int record_uniq = 0; record_uniq < uniq_pairs[culture][region].row_count; record_uniq++)
            {
                field = uniq_pairs[culture][region].higher_tm[record_uniq].value();
                for (int record_init = 0; record_init < init_data[culture].row_count; record_init++)
                {
                    if (uniq_pairs[culture][region].material_order[record_uniq].value() == init_data[culture].operation[record_init].value() and uniq_pairs[culture][region].nzp_zp[record_uniq].value() == init_data[culture].season[record_init].value())
                    {
                        if (!init_data[culture].alternative_input[record_init].has_value())
                        {
                            uniq_pairs[culture][region].actual_alternative_data[record_uniq] = std::nullopt;
                        }
                        else
                        {
                            alternative_number = init_data[culture].alternative_input[record_init].value();
                            days = init_data[culture].alternative_complete[record_init].value();
                            for (int el = 0; el < init_data[culture].row_count; el++)
                            {
                                if (alternative_number == init_data[culture].id[el])
                                {
                                    current_operation = init_data[culture].operation[el].value();
                                    for (int i = 0; i < uniq_pairs[culture][region].row_count; i++)
                                    {
                                        if (uniq_pairs[culture][region].material_order[i] == current_operation and field == uniq_pairs[culture][region].higher_tm[i])
                                        {
                                            if (uniq_pairs[culture][region].actual_data[i].has_value())
                                            {
                                                std::tm temp = uniq_pairs[culture][region].actual_data[i].value();
                                                uniq_pairs[culture][region].actual_alternative_data[record_uniq] = add_days(temp, days);
                                            }
                                            else
                                            {
                                                uniq_pairs[culture][region].actual_alternative_data[record_uniq] = std::nullopt;
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//Функция для расчета первой даты при которой было выполнено 10% от actual_volume
void calc_ten_percent(data data_shbn[CULTURES_COUNT][REGIONS_COUNT], unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    const double START_VALUE = 10;
    double planned_item_value;
    double count = 0;

    #pragma omp parallel for private(count, planned_item_value)
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int el = 0; el < uniq_pairs[culture][region].row_count; el++)
            {
                count = 0;
                for (int item = 0; item < data_shbn[culture][region].row_count; item++)
                {
                    if (uniq_pairs[culture][region].higher_tm[el].value() == data_shbn[culture][region].higher_tm[item].value() and uniq_pairs[culture][region].material_order[el].value() == data_shbn[culture][region].material_order[item].value() and uniq_pairs[culture][region].nzp_zp[el].value() == data_shbn[culture][region].nzp_zp[item].value())
                    {
                        planned_item_value = data_shbn[culture][region].planned_volume[item].value();
                        count += data_shbn[culture][region].actual_volume[item].value();
                        if (count >= ((planned_item_value / 100.0) * START_VALUE))
                        {
                            uniq_pairs[culture][region].ten_percent[el] = data_shbn[culture][region].calendar_day[item].value();
                        }
                        else
                        {
                            uniq_pairs[culture][region].ten_percent[el] = std::nullopt;
                        }
                    }
                }
            }
        }
    }
}

//Функция запускающая вычисление фактических дат и 10%
void summarize(soci::session& sql, initial_data init_data[], data data_shbn[CULTURES_COUNT][REGIONS_COUNT], unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    get_unique_higher_tm_material_order(sql, data_shbn, uniq_pairs);
    calc_actual_input_date_completion_entry_oper(init_data ,uniq_pairs);
    calc_actual_alternative_date_completion_entry_oper(init_data, uniq_pairs);
    calc_ten_percent(data_shbn, uniq_pairs);
}