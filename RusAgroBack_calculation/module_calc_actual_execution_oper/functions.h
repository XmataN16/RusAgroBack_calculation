#pragma once
#include "class_DB/data_class.h"

//Промежуточный класс для работы с фактическими датами завершения и 10%
class unique_pairs
{
public:
    int row_count;
    std::vector<std::optional<std::string>> higher_tm;
    std::vector<std::optional<std::string>> material_order;
    std::vector<std::optional<std::string>> culture;
    std::vector<std::optional<std::string>> business_dir;
    std::vector<std::optional<std::string>> nzp_zp;
    std::vector<std::optional<std::string>> pu;
    std::vector<std::optional<int>> order;
    std::vector<std::optional<bool>> is_completed;
    std::vector<std::optional<std::tm>> minimal_planned_date;
    std::vector<std::optional<std::tm>> actual_data;
    std::vector<std::optional<std::tm>> actual_input_data;
    std::vector<std::optional<std::tm>> actual_alternative_data;
    std::vector<std::optional<std::tm>> ten_percent;
    std::vector<std::optional<std::tm>> minimal_date;
    std::vector<std::optional<std::string>> status;
    std::vector<std::optional<std::string>> is_actual;
    

    unique_pairs(soci::rowset<soci::row> data)
    {
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            const soci::row& r = *it;
            higher_tm.push_back(r.get_indicator(0) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(0));
            material_order.push_back(r.get_indicator(1) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(1));
            culture.push_back(r.get_indicator(2) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(2));
            business_dir.push_back(r.get_indicator(3) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(3));
            nzp_zp.push_back(r.get_indicator(4) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(4));
            pu.push_back(r.get_indicator(5) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(5));
            order.push_back(std::nullopt);
            is_completed.push_back(std::nullopt);
            actual_data.push_back(std::nullopt);
            minimal_planned_date.push_back(std::nullopt);
            actual_input_data.push_back(std::nullopt);
            actual_alternative_data.push_back(std::nullopt);
            ten_percent.push_back(std::nullopt);
            minimal_date.push_back(std::nullopt);
            status.push_back(std::nullopt);
            is_actual.push_back(std::nullopt);
        }
        this->row_count = higher_tm.size();
    }

    unique_pairs()
    {
        this->row_count = 0;
    }

    void print()
    {
        std::cout << higher_tm.size() << std::endl;
        for (size_t i = 0; i < this->row_count; i++)
        {
                if (higher_tm[i].has_value())
                    std::cout << "higher_tm: " << higher_tm[i].value() << "\n";
                else
                    std::cout << "higher_tm: NULL\n";

                if (material_order[i].has_value())
                    std::cout << "material_order: " << material_order[i].value() << "\n";
                else
                    std::cout << "material_order: NULL\n";

                if (culture[i].has_value())
                    std::cout << "culture: " << culture[i].value() << "\n";
                else
                    std::cout << "culture: NULL\n";

                if (business_dir[i].has_value())
                    std::cout << "business_dir: " << business_dir[i].value() << "\n";
                else
                    std::cout << "business_dir: NULL\n";

                if (nzp_zp[i].has_value())
                    std::cout << "nzp_zp: " << nzp_zp[i].value() << "\n";
                else
                    std::cout << "nzp_zp: NULL\n";

                if (pu[i].has_value())
                    std::cout << "PU: " << pu[i].value() << "\n";
                else
                    std::cout << "PU: NULL\n";

                if (order[i].has_value())
                    std::cout << "order: " << order[i].value() << "\n";
                else
                    std::cout << "order: NULL\n";

                if (is_completed[i].has_value())
                {
                    if (is_completed[i].value())
                    {
                        std::cout << "is_completed: true\n";
                    }
                    else
                    {
                        std::cout << "is_completed: false\n";
                    }
                }
                else
                    std::cout << "is_completed: NULL\n";

                if (actual_data[i].has_value())
                {
                    std::tm tm_date = actual_data[i].value();
                    char buffer[80];
                    strftime(buffer, 80, "%Y-%m-%d", &tm_date);
                    std::cout << "actual_data: " << buffer << "\n";
                }
                else
                    std::cout << "actual_data: NULL\n";

                if (minimal_planned_date[i].has_value())
                {
                    std::tm tm_date = minimal_planned_date[i].value();
                    char buffer[80];
                    strftime(buffer, 80, "%Y-%m-%d", &tm_date);
                    std::cout << "minimal_planned_date: " << buffer << "\n";
                }
                else
                    std::cout << "minimal_planned_date: NULL\n";

                if (actual_input_data[i].has_value())
                {
                    std::tm tm_date = actual_input_data[i].value();
                    char buffer[80];
                    strftime(buffer, 80, "%Y-%m-%d", &tm_date);
                    std::cout << "actual_input_data: " << buffer << "\n";
                }
                else
                    std::cout << "actual_input_data: NULL\n";

                if (actual_alternative_data[i].has_value())
                {
                    std::tm tm_date = actual_alternative_data[i].value();
                    char buffer[80];
                    strftime(buffer, 80, "%Y-%m-%d", &tm_date);
                    std::cout << "actual_alternative_data: " << buffer << "\n";
                }
                else
                    std::cout << "actual_alternative_data: NULL\n";

                if (ten_percent[i].has_value())
                {
                    std::tm tm_date = ten_percent[i].value();
                    char buffer[80];
                    strftime(buffer, 80, "%Y-%m-%d", &tm_date);
                    std::cout << "ten_percent: " << buffer << "\n";
                }
                else
                    std::cout << "ten_percent: NULL\n";

                if (minimal_date[i].has_value())
                {
                    std::tm tm_date = minimal_date[i].value();
                    char buffer[80];
                    strftime(buffer, 80, "%Y-%m-%d", &tm_date);
                    std::cout << "minimal_date: " << buffer << "\n";
                }
                else
                    std::cout << "minimal_date: NULL\n";

                if (status[i].has_value())
                    std::cout << "status: " << status[i].value() << "\n";
                else
                    std::cout << "status: NULL\n";

                if (is_actual[i].has_value())
                    std::cout << "is_actual: " << is_actual[i].value() << "\n";
                else
                    std::cout << "is_actual: NULL\n";

                std::cout << "-----------------------------\n";
            }
    }

    nlohmann::json to_json() const 
    {
        nlohmann::json j;

        for (int i = 0; i < row_count; ++i) 
        {
            nlohmann::json row;

            row[u8"higher_tm"] = higher_tm[i] ? *higher_tm[i] : nullptr;
            row[u8"material_order"] = material_order[i] ? *material_order[i] : nullptr;
            row[u8"culture"] = culture[i] ? *culture[i] : nullptr;
            row[u8"business_dir"] = business_dir[i] ? *business_dir[i] : nullptr;
            row[u8"nzp_zp"] = nzp_zp[i] ? *nzp_zp[i] : nullptr;
            row[u8"pu"] = pu[i] ? *pu[i] : nullptr;
            if (order[i].has_value())
            {
                row[u8"order"] = order[i].value();
            }
            else
            {
                row[u8"order"] = nullptr;
            }
            if (tm_to_str(actual_data[i]).has_value())
            {
                row[u8"actual_data"] = tm_to_str(actual_data[i]).value();
            }
            else
            {
                row[u8"actual_data"] = nullptr;
            }
            if (tm_to_str(minimal_date[i]).has_value())
            {
                row[u8"minimal_date"] = tm_to_str(minimal_date[i]).value();
            }
            else
            {
                row[u8"minimal_date"] = nullptr;
            }
            if (status[i].has_value())
            {
                row[u8"status"] = status[i].value();
            }
            else
            {
                row[u8"status"] = nullptr;
            }
            if (is_actual[i].has_value())
            {
                row[u8"is_actual"] = is_actual[i].value();
            }
            else
            {
                row[u8"is_actual"] = nullptr;
            }

            j.push_back(row);
        }

        return j;
    }

    void to_json_file(const std::string& filename) const
    {
        nlohmann::json j = this->to_json();
        std::ofstream file(filename);
        file << j.dump(4);  // 4 is the indentation level
    }

    
};

nlohmann::json to_json_all(unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    nlohmann::json j;
    //j["row_count"] = row_count;

    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            for (int i = 0; i < uniq_pairs[culture][region].row_count; ++i)
            {
                nlohmann::json row;

                row[u8"higher_tm"] = uniq_pairs[culture][region].higher_tm[i] ? *uniq_pairs[culture][region].higher_tm[i] : nullptr;
                row[u8"material_order"] = uniq_pairs[culture][region].material_order[i] ? *uniq_pairs[culture][region].material_order[i] : nullptr;
                row[u8"culture"] = uniq_pairs[culture][region].culture[i] ? *uniq_pairs[culture][region].culture[i] : nullptr;
                row[u8"business_dir"] = uniq_pairs[culture][region].business_dir[i] ? *uniq_pairs[culture][region].business_dir[i] : nullptr;
                row[u8"nzp_zp"] = uniq_pairs[culture][region].nzp_zp[i] ? *uniq_pairs[culture][region].nzp_zp[i] : nullptr;
                row[u8"pu"] = uniq_pairs[culture][region].pu[i] ? *uniq_pairs[culture][region].pu[i] : nullptr;
                if (uniq_pairs[culture][region].order[i].has_value())
                {
                    row[u8"order"] = uniq_pairs[culture][region].order[i].value();
                }
                else
                {
                    row[u8"order"] = nullptr;
                }
                if (tm_to_str(uniq_pairs[culture][region].actual_data[i]).has_value())
                {
                    row[u8"actual_data"] = tm_to_str(uniq_pairs[culture][region].actual_data[i]).value();
                }
                else
                {
                    row[u8"actual_data"] = nullptr;
                }
                if (tm_to_str(uniq_pairs[culture][region].minimal_date[i]).has_value())
                {
                    row[u8"minimal_date"] = tm_to_str(uniq_pairs[culture][region].minimal_date[i]).value();
                }
                else
                {
                    row[u8"minimal_date"] = nullptr;
                }
                if (uniq_pairs[culture][region].status[i].has_value())
                {
                    row[u8"status"] = uniq_pairs[culture][region].status[i].value();
                }
                else
                {
                    row[u8"status"] = nullptr;
                }
                if (uniq_pairs[culture][region].is_actual[i].has_value())
                {
                    row[u8"is_actual"] = uniq_pairs[culture][region].is_actual[i].value();
                }
                else
                {
                    row[u8"is_actual"] = nullptr;
                }

                j.push_back(row);
            }
        }
    }
    return j;
}

void to_json_all_file(unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT], const std::string& filename)
{
    nlohmann::json j = to_json_all(uniq_pairs);
    std::ofstream file(filename);
    file << j.dump(4);  // 4 is the indentation level
}

void save_jsons_uniq_pairs(unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    #pragma omp parallel for collapse(2)
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            uniq_pairs[culture][region].to_json_file("json/" + CULTURES[culture] + "_" + REGIONS[region] + ".json");
        }
    }
}

// Cчитывание таблицы уникальных пар по каждой культуре в массив в PostgreSQL
void read_table_unique_pairs(soci::session& sql, unique_pairs uniq_pairs[][REGIONS_COUNT])
{
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            soci::rowset<soci::row> rs = (sql.prepare << "SELECT DISTINCT higher_tm, material_order, culture, business_dir, nzp_zp, pu FROM platform_shbn_data WHERE culture = '" << CULTURES_RUS[culture] << "' and business_dir = '" << REGIONS_RUS[region] << "'");
            uniq_pairs[culture][region] = unique_pairs(rs);
        }
    }
}

// Функция составления уникальной выборки из талбицы DATA в PostgreSQL
void get_unique_higher_tm_material_order(soci::session& sql, data data_shbn[CULTURES_COUNT][REGIONS_COUNT], unique_pairs uniq_pairs[CULTURES_COUNT][REGIONS_COUNT])
{
    for (int culture = 0; culture < CULTURES_COUNT; culture++)
    {
        for (int region = 0; region < REGIONS_COUNT; region++)
        {
            std::vector<std::optional<bool>> temp_is_completed(uniq_pairs[culture][region].row_count);
            std::vector<std::optional<std::tm>> temp_actual_data(uniq_pairs[culture][region].row_count);

            //#pragma omp parallel for
            for (int pair = 0; pair < uniq_pairs[culture][region].row_count; pair++)
            {
                float sum_planned_volume = 0.0f;
                float sum_actual_volume = 0.0f;
                std::tm max_date = create_date(1999, 1, 1);

                // Параллелизация внешнего цикла по парам
                #pragma omp parallel for
                for (int item = 0; item < data_shbn[culture][region].row_count; item++)
                {
                    if (data_shbn[culture][region].higher_tm[item] == uniq_pairs[culture][region].higher_tm[pair] &&
                        data_shbn[culture][region].material_order[item] == uniq_pairs[culture][region].material_order[pair] &&
                        data_shbn[culture][region].culture[item] == uniq_pairs[culture][region].culture[pair])
                    {
                        sum_planned_volume += data_shbn[culture][region].planned_volume[item].value();
                        sum_actual_volume += data_shbn[culture][region].actual_volume[item].value();
                        if (max_date_bool(data_shbn[culture][region].calendar_day[item].value(), max_date))
                        {
                            max_date = data_shbn[culture][region].calendar_day[item].value();
                        }
                    }
                }
                temp_is_completed[pair] = ((sum_planned_volume * 1.0f) <= sum_actual_volume && sum_planned_volume != 0);

                if (temp_is_completed[pair] == 0)
                {
                    temp_actual_data[pair] = std::nullopt;
                }
                else
                {
                    temp_actual_data[pair] = max_date;
                }
            }

            uniq_pairs[culture][region].is_completed = std::move(temp_is_completed);
            uniq_pairs[culture][region].actual_data = std::move(temp_actual_data);
        }
    }
}

