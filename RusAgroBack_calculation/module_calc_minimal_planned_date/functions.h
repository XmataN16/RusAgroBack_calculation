#pragma once
#include "initial_data_class.h"

// Функции для расчета минимальных плановых дат

//считываем таблицы по каждой культуре в массив в PostgreSQL
void read_tables(soci::session& sql, initial_data init_data[])
{
    for (int i = 0; i < CULTURES_COUNT; i++)
    {
        soci::rowset<soci::row> rs = (sql.prepare << "SELECT * FROM platform_shbn_initial_data_" << CULTURES[i] << " ORDER BY id");
        init_data[i] = initial_data(rs);
    }
}

void calc_minimal_planned_date(soci::session& sql)
{
    initial_data init_data[CULTURES_COUNT];
    read_tables(sql, init_data);
    init_data[6].print();
}

void calc_planned_date()
{




}