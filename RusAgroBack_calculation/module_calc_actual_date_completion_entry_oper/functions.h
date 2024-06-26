#pragma once
#include "class_DB/initial_data_class.h"
// Функции для вычисления фактических дат завершения операций и вычисления 10%

//Вычисление фактических дат завершения операции от вводной
unique_pairs calc_actual_input_date_completion_entry_oper(unique_pairs uniq_pairs, initial_data init_data[])
{
    std::string field;
    int input_number;
    int days;
    std::string current_operation;

    #pragma omp parallel for private(field, input_number, days, current_operation) shared(uniq_pairs, init_data)
    for (int record_uniq = 0; record_uniq < uniq_pairs.row_count; record_uniq++)
    {
        field = uniq_pairs.higher_tm[record_uniq].value();
        for (int record_init = 0; record_init < init_data[12].row_count; record_init++)
        {
            if (uniq_pairs.material_order[record_uniq].value() == init_data[12].operation[record_init].value() and uniq_pairs.nzp_zp[record_uniq].value() == init_data[12].season[record_init].value())
            {
                
                if (!init_data[12].input_operation[record_init].has_value())
                {
                    uniq_pairs.actual_input_data[record_uniq] = std::nullopt;
                }
                else
                {
                    input_number = init_data[12].input_operation[record_init].value();
                    days = init_data[12].deadline_input[record_init].value();
                    for (int el = 0; el < init_data[12].row_count; el++)
                    {
                        if (input_number == init_data[12].id[el])
                        {
                            current_operation = init_data[12].operation[el].value();
                            for (int i = 0; i < uniq_pairs.row_count; i++)
                            {
                                if (uniq_pairs.material_order[i] == current_operation and field == uniq_pairs.higher_tm[i])
                                {
                                    std::tm temp = uniq_pairs.actual_data[i].value();
                                    uniq_pairs.actual_input_data[record_uniq] = add_days(temp, days);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return uniq_pairs;
}

//Вычисление фактических дат завершения операции от альтернативы
unique_pairs calc_actual_alternative_date_completion_entry_oper(unique_pairs uniq_pairs, initial_data init_data[])
{
    std::string field;
    int alternative_number;
    int days;
    std::string current_operation;

    #pragma omp parallel for private(field, alternative_number, days, current_operation) shared(uniq_pairs, init_data)
    for (int record_uniq = 0; record_uniq < uniq_pairs.row_count; record_uniq++)
    {
        field = uniq_pairs.higher_tm[record_uniq].value();
        for (int record_init = 0; record_init < init_data[12].row_count; record_init++)
        {
            if (uniq_pairs.material_order[record_uniq].value() == init_data[12].operation[record_init].value() and uniq_pairs.nzp_zp[record_uniq].value() == init_data[12].season[record_init].value())
            {
                if (!init_data[12].alternative_input[record_init].has_value())
                {
                    uniq_pairs.actual_alternative_data[record_uniq] = std::nullopt;
                }
                else
                {
                    alternative_number = init_data[12].alternative_input[record_init].value();
                    days = init_data[12].alternative_complete[record_init].value();
                    for (int el = 0; el < init_data[12].row_count; el++)
                    {
                        if (alternative_number == init_data[12].id[el])
                        {
                            current_operation = init_data[12].operation[el].value();
                            for (int i = 0; i < uniq_pairs.row_count; i++)
                            {
                                if (uniq_pairs.material_order[i] == current_operation and field == uniq_pairs.higher_tm[i])
                                {
                                    std::tm temp = uniq_pairs.actual_data[i].value();
                                    uniq_pairs.actual_alternative_data[record_uniq] = add_days(temp, days);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return uniq_pairs;
}

//Функция для расчета первой даты при которой было выполнено 10% от actual_volume
unique_pairs calc_ten_percent(unique_pairs uniq_pairs, data data_shbn)
{
    const double START_VALUE = 10;
    double planned_item_value;
    double count = 0;

    #pragma omp parallel for private(count, planned_item_value)
    for (int el = 0; el < uniq_pairs.row_count; el++)
    {
        count = 0;
        for (int item = 0; item < data_shbn.row_count; item++)
        {
            if (uniq_pairs.higher_tm[el].value() == data_shbn.higher_tm[item].value() and uniq_pairs.material_order[el].value() == data_shbn.material_order[item].value() and uniq_pairs.nzp_zp[el].value() == data_shbn.nzp_zp[item].value())
            {
                planned_item_value = data_shbn.planned_volume[item].value();
                count += data_shbn.actual_volume[item].value();
                if (count >= ((planned_item_value / 100.0) * START_VALUE))
                {
                    uniq_pairs.ten_percent[el] = data_shbn.calendar_day[item].value();
                }
                else
                {
                    uniq_pairs.ten_percent[el] = std::nullopt;
                }
            }
        }
    }
    return uniq_pairs;
}

//Функция запускающая вычисление фактических дат и 10%
unique_pairs summarize(soci::session& sql, initial_data init_data[], unique_pairs uniq_pairs, data data_shbn)
{
    uniq_pairs = get_unique_higher_tm_material_order(sql, data_shbn);
    uniq_pairs = calc_actual_input_date_completion_entry_oper(uniq_pairs, init_data);
    uniq_pairs = calc_actual_alternative_date_completion_entry_oper(uniq_pairs, init_data);
    uniq_pairs = calc_ten_percent(uniq_pairs, data_shbn);
    return uniq_pairs;
}