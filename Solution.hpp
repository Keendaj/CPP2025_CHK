#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <random>
#include <cmath>

namespace Solver{
    namespace Constants {
        constexpr double EPSILON = 1e-6;
    }

    enum class TypesOfEquation {CONSTANT, LINEAR, QUADRATIC, SIZE};
    enum class RootsCount {ZERO, ONE, TWO, INF, ERROR, SIZE};

    struct Answer
    {
        RootsCount roots_num;
        std::vector<double> roots;
    };

    class Solution
    {
        public:
            explicit Solution(std::string const &name = "", std::vector<double> const &odds = {}, Answer const &answer = {}) : name(name), odds(odds), answer(answer){};
            
            void writeFile(std::string const &output_filepath, int precision = 8) const;
            void readFile(std::string const &input_filepath);

            void setName(std::string const &name);
            void setOdds(std::vector<double> const &odds);
            void setAnswer(Answer const &answer);

            std::string getName() const;
            std::vector<double> getOdds() const;
            Answer getAnswer() const;

        private:
            std::vector<double> odds;
            Answer answer;
            std::string name;

    };

    class Homework{
        public:
            size_t getTotalNumber() const;

            std::vector<double> getNextEquation();
            std::vector<double> getPreviousEquation();
            std::vector<double> getCurrentEquation() const;
            std::vector<double> getEquationByNum(size_t number);

        private:
            Homework(std::string const &filepath);
            friend class  Teacher;

            std::vector<std::vector<double>> all_odds;
            size_t current_equation;
    };

    class Human
    {
        public:
            explicit Human(std::string const &name = "") : name(name){};

            std::string getName() const;

            void setName(std::string const &name);

        protected:

            TypesOfEquation DetermineType(std::vector<double> const &odds) const;
            Answer SolveConstant(std::vector<double> const &odds) const;
            Answer SolveLinear(std::vector<double> const &odds) const;
            Answer SolveQuadratic(std::vector<double> const &odds) const;

            std::string name;
    };

    class Teacher : public Human{
        public:
            explicit Teacher(std::string const &name = "") : Human(name){};
            
            Homework makeNewHomework(std::string const &input_filepath) const;
            
            void writeProgress(std::string const &output_filepath, int precision = 4) const;
            void calcProgress(std::vector<Solution> const &solutions);

        private:
            bool IsAnswerCorrect(Answer const &correct, Answer const &student) const;
            
            std::unordered_map<std::string, long> solved_equations;
            std::unordered_map<std::string, long> total_equations;
    };

    class BasicStudent : public Human{
        public:
            virtual std::vector<Solution> solveHomework(Homework &hw) = 0;

        protected:
            explicit BasicStudent(std::string const &name = "") : Human(name){};
    };

    class BadStudent : public BasicStudent{
        public:
            explicit BadStudent(std::string const &name = "") : BasicStudent(name){};
            
            std::vector<Solution> solveHomework(Homework  &hw) override;
    };

    class AverageStudent : public BasicStudent{
        public:
            explicit AverageStudent(std::string const &name = "", double error_chance = 0.5) : BasicStudent(name), error_chance(error_chance){};
            
            std::vector<Solution> solveHomework(Homework &hw) override;

        private:
            bool MakingError();
            double error_chance;
    };

    class GoodStudent : public BasicStudent{
        public:
            explicit GoodStudent(std::string const &name = "") : BasicStudent(name){};
            
            std::vector<Solution> solveHomework(Homework &hw) override;
    };
}