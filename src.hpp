// Implementation for Problem 059 - OJ 的继承与派生
// Provides BaseJudger and derived classes: ICPCJudger, OIJudger, SpacialJudger

#ifndef SRC_HPP_
#define SRC_HPP_

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

class BaseJudger {
public:
    BaseJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : time_limit_(time_limit), memory_limit_(memory_limit), score_(0) {
        if (answer == nullptr) {
            answer_ = nullptr;
        } else {
            size_t n = std::strlen(answer);
            answer_ = new char[n + 1];
            std::memcpy(answer_, answer, n);
            answer_[n] = '\0';
        }
    }

    virtual void Submit(size_t time, size_t memory, const char *output) = 0;

    size_t GetScore() const { return score_; }

    virtual ~BaseJudger() {
        delete[] answer_;
        answer_ = nullptr;
    };

protected:
    char *answer_;
    const size_t time_limit_;
    const size_t memory_limit_;
    size_t score_;

    virtual bool CheckAnswer(const char *output) const {
        // the output must equal to the answer
        if (answer_ == nullptr && output == nullptr) return true;
        if (answer_ == nullptr || output == nullptr) return false;
        return std::strcmp(answer_, output) == 0;
    }
};


class OIJudger : public BaseJudger {
public:
    OIJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), submitted_(false) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (submitted_) {
            // Multiple submissions cause overall score to be zero
            score_ = 0;
            return;
        }
        submitted_ = true;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            score_ = 100;
        } else {
            score_ = 0;
        }
    }

protected:
    bool CheckAnswer(const char *output) const override {
        // Compare ignoring trailing spaces at the end of each line
        if (answer_ == nullptr && output == nullptr) return true;
        if (answer_ == nullptr || output == nullptr) return false;

        auto split_lines = [](const char *s) {
            std::vector<std::string> lines;
            if (!s) return lines;
            const char *p = s;
            const char *start = p;
            while (*p) {
                if (*p == '\n') {
                    lines.emplace_back(start, p - start);
                    ++p;
                    start = p;
                } else {
                    ++p;
                }
            }
            // If the string does not end with a newline, capture the last line
            if (p != start) {
                lines.emplace_back(start, p - start);
            }
            return lines;
        };

        auto rtrim_trailing_spaces = [](std::string &s) {
            size_t i = s.size();
            while (i > 0) {
                char c = s[i - 1];
                if (c == ' ' || c == '\t') {
                    --i;
                } else {
                    break;
                }
            }
            if (i < s.size()) s.erase(i);
        };

        std::vector<std::string> ans_lines = split_lines(answer_);
        std::vector<std::string> out_lines = split_lines(output);

        if (ans_lines.size() != out_lines.size()) return false;
        for (size_t i = 0; i < ans_lines.size(); ++i) {
            rtrim_trailing_spaces(ans_lines[i]);
            rtrim_trailing_spaces(out_lines[i]);
            if (ans_lines[i] != out_lines[i]) return false;
        }
        return true;
    }

private:
    bool submitted_;
};


class ICPCJudger : public BaseJudger {
public:
    ICPCJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        size_t cur = 0;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            cur = 100;
        } else {
            cur = 0;
        }
        if (cur > score_) score_ = cur;
    }
};


class SpacialJudger : public BaseJudger {
public:
    SpacialJudger(size_t time_limit, size_t memory_limit,
                  size_t full_score_time, size_t full_score_memory,
                  const char *answer)
        : BaseJudger(time_limit, memory_limit, answer),
          full_score_time_(full_score_time), full_score_memory_(full_score_memory) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        size_t cur = 0;
        if (time > time_limit_ || memory > memory_limit_ || !CheckAnswer(output)) {
            cur = 0;
        } else {
            int time_score = 0;
            int mem_score = 0;

            // Time score
            if (time <= full_score_time_) {
                time_score = 100;
            } else if (time >= time_limit_) {
                time_score = 0;
            } else {
                // Linear between (full_score_time_, 100) and (time_limit_, 0)
                // floor applied via integer division
                double numerator_t = static_cast<double>(time_limit_ - time) * 100.0;
                double denom_t = static_cast<double>(time_limit_ - full_score_time_);
                time_score = static_cast<int>(std::floor(numerator_t / denom_t));
                if (time_score < 0) time_score = 0;
                if (time_score > 100) time_score = 100;
            }

            // Memory score
            if (memory <= full_score_memory_) {
                mem_score = 100;
            } else if (memory >= memory_limit_) {
                mem_score = 0;
            } else {
                double numerator_m = static_cast<double>(memory_limit_ - memory) * 100.0;
                double denom_m = static_cast<double>(memory_limit_ - full_score_memory_);
                mem_score = static_cast<int>(std::floor(numerator_m / denom_m));
                if (mem_score < 0) mem_score = 0;
                if (mem_score > 100) mem_score = 100;
            }

            cur = static_cast<size_t>((time_score * mem_score) / 100);
        }
        if (cur > score_) score_ = cur;
    }

private:
    size_t full_score_time_;
    size_t full_score_memory_;
};

#endif // SRC_HPP_

