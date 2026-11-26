#include <array>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct Student {
    std::string name;
    char gender;
    int class_number;
    std::array<int, 9> scores{};
    int total = 0;
    int rank = 0;

    int average() const { return total / 9; }
};

struct RankingOrder {
    const std::vector<Student>* students;

    bool operator()(int left, int right) const {
        const auto& a = (*students)[left];
        const auto& b = (*students)[right];
        if (a.average() != b.average()) return a.average() > b.average();
        if (a.scores != b.scores) return a.scores > b.scores;
        return a.name < b.name;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Student> students;
    std::unordered_map<std::string, int> by_name;
    std::set<int, RankingOrder> current_order(RankingOrder{&students});
    std::vector<int> published_order;
    bool started = false;
    bool dirty = false;

    const auto publish = [&]() {
        if (!dirty) return;
        published_order.assign(current_order.begin(), current_order.end());
        for (std::size_t i = 0; i < published_order.size(); ++i) {
            students[published_order[i]].rank = static_cast<int>(i) + 1;
        }
        dirty = false;
    };

    std::string command;
    while (std::cin >> command && command != "END") {
        if (command == "ADD") {
            Student student;
            std::cin >> student.name >> student.gender >> student.class_number;
            for (int& score : student.scores) {
                std::cin >> score;
                student.total += score;
            }
            if (started) {
                std::cout << "[Error]Cannot add student now.\n";
            } else if (by_name.count(student.name)) {
                std::cout << "[Error]Add failed.\n";
            } else {
                const int id = static_cast<int>(students.size());
                by_name.emplace(student.name, id);
                students.push_back(std::move(student));
            }
        } else if (command == "START") {
            started = true;
            for (std::size_t i = 0; i < students.size(); ++i) {
                current_order.insert(static_cast<int>(i));
            }
            dirty = true;
            publish();
        } else if (command == "UPDATE") {
            std::string name;
            int course, score;
            std::cin >> name >> course >> score;
            const auto it = by_name.find(name);
            if (it == by_name.end()) {
                std::cout << "[Error]Update failed.\n";
                continue;
            }
            auto& student = students[it->second];
            if (student.scores[course] == score) continue;
            // Remove the old key before changing data used by the set comparator.
            current_order.erase(it->second);
            student.total += score - student.scores[course];
            student.scores[course] = score;
            current_order.insert(it->second);
            dirty = true;
        } else if (command == "FLUSH") {
            publish();
        } else if (command == "PRINTLIST") {
            for (int id : published_order) {
                const auto& student = students[id];
                std::cout << student.rank << ' ' << student.name << ' '
                          << (student.gender == 'M' ? "male" : "female") << ' '
                          << student.class_number << ' ' << student.average() << '\n';
            }
        } else if (command == "QUERY") {
            std::string name;
            std::cin >> name;
            const auto it = by_name.find(name);
            if (it == by_name.end()) {
                std::cout << "[Error]Query failed.\n";
            } else {
                std::cout << "STUDENT " << name << " NOW AT RANKING "
                          << students[it->second].rank << '\n';
            }
        }
    }
}
