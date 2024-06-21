#pragma once

//Класс реализованный для хранения таблиц initial_data из PostgreSQL
class initial_data
{
public:
    int row_count;
    // Поля класса (атрибуты таблицы initial_data в PostgreSQL)
    std::vector<long long int> id;
    std::vector<std::optional<std::string>> t_material;
    std::vector<std::optional<std::string>> season;
    std::vector<std::optional<std::string>> operation;
    std::vector<std::optional<int>> input_operation;
    std::vector<std::optional<int>> deadline_input;
    std::vector<std::optional<std::tm>> regions[REGIONS_COUNT];
    std::vector<std::optional<int>> noinput_deadline;
    std::vector<std::optional<int>> alternative_input;
    std::vector<std::optional<int>> alternative_complete;

    initial_data(soci::rowset<soci::row> data)
    {
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            const soci::row& r = *it;
            id.push_back(r.get<long long int>(0));
            t_material.push_back(r.get_indicator(1) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(1));
            season.push_back(r.get_indicator(2) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(2));
            operation.push_back(r.get_indicator(3) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(3));

            // Работа с int и NULL
            input_operation.push_back(r.get_indicator(4) == soci::i_null ? std::optional<int>{} : r.get<int>(4));
            deadline_input.push_back(r.get_indicator(5) == soci::i_null ? std::optional<int>{} : r.get<int>(5));
            noinput_deadline.push_back(r.get_indicator(13) == soci::i_null ? std::optional<int>{} : r.get<int>(13));
            alternative_input.push_back(r.get_indicator(14) == soci::i_null ? std::optional<int>{} : r.get<int>(14));
            alternative_complete.push_back(r.get_indicator(15) == soci::i_null ? std::optional<int>{} : r.get<int>(15));

            // Работа с std::tm и NULL
            regions[0].push_back(r.get_indicator(6) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(6));
            regions[1].push_back(r.get_indicator(7) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(7));
            regions[2].push_back(r.get_indicator(8) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(8));
            regions[3].push_back(r.get_indicator(9) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(9));
            regions[4].push_back(r.get_indicator(10) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(10));
            regions[5].push_back(r.get_indicator(11) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(11));
            regions[6].push_back(r.get_indicator(12) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(12));
        }
        this->row_count = this->id.size();
    }

    initial_data()
    {
        this->row_count = 0;
    }

    void print()
    {
        for (size_t i = 0; i < id.size(); i++)
        {
            std::cout << "ID: " << id[i] << "\n";

            if (t_material[i].has_value())
                std::cout << "Material: " << t_material[i].value() << "\n";
            else
                std::cout << "Material: NULL\n";

            if (season[i].has_value())
                std::cout << "Season: " << season[i].value() << "\n";
            else
                std::cout << "Season: NULL\n";

            if (operation[i].has_value())
                std::cout << "Operation: " << operation[i].value() << "\n";
            else
                std::cout << "Operation: NULL\n";

            if (input_operation[i].has_value())
                std::cout << "Input Operation: " << input_operation[i].value() << "\n";
            else
                std::cout << "Input Operation: NULL\n";

            if (deadline_input[i].has_value())
                std::cout << "Deadline Input: " << deadline_input[i].value() << "\n";
            else
                std::cout << "Deadline Input: NULL\n";

            if (noinput_deadline[i].has_value())
                std::cout << "No Input Deadline: " << noinput_deadline[i].value() << "\n";
            else
                std::cout << "No Input Deadline: NULL\n";

            if (alternative_input[i].has_value())
                std::cout << "Alternative Input: " << alternative_input[i].value() << "\n";
            else
                std::cout << "Alternative Input: NULL\n";

            if (alternative_complete[i].has_value())
                std::cout << "Alternative Complete: " << alternative_complete[i].value() << "\n";
            else
                std::cout << "Alternative Complete: NULL\n";

            auto print_date = [](const std::optional<std::tm>& date, const std::string& label) 
                {
                if (date.has_value()) 
                {
                    std::tm tm_date = date.value();
                    char buffer[80];
                    strftime(buffer, 80, "%Y-%m-%d", &tm_date);
                    std::cout << label << ": " << buffer << "\n";
                }
                else 
                {
                    std::cout << label << ": NULL\n";
                }
                };

            print_date(regions[0][i], "Region BELS");
            print_date(regions[1][i], "Region BELC");
            print_date(regions[2][i], "Region BELK");
            print_date(regions[3][i], "Region TAMS");
            print_date(regions[4][i], "Region TAMN");
            print_date(regions[5][i], "Region OREL");
            print_date(regions[6][i], "Region PRIM");

            std::cout << "-----------------------------\n";
        }
    }
};