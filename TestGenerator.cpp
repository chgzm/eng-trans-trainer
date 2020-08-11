#include "TestGenerator.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

int TestGenerator::load(const std::string& filePath) noexcept {
    if (this->loadQuestions(filePath) != 0) {
        std::fprintf(stderr, "Failed to load questions%s\n", filePath.c_str());
        return -1;
    }

    const size_t lastSlashIndex = filePath.find_last_of("/");
    this->statusPath = "./Status/" + filePath.substr(lastSlashIndex + 1, filePath.size() - 3);

    if (this->loadLearningStatus(this->statusPath) != 0) {
        std::fprintf(stderr, "Failed to load learning status %s\n", this->statusPath.c_str());
        return -1;
    }

    return 0;
}

void TestGenerator::startTest() noexcept {
    std::sort(this->questions.begin(), this->questions.end(), [](const auto& l, const auto& r) {
        return l.ansCount < r.ansCount;
    });

    std::vector<Question*> extracted;
    const int minAnsCount = this->questions[0].ansCount;
    for (size_t i = 0; i < this->questions.size(); ++i) {
        if ((5 <= extracted.size()) && (minAnsCount < this->questions[i].ansCount)) {
            break;
        }

        extracted.push_back(&(this->questions[i]));
    }

    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(extracted.begin(), extracted.end(), engine);

    const size_t nums = std::min<size_t>(5, extracted.size()); 
    for (size_t i = 0; i < nums; ++i) {
        Question& q = *(extracted[i]);

        std::printf("%s\n", q.jp.c_str());

        std::string input;
        std::getline(std::cin, input);

        bool correct = false;
        for (const std::string& ans : q.en) {
            if (this->isCorrect(input, ans)) {
                std::printf("\x1b[1;32mCorrect!!\x1b[0m\n");
                correct = true;
                ++(q.ansCount);
                
                break;
            } 
        }

        if (correct == false) {
            std::printf("\x1b[1;31mWrong...\x1b[0m  \n");
            std::printf("Answer:\n");
            for (const std::string& ans : q.en) {
                std::printf("%s\n", ans.substr(2).c_str());
            } 
        }
    }
}

int TestGenerator::writeStatus() noexcept {
    std::sort(this->questions.begin(), this->questions.end(), [](const auto& l, const auto& r) {
        return l.qnum < r.qnum;
    });

    std::ofstream ofs(this->statusPath);
    if (!ofs) {
        std::fprintf(stderr, "Failed to read %s\n", this->statusPath.c_str());
        return -1;
    }

    for (const Question& q : this->questions) {
        ofs << q.qnum << " " << q.ansCount << std::endl;
    }    

    return 0;
}

int TestGenerator::loadQuestions(const std::string& filePath) noexcept {
    std::ifstream ifs(filePath);
    if (!ifs) {
        std::fprintf(stderr, "Failed to read %s\n", filePath.c_str());
        return -1;
    }

    int state = QUE;
    int qnum = 0;
    Question current;
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.size() == 0 || line[0] == ' ') {
            continue;
        }

        switch (state) {
        case QUE: {
            if (line[0] != '#') {
                std::fprintf(stderr, "Invalid line=\"%s\"\n", line.c_str()); 
                return -1;
            }
            current = Question();
            current.qnum = qnum++;
            current.jp = std::move(line); 

            state = ANS;
            break;
        }
        case ANS: {
            while (line.size() != 0) {
                if (line[0] != '-') {
                    std::fprintf(stderr, "Invalid line=\"%s\"\n", line.c_str()); 
                    return -1;
                }
                current.en.push_back(line);
                if (!std::getline(ifs, line)) {
                    break;
                }
            }

            this->questions.push_back(std::move(current));

            state = QUE;
            break;
        } 
        default: {
            std::fprintf(stderr, "Invalid State=%d\n", state);
            return -1;
        }
        }
    }

    return 0;
}

int TestGenerator::loadLearningStatus(const std::string& filePath) noexcept {
    std::ifstream ifs(filePath);
    if (!ifs) {
        std::fprintf(stderr, "Failed to read %s\n", filePath.c_str());
        return -1;
    }

    int i, cnt;
    while (ifs >> i >> cnt) {
        this->questions[i].ansCount = cnt;
    }

    return 0;
}


bool TestGenerator::isCorrect(const std::string& input, const std::string& ansline) noexcept {
    std::string ans = ansline.substr(2);
    if (input == ans) {
        return true;
    }

    std::string str = input;
    for (const auto& p : this->shortened) {
        this->replaceString(ans, p.first, p.second);
        this->replaceString(str, p.first, p.second);
    }

    if (str == ans) {
        return true;
    }

    return false;
}

void TestGenerator::replaceString(std::string& str, const std::string& from, const std::string& to) noexcept { 
    std::size_t pos = str.find(from);
    while (pos != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos = str.find(from);
    }
}
