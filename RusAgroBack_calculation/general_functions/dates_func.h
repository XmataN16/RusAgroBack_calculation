#pragma once
//Функция создания объекта даты
std::tm create_date(int year, int month, int day) 
{
    std::tm date;
    std::memset(&date, 0, sizeof(date)); // Очищаем структуру (обнуляем поля)

    date.tm_year = year - 1900; // Год с 1900
    date.tm_mon = month - 1;    // Месяц (январь = 0, февраль = 1, ..., декабрь = 11)
    date.tm_mday = day;         // День месяца

    return date;
}

//Функция сравнения двух дат
std::optional<std::tm> max_date(const std::tm& date1, const std::tm& date2)
{
    std::time_t time1 = mktime(const_cast<std::tm*>(&date1));
    std::time_t time2 = mktime(const_cast<std::tm*>(&date2));

    if (difftime(time1, time2) > 0)
    {
        return date1;
    }
    else
    {
        return date2;
    }
}

bool max_date_bool(const std::tm& date1, const std::tm& date2)
{
    std::time_t time1 = std::mktime(const_cast<std::tm*>(&date1));
    std::time_t time2 = std::mktime(const_cast<std::tm*>(&date2));

    return std::difftime(time1, time2) > 0;
}

// Функция для сравнения двух std::optional<std::tm>
bool compare_optional_tm(const std::optional<std::tm>& opt_tm1, const std::optional<std::tm>& opt_tm2)
{
    if (!opt_tm1.has_value())
    {
        return false; // opt_tm2 больше, если opt_tm1 пуст
    }
    if (!opt_tm2.has_value())
    {
        return true; // opt_tm1 больше, если opt_tm2 пуст
    }

    std::tm tm1 = opt_tm1.value();
    std::tm tm2 = opt_tm2.value();

    // Преобразуем std::tm в time_t для сравнения
    std::time_t time1 = std::mktime(&tm1);
    std::time_t time2 = std::mktime(&tm2);

    return time1 < time2;
}

// Функция для нахождения максимальной даты среди std::optional<std::tm>
std::optional<std::tm> custom_max(const std::vector<std::optional<std::tm>>& dates)
{
    auto max_it = std::max_element(dates.begin(), dates.end(), compare_optional_tm);
    if (max_it != dates.end())
    {
        return *max_it;
    }
    else
    {
        return std::nullopt; // Если все элементы вектора пусты
    }
}

// Функция для нахождения минимальной даты среди std::optional<std::tm>
std::optional<std::tm> custom_min(const std::vector<std::optional<std::tm>>& dates)
{
    auto min_it = std::min_element(dates.begin(), dates.end(), compare_optional_tm);
    if (min_it != dates.end())
    {
        return *min_it;
    }
    else
    {
        return std::nullopt; // Если все элементы вектора пусты
    }
}

// Преобразование строки в std::tm
std::optional<std::tm> str_to_tm(const std::optional<std::string>& str)
{
    if (str.has_value())
    {
        std::optional<std::tm> tm = {};
        std::istringstream ss(str.value());
        ss >> std::get_time(&tm.value(), "%Y-%m-%d");
        return tm;
    }
    else
    {
        return std::nullopt;
    }
}

// Преобразование std::tm обратно в строку
std::optional<std::string> tm_to_str(const std::optional<std::tm>& tm)
{
    if (tm.has_value())
    {
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm.value());
        return std::string(buffer);
    }
    else
    {
        return std::nullopt;
    }
}

// Добавление дней к дате
std::optional<std::tm> add_days(const std::optional<std::tm>& tm, int days)
{
    std::optional<std::tm> result = tm;
    if (result.has_value())
    {
        time_t time = mktime(&result.value());
        time += days * 24 * 60 * 60; // добавление дней в секундах
        result = *localtime(&time);
        return result;
    }
    else
    {
        return std::nullopt;
    }

}