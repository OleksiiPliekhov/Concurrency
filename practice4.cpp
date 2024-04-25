#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

class Student {
public:
    std::string name;
    int score;

    Student(const std::string &_name, int _score) : name(_name), score(_score) {}
};



void maxScore(std::vector<Student> students, int &maxScore) {
    maxScore = std::max_element(students.begin(), students.end(),
                            [](const Student &a, const Student &b) -> bool {
                                return a.score < b.score;
                            })->score;
}


class Month {
public:
    std::string name;
    double average_temperature;

    Month(const std::string &_name, double _average_temperature)
    : name(_name), average_temperature(_average_temperature) {}

    bool operator<(const Month &other) const {
        return average_temperature < other.average_temperature;
    }
};


std::vector<Month> multithreadedSortMonths(std::vector<Month>& months, int num_chunks) {
    if (num_chunks <= 0 || num_chunks > months.size()) {
        throw std::invalid_argument("Invalid number of chunks");
    }

    const int chunk_size = (months.size() + num_chunks - 1) / num_chunks;

    std::vector<std::vector<Month>> chunks(num_chunks);
    for (int i = 0, chunk_index = 0; i < months.size(); ++i, chunk_index = (i + chunk_size - 1) / chunk_size) {
        chunks[chunk_index].push_back(months[i]);
    }

    std::vector<std::thread> threads(num_chunks);
    for (int i = 0; i < num_chunks; ++i) {
        threads[i] = std::thread([&chunks, i]() {
            std::sort(chunks[i].begin(), chunks[i].end(), [](Month a, Month b) -> bool { return a.average_temperature < b.average_temperature; });
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    months.clear();
    std::sort(chunks.begin(), chunks.end(), [](const std::vector<Month>& a, const std::vector<Month>& b)
        { return a[0].average_temperature < b[0].average_temperature; });
    for (const auto& chunk : chunks) {
        for (const Month& month : chunk) {
            months.push_back(month);
        }
    }

    return months;
}

/*int main() {
    // Створюємо масив студентів
    std::vector<Student> students = {
        {"Tom",   89},
        {"Kate", 93},
        {"Kiril",  78},
        {"Ban",  50},
        {"Alice", 83}
    };

    // Знаходимо студента з найвищими балами
    auto highest_score_student = std::max_element(students.begin(), students.end(),
                                                  [](const Student &a, const Student &b) -> bool {
                                                      return a.score < b.score;
                                                  });
    std::cout << "highest_score_student: " << highest_score_student->name << std::endl;


    int num_chunks = 3;
    int number_of_chunks = students.size() / num_chunks + bool(students.size() % num_chunks);
    std::vector<std::vector<Student>> vectors;
    for (int i = 0; i < students.size(); i += num_chunks) {
        std::vector<Student> sub_vector;
        int k = 0;
        for (int j = i; j < students.size() && k < num_chunks; ++k, ++j) {
            sub_vector.push_back(students[j]);
        }
        vectors.push_back(sub_vector);
    }
    std::vector<std::thread> threads(number_of_chunks);
    std::vector<int> maxScores(number_of_chunks, 0);
    for (int i = 0; i < number_of_chunks; ++i) {
        threads[i] = std::thread(maxScore, vectors[i], std::ref(maxScores[i]));
    }
    for (int i = 0; i < number_of_chunks; ++i) {
        threads[i].join();
    }
    std::sort(maxScores.begin(), maxScores.end(), [](int a, int b) -> bool { return a > b; });
    std::cout << "Max score: " << maxScores[0] << std::endl;


    std::vector<Month> months = {
        {"December ",  -4},
        {"March ", 10},
        {"June ",    30},
        {"September  ",    24},
        {"November  ",    12},
    };

    std::sort(months.begin(), months.end(),
             [](Month a, Month b) -> bool { return a.average_temperature < b.average_temperature; });
    for (const auto &month: months) {
        std::cout << month.name << " " << month.average_temperature << "\n";
    }
    std::vector<Month> months2 = {
        {"June ",    30},
        {"September  ",    24},
        {"November  ",    12},
        {"December ",  -4},
        {"March ", 10},
    };
    std::cout << "\n";


    std::vector<Month> sorted_months = multithreadedSortMonths(months2, num_chunks);

    for (const auto& month : sorted_months) {
        std::cout << month.name << month.average_temperature << "\n";
    }
    return 0;
}*/

