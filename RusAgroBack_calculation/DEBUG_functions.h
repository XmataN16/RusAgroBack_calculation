#pragma once
// Вывод числа объектов по каждому статусу
void DEBUG_get_count_rows_on_status(unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    float sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0;
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int row = 0; row < uniq_pairs[culture][region].status.size(); row++)
            {
                if (uniq_pairs[culture][region].status[row].value() == u8"Операция не отслеживается")
                {
                    sum1++;
                }
                else if (uniq_pairs[culture][region].status[row].value() == u8"Не завершено")
                {
                    sum2++;
                }
                else if (uniq_pairs[culture][region].status[row].value() == u8"Просрочено")
                {
                    sum3++;
                }
                else if (uniq_pairs[culture][region].status[row].value() == u8"Выполнено в срок")
                {
                    sum4++;
                }
                else if (uniq_pairs[culture][region].status[row].value() == u8"Выполнено не в срок")
                {
                    sum5++;
                }
            }
        }
    }
    float all_count = sum1 + sum2 + sum3 + sum4 + sum5;
    std::cout << u8"Число объектов со следующими статусами:" << std::endl;
    std::cout << u8"Операция не отслеживается: " << sum1 << u8" или " << float(sum1 / all_count) * 100.0 << "%" << std::endl;
    std::cout << u8"Не завершено: " << sum2 << u8" или " << float(sum2 / all_count) * 100.0 << "%" << std::endl;
    std::cout << u8"Просрочено: " << sum3 << u8" или " << float(sum3 / all_count) * 100.0 << "%" << std::endl;
    std::cout << u8"Выполнено в срок: " << sum4 << u8" или " << float(sum4 / all_count) * 100.0 << "%" << std::endl;
    std::cout << u8"Выполнено не в срок: " << sum5 << u8" или " << float(sum5 / all_count) * 100.0 << "%" << std::endl;
}

// Вывод статуса по заданным операции и полю
void DEBUG_get_status_count_rows_on_material_order_and_higher_tm(unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT], std::string material_order, std::string higher_tm)
{
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int row = 0; row < uniq_pairs[culture][region].status.size(); row++)
            {
                if (uniq_pairs[culture][region].material_order[row].value() == material_order and uniq_pairs[culture][region].higher_tm[row].value() == higher_tm)
                {
                    std::cout << u8"Операция: " << material_order << u8" , поле: " << higher_tm << u8" , статус: " << uniq_pairs[culture][region].status[row].value() << std::endl;
                }
            }
        }
    }
}

// Вывод записей по заданному операции и полю из DATA
void DEBUG_data_get_count_rows_on_material_order_and_higher_tm(data data_shbn[CULTURES_COUNT][REGIONS_COUNT], std::string material_order, std::string higher_tm)
{
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int row = 0; row < data_shbn[culture][region].id.size(); row++)
            {
                if (data_shbn[culture][region].material_order[row].value() == material_order and data_shbn[culture][region].higher_tm[row].value() == higher_tm)
                {
                    data_shbn[culture][region].print_index(row);
                }
            }
        }
    }
}

// Вывод записей по заданному операции и полю из uniq_pairs
void DEBUG_uniq_pairs_get_count_rows_on_material_order_and_higher_tm(unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT], std::string material_order, std::string higher_tm)
{
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int row = 0; row < uniq_pairs[culture][region].row_count; row++)
            {
                if (uniq_pairs[culture][region].material_order[row].value() == material_order and uniq_pairs[culture][region].higher_tm[row].value() == higher_tm)
                {
                    uniq_pairs[culture][region].print_index(row);
                }
            }
        }
    }
}