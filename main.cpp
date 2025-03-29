#include "Solution.hpp"

using namespace Solver;

int main(){
    std::string filepath = "dz.csv";

    Teacher teacher = Teacher();
    teacher.setName("Vyacheslav");

    BadStudent student1 = BadStudent("Ivan");
    BadStudent student2 = BadStudent("Alexey");
    AverageStudent student3 = AverageStudent("Alexander");
    AverageStudent student4 = AverageStudent("Albert", 0.5);
    AverageStudent student5 = AverageStudent("Michael", 0.3);
    AverageStudent student6 = AverageStudent("Maksim", 0.8);
    GoodStudent student7 = GoodStudent("Dmitriy");
    GoodStudent student8 = GoodStudent("Andrew");
    

    Homework hw = teacher.makeNewHomework(filepath);

    std::vector<Solution> sols;

    for (Solution sol : student1.solveHomework(hw)) sols.push_back(sol);
    for (Solution sol : student2.solveHomework(hw)) sols.push_back(sol);
    for (Solution sol : student3.solveHomework(hw)) sols.push_back(sol);
    for (Solution sol : student4.solveHomework(hw)) sols.push_back(sol);
    for (Solution sol : student5.solveHomework(hw)) sols.push_back(sol);
    for (Solution sol : student6.solveHomework(hw)) sols.push_back(sol);
    for (Solution sol : student7.solveHomework(hw)) sols.push_back(sol);
    for (Solution sol : student8.solveHomework(hw)) sols.push_back(sol);

    teacher.calcProgress(sols);
    teacher.writeProgress("test.csv");
}