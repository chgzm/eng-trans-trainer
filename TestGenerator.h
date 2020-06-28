#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

#include <string>
#include <vector>

enum State {
    QUE,
    ANS,
};

struct Question {
    Question() : qnum(0), ansCount(0) {
    }

    int qnum;
    int ansCount;
    std::string jp;
    std::vector<std::string> en;
};

class TestGenerator {
public:
    TestGenerator() = default;
    ~TestGenerator() = default;

    int load(const std::string& filePath) noexcept; 
    void startTest() noexcept;
    int writeStatus() noexcept;

private:
    int loadQuestions(const std::string& filePath) noexcept;
    int loadLearningStatus(const std::string& filePath) noexcept;
    bool isCorrect(const std::string& input, const std::string& ansline) noexcept;
    void replaceString(std::string& str, const std::string& from, const std::string& to) noexcept;

    const std::vector<std::pair<std::string, std::string>> shortened = {
        {"It is ",      "It's "     },
        {" it is ",      " it's "   },
        {"What is ",    "What's "   },
        {"I am ",       "I'm "      },
        {"I will ",     "I'll "     },
        {"He will ",    "He'll "    },
        {"She will ",   "She'll "   },
        {"They will ",  "They'll "  },
        {" does not ",  " doesn't " },
        {" did not ",   " didn't "  },
        {" will not ",  " won't "   },
        {" would not ", " wouldn't "},
        {" cannot ",    " can't "   },
        {" could not ", " couldn't "},
        {" have not ",  " haven't " },
        {" has not ",   " hasn't "  },
        {" was not ",   " wasn't "  },
        {" were not ",  " weren't " },
    };

    std::vector<Question> questions;
    std::string statusPath;
};

#endif
