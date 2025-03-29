#include "Solution.hpp"

using namespace Solver;


std::vector<double> parseString(std::string str){
    std::vector<double> result;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, ';'))
    {
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        
        if(!token.empty()){
            try{
                result.push_back(std::stod(token));
            }
            catch (const std::invalid_argument &e){
                std::cerr << "Can't convert " << token << " to number\n";
                return {};
            }
        }
    }

    return result;
}



Homework::Homework(std::string const &filepath){
    std::ifstream input_file(filepath);

    if(!input_file.is_open()){
        throw std::runtime_error("Can't open file: " + filepath);
    }

    std::vector<double> odds;
    std::string input;

    while (input_file >> input)
    {
        all_odds.push_back(parseString(input));
    }
    current_equation = 0;
}

size_t Homework::getTotalNumber() const{
    return all_odds.size();
}

std::vector<double> Homework::getNextEquation(){
    if(all_odds.size() == 0){
        return {};
    }

    current_equation = (current_equation +1) % all_odds.size();

    return all_odds[current_equation];
}

std::vector<double> Homework::getPreviousEquation(){
    if(all_odds.size() == 0){
        return {};
    }

    current_equation = (current_equation - 1 + all_odds.size()) % all_odds.size();

    return all_odds[current_equation];
}

std::vector<double> Homework::getCurrentEquation() const{
    if(all_odds.size() == 0){
        return {};
    }

    return all_odds[current_equation];
}

std::vector<double> Homework::getEquationByNum(size_t number){
    if(number < 0 || number >= all_odds.size()){
        std::cerr << "There is no equation with number " << number << "\n";
        return {};
    }
    
    current_equation = number;

    return all_odds[number];
}



TypesOfEquation Human::DetermineType(std::vector<double> const &odds) const{
    if(fabs(odds[0]) <= Constants::EPSILON){
        if(fabs(odds[1])<= Constants::EPSILON){
            return TypesOfEquation::CONSTANT;
        }
        return TypesOfEquation::LINEAR;
    }
    return TypesOfEquation::QUADRATIC;
}

Answer Human::SolveConstant(std::vector<double> const &odds) const{
    Answer ans;

    if(fabs(odds[2]) <= Constants::EPSILON){
        ans.roots_num = RootsCount::INF;
        return ans;
    }
    
    ans.roots_num = RootsCount::ZERO; 
    return ans;
}

Answer Human::SolveLinear(std::vector<double> const &odds) const{
    Answer ans;
    ans.roots_num = RootsCount::ONE;
    ans.roots = {-odds[2] / odds[1] };

    return ans;
}

Answer Human::SolveQuadratic(std::vector<double> const &odds) const{
    double D = odds[1]*odds[1] - 4 * odds[0] * odds[2];
    Answer ans;

    if(D > 0){
        D = std::sqrt(D);

        ans.roots_num = RootsCount::TWO;

        ans.roots.push_back( (-odds[1] - D) / (2 * odds[0]) );
        ans.roots.push_back( (-odds[1] + D) / (2 * odds[0]) );

        if(ans.roots[1] < ans.roots[0]){
            std::swap(ans.roots[1], ans.roots[0]);
        }
    }
    else if(fabs(D) <= Constants::EPSILON){
        ans.roots_num = RootsCount::ONE;

        ans.roots.push_back(-odds[1] / (2 * odds[0]));
    }
    else{
        ans.roots_num = RootsCount::ZERO;
    }

    return ans;
}



Homework Teacher::makeNewHomework(std::string const &input_filepath) const{
    return Homework(input_filepath);
}

void Teacher::writeProgress(std::string const &output_filepath, int precision) const{
    std::ofstream output(name + "_"+ output_filepath);

    if (!output){ 
        throw std::runtime_error("Cannot open file: " + output_filepath);
    }
    output << std::setprecision(precision);
    output << "Name;Percentage\n";
    for(const auto &[key, value]: total_equations){
        output << key << ";";
        if(value == 0){
            output << "0%\n";
        }
        else{
            output << (100.0 * solved_equations.at(key))/value << "%\n";
        }
    }
}

void Teacher::calcProgress(std::vector<Solution> const& solutions) {
    for (const Solution& sol : solutions) {
        std::string name = sol.getName();
        if (total_equations.find(name) == total_equations.end()) {
            total_equations[name] = 0;
            solved_equations[name] = 0;
        }

        total_equations[name]++;
        std::vector<double> odds = sol.getOdds();
        TypesOfEquation type = DetermineType(odds);
        Answer correct_answer;

        switch (type) {
            case TypesOfEquation::CONSTANT:
                correct_answer = SolveConstant(odds);
                break;
            case TypesOfEquation::LINEAR:
                correct_answer = SolveLinear(odds);
                break;
            case TypesOfEquation::QUADRATIC:
                correct_answer = SolveQuadratic(odds);
                break;
            default:
                break;
        }
            
        if (IsAnswerCorrect(correct_answer, sol.getAnswer())) {
            solved_equations[name]++;           
        }
    }
}

bool Teacher::IsAnswerCorrect(Answer const& correct, Answer const& student) const {
    if (correct.roots_num != student.roots_num) {
        return false;
    }

    if (correct.roots_num == RootsCount::INF || correct.roots_num == RootsCount::ZERO) {
        return true;
    }

    if (correct.roots.size() != student.roots.size()) {
        return false;
    }

    if (correct.roots_num == RootsCount::ONE) {
        return std::abs(correct.roots[0] - student.roots[0]) <= Constants::EPSILON;
    }

    if (correct.roots_num == RootsCount::TWO) {
        return (std::abs(correct.roots[0] - student.roots[0]) <= Constants::EPSILON) && (std::abs(correct.roots[1] - student.roots[1]) <= Constants::EPSILON);
    }

    return false;
}

std::string Human::getName() const { return name;}

void Human::setName(std::string const &name){
    this->name = name;
}



std::vector<Solution> BadStudent::solveHomework(Homework &hw){
    std::vector<Solution> solutions;

    Answer ans;
    ans.roots_num = RootsCount::ZERO;
    ans.roots = {};

    Solution sol;
    sol.setName(name);
    sol.setAnswer(ans);
    for(int i = 0; i < hw.getTotalNumber(); i++){
        sol.setOdds(hw.getEquationByNum(i)); 
        solutions.push_back(sol);
    }
    
    return solutions;
}

bool AverageStudent::MakingError(){
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    static auto gen = std::mt19937{std::random_device{}()};
    return dist(gen) <= error_chance;
}

std::vector<Solution> AverageStudent::solveHomework(Homework &hw){
    std::vector<Solution> solutions;

    Answer ans;

    Solution sol;
    sol.setName(name);
    for(int i = 0; i < hw.getTotalNumber(); i++){
        sol.setOdds(hw.getEquationByNum(i)); 

        if(MakingError()){
            ans.roots_num = RootsCount::ZERO;
            ans.roots = {};
        }

        else{
            TypesOfEquation type = DetermineType(sol.getOdds());

            switch (type) {
                case TypesOfEquation::CONSTANT:
                    ans = SolveConstant(sol.getOdds());
                    break;
                case TypesOfEquation::LINEAR:
                    ans = SolveLinear(sol.getOdds());
                    break;
                case TypesOfEquation::QUADRATIC:
                    ans = SolveQuadratic(sol.getOdds());
                    break;
                default:
                    break;
            }
        }

        sol.setAnswer(ans);
        solutions.push_back(sol);
    }

    return solutions;
}

std::vector<Solution> GoodStudent::solveHomework(Homework &hw){
    std::vector<Solution> solutions;

    Answer ans;

    Solution sol;
    sol.setName(name);
    for(int i = 0; i < hw.getTotalNumber(); i++){
        std::vector<double> odds = hw.getEquationByNum(i);
        sol.setOdds(odds); 
        TypesOfEquation type = DetermineType(odds);

        switch (type) {
            case TypesOfEquation::CONSTANT:
                ans = SolveConstant(odds);
                break;
            case TypesOfEquation::LINEAR:
                ans = SolveLinear(odds);
                break;
            case TypesOfEquation::QUADRATIC:
                ans = SolveQuadratic(odds);
                break;
            default:
                break;
        }

        sol.setAnswer(ans);
        solutions.push_back(sol);
    }
        

    return solutions;
}



void Solution::writeFile(std::string const &output_filepath, int precision) const{//Структура вывода такая a;b;c;RootsCount;all_roots через ;
    std::ofstream output(name+"_"+output_filepath);

    if (!output){ 
        throw std::runtime_error("Cannot open file: " + output_filepath);
    }
    output << std::setprecision(precision);
    for(auto i: odds){
        output << i << ";";
    }

    output << static_cast<int>(answer.roots_num) << ";";

    for(auto i: answer.roots){
        output << i << ";";
    }

    output << "\n";
}

void Solution::readFile(std::string const &input_filepath){
    std::ifstream input(input_filepath);

    if (!input){ 
        throw std::runtime_error("Cannot open file: " + input_filepath);
    }

    std::string str;
    input >> str;

    std::vector<double> result =  parseString(str);
    odds = {result[0], result[1], result[2]};
    Answer ans;
    ans.roots_num = static_cast<RootsCount>(static_cast<int>(result[3]));
    switch (ans.roots_num)
    {
        case RootsCount::ZERO:
        case RootsCount::INF:
            ans.roots = {};
            break;
        case RootsCount::ONE:
            ans.roots = {result[4]};
            break;
        case RootsCount::TWO:
            ans.roots = {result[4], result[5]};
            break;
        default:
            break;
    }
}

void Solution::setName(std::string const &name){
    this->name = name;
}

void Solution::setOdds(std::vector<double> const &odds){
    this->odds = odds;
}

void Solution::setAnswer(Answer const &answer){
    this->answer = answer;
}

std::string Solution::getName() const{
    return name;
}

std::vector<double> Solution::getOdds() const{
    return odds;
}

Answer Solution::getAnswer() const{
    return answer;
}