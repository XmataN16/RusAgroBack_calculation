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
    std::vector<std::optional<bool>> is_completed;
    std::vector<std::optional<std::tm>> actual_data;
    std::vector<std::optional<std::tm>> actual_input_data;
    std::vector<std::optional<std::tm>> actual_alternative_data;
    std::vector<std::optional<std::tm>> ten_percent;

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
            is_completed.push_back(std::nullopt);
            actual_data.push_back(std::nullopt);
            actual_input_data.push_back(std::nullopt);
            actual_alternative_data.push_back(std::nullopt);
            ten_percent.push_back(std::nullopt);
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
        for (size_t i = 0; i < 30; i++)
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

            std::cout << "-----------------------------\n";
        }
    }
};

// Функция составления уникальной выборки из талбицы DATA в PostgreSQL
unique_pairs get_unique_higher_tm_material_order(soci::session& sql, data data_shbn)
{
    soci::rowset<soci::row> rs = (sql.prepare << "SELECT DISTINCT higher_tm, material_order, culture, business_dir, nzp_zp FROM platform_shbn_data");
    unique_pairs uniq_pairs(rs);

    std::vector<std::optional<bool>> temp_is_completed(uniq_pairs.row_count);
    std::vector<std::optional<std::tm>> temp_actual_data(uniq_pairs.row_count);

    #pragma omp parallel for
    for (int pair = 0; pair < uniq_pairs.row_count; pair++)
    {
        float sum_planned_volume = 0;
        float sum_actual_volume = 0;
        std::tm max_date = create_date(1999, 1, 1);

        for (int item = 0; item < data_shbn.row_count; item++)
        {
            if (data_shbn.higher_tm[item] == uniq_pairs.higher_tm[pair] &&
                data_shbn.material_order[item] == uniq_pairs.material_order[pair] &&
                data_shbn.culture[item] == uniq_pairs.culture[pair])
            {
                sum_planned_volume += data_shbn.planned_volume[item].value();
                sum_actual_volume += data_shbn.actual_volume[item].value();
                if (max_date_bool(data_shbn.calendar_day[item].value(), max_date))
                {
                    max_date = data_shbn.calendar_day[item].value();
                }
            }
        }
        temp_is_completed[pair] = (sum_planned_volume == sum_actual_volume && sum_actual_volume != 0);
        temp_actual_data[pair] = max_date;
    }

    uniq_pairs.is_completed = std::move(temp_is_completed);
    uniq_pairs.actual_data = std::move(temp_actual_data);

    return uniq_pairs;
}