#pragma once
//Класс реализованный для хранения таблицы Data из PostgreSQL
class data
{
public:
    int row_count;
    // Поля класса (атрибуты таблицы data в PostgreSQL)
    std::vector<long long int> id;
    std::vector<std::optional<std::string>> culture;
    std::vector<std::optional<std::string>> nzp_zp;
    std::vector<std::optional<std::string>> business_dir;
    std::vector<std::optional<std::tm>> calendar_day;
    std::vector<std::optional<std::string>> higher_tm;
    std::vector<std::optional<std::string>> material_order;
    std::vector<std::optional<float>> planned_volume;
    std::vector<std::optional<float>> actual_volume;
    std::vector<std::optional<std::string>> pu;

    data(soci::rowset<soci::row> data)
    {
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            const soci::row& r = *it;
            id.push_back(r.get<long long int>(0));
            culture.push_back(r.get_indicator(1) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(1));
            nzp_zp.push_back(r.get_indicator(2) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(2));
            business_dir.push_back(r.get_indicator(3) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(3));
            calendar_day.push_back(r.get_indicator(4) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(4));
            higher_tm.push_back(r.get_indicator(5) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(5));
            material_order.push_back(r.get_indicator(6) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(6));
            planned_volume.push_back(r.get_indicator(7) == soci::i_null ? std::optional<double>{} : r.get<double>(7));
            actual_volume.push_back(r.get_indicator(8) == soci::i_null ? std::optional<double>{} : r.get<double>(8));
            pu.push_back(r.get_indicator(9) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(9));
        }
        this->row_count = this->id.size();
    }

    data()
    {
        this->row_count = 0;
    }

    void print()
    {
        std::cout << id.size() << std::endl;
        for (size_t i = 0; i < id.size(); i++)
        {
            std::cout << "ID: " << id[i] << "\n";

            if (culture[i].has_value())
                std::cout << "Culture: " << culture[i].value() << "\n";
            else
                std::cout << "Culture: NULL\n";

            if (nzp_zp[i].has_value())
                std::cout << "NZP_ZP: " << nzp_zp[i].value() << "\n";
            else
                std::cout << "NZP_ZP: NULL\n";

            if (business_dir[i].has_value())
                std::cout << "Business Direction: " << business_dir[i].value() << "\n";
            else
                std::cout << "Business Direction: NULL\n";

            if (calendar_day[i].has_value())
            {
                std::tm tm_date = calendar_day[i].value();
                char buffer[80];
                strftime(buffer, 80, "%Y-%m-%d", &tm_date);
                std::cout << "Calendar Day: " << buffer << "\n";
            }
            else
                std::cout << "Calendar Day: NULL\n";

            if (higher_tm[i].has_value())
                std::cout << "Higher TM: " << higher_tm[i].value() << "\n";
            else
                std::cout << "Higher TM: NULL\n";

            if (material_order[i].has_value())
                std::cout << "Material Order: " << material_order[i].value() << "\n";
            else
                std::cout << "Material Order: NULL\n";

            if (planned_volume[i].has_value())
                std::cout << "Planned Volume: " << planned_volume[i].value() << "\n";
            else
                std::cout << "Planned Volume: NULL\n";

            if (actual_volume[i].has_value())
                std::cout << "Actual Volume: " << actual_volume[i].value() << "\n";
            else
                std::cout << "Actual Volume: NULL\n";

            std::cout << "-----------------------------\n";
            if (pu[i].has_value())
                std::cout << "PU: " << pu[i].value() << "\n";
            else
                std::cout << "PU: NULL\n";
        }
    }
};

// Cчитывание таблицы по каждой культуре в массив в PostgreSQL
void read_table_data(soci::session& sql, data data_shbn[][REGIONS_COUNT])
{
    for (int i = 0; i < CULTURES_COUNT; i++)
    {
        for (int j = 0; j < REGIONS_COUNT; j++)
        {
            soci::rowset<soci::row> rs = (sql.prepare << "SELECT * FROM platform_shbn_data WHERE culture = '" << CULTURES_RUS[i] << "' and business_dir = '" << REGIONS_RUS[j] << "'");
            data_shbn[i][j] = data(rs);
        }
    }
}