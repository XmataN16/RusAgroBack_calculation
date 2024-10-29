#pragma once
// ����� ����� �������� �� ������� �������
void DEBUG_get_count_rows_on_status(unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    float sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0;
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int row = 0; row < uniq_pairs[culture][region].status.size(); row++)
            {
                if (uniq_pairs[culture][region].status[row].value() == u8"�������� �� �������������")
                {
                    sum1++;
                }
                else if (uniq_pairs[culture][region].status[row].value() == u8"�� ���������")
                {
                    sum2++;
                }
                else if (uniq_pairs[culture][region].status[row].value() == u8"����������")
                {
                    sum3++;
                }
                else if (uniq_pairs[culture][region].status[row].value() == u8"��������� � ����")
                {
                    sum4++;
                }
                else if (uniq_pairs[culture][region].status[row].value() == u8"��������� �� � ����")
                {
                    sum5++;
                }
            }
        }
    }
    float all_count = sum1 + sum2 + sum3 + sum4 + sum5;
    std::cout << u8"����� �������� �� ���������� ���������:" << std::endl;
    std::cout << u8"�������� �� �������������: " << sum1 << u8" ��� " << float(sum1 / all_count) * 100.0 << "%" << std::endl;
    std::cout << u8"�� ���������: " << sum2 << u8" ��� " << float(sum2 / all_count) * 100.0 << "%" << std::endl;
    std::cout << u8"����������: " << sum3 << u8" ��� " << float(sum3 / all_count) * 100.0 << "%" << std::endl;
    std::cout << u8"��������� � ����: " << sum4 << u8" ��� " << float(sum4 / all_count) * 100.0 << "%" << std::endl;
    std::cout << u8"��������� �� � ����: " << sum5 << u8" ��� " << float(sum5 / all_count) * 100.0 << "%" << std::endl;
}

// ����� ������� �� �������� �������� � ����
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
                    std::cout << u8"��������: " << material_order << u8" , ����: " << higher_tm << u8" , ������: " << uniq_pairs[culture][region].status[row].value() << std::endl;
                }
            }
        }
    }
}

// ����� ������� �� ��������� �������� � ���� �� DATA
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

// ����� ������� �� ��������� �������� � ���� �� uniq_pairs
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