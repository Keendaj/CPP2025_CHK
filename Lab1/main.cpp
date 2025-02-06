#include "solution.h"


using namespace std;


void QuadraticEqationSolveTwoRootsTest_1(){
    float odds[3] = {1,-7,12};//roots 3 and 4
    RootsCount root_num;
    vector<float> roots = SolveEquation(odds, root_num);

    if(root_num == RootsCount::TWO && roots[0] == 3 && roots[1] == 4 && roots.size() == 2){
        cout << "| TEST 1\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 1\tDENY\t |\n";
    }
}

void QuadraticEqationSolveOneRootTest_2(){
    float odds[3] = {1,-4,4};//root 2
    RootsCount root_num;
    vector<float> roots = SolveEquation(odds, root_num);

    if(root_num == RootsCount::ONE && roots[0] == 2 && roots.size() == 1){
        cout << "| TEST 2\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 2\tDENY\t |\n";
    }
}

void QuadraticEqationSolveZeroRootsTest_3(){
    float odds[3] = {1,2,2};//has no roots
    RootsCount root_num;
    vector<float> roots = SolveEquation(odds, root_num);

    if(root_num == RootsCount::ZERO && roots.size() == 0){
        cout << "| TEST 3\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 3\tDENY\t |\n";
    }
}

void QuadraticEqationSolveNegativeFirstOddTest_4(){
    float odds[3] = {-1,7,-12};//roots 3 and 4
    RootsCount root_num;
    vector<float> roots = SolveEquation(odds, root_num);
    if(root_num == RootsCount::TWO && roots[0] == 3 && roots[1] == 4 && roots.size() == 2){
        cout << "| TEST 4\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 4\tDENY\t |\n";
    }
}

void LinearEquationPositiveOddTest_5(){
    float odds[3] = {0,1,12};//root -12
    RootsCount root_num;
    vector<float> roots = SolveEquation(odds, root_num);
    if(root_num == RootsCount::ONE && roots[0] == -12 && roots.size() == 1){
        cout << "| TEST 5\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 5\tDENY\t |\n";
    }
}

void LinearEquationNegativeOddTest_6(){
    float odds[3] = {0,-1,12};//root 12
    RootsCount root_num;
    vector<float> roots = SolveEquation(odds, root_num);
    if(root_num == RootsCount::ONE && roots[0] == 12 && roots.size() == 1){
        cout << "| TEST 6\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 6\tDENY\t |\n";
    }
}

void ConstantEquationZeroRootsTest_7(){
    float odds[3] = {0,0,12};//root 12
    RootsCount root_num;
    vector<float> roots = SolveEquation(odds, root_num);
    if(root_num == RootsCount::ZERO && roots.size() == 0){
        cout << "| TEST 7\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 7\tDENY\t |\n";
    }
}

void ConstantEquationInfinityRootsTest_8(){
    float odds[3] = {0,0,0};//root 12
    RootsCount root_num;
    vector<float> roots = SolveEquation(odds, root_num);
    if(root_num == RootsCount::INF && roots.size() == 0){
        cout << "| TEST 8\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 8\tDENY\t |\n";
    }
}

void WriteToFileTest_9(){
    vector<float> roots = {3,4};
    
    ofstream output("output.txt");

    if(!output.is_open()){
        cout << "| TEST 9\tDENY\t |\n";
        return;
    }
    WriteRootsToStream(output, roots, RootsCount::TWO);
    cout << "| TEST 9\tSUCCESS |\n";
}

void ReadFromFileTest_10(){
    ifstream input("input.txt");

    if(!input.is_open()){
        cout << "| TEST 9\tDENY\t |\n";
        return;
    }

    float *odds = ReadOddsFromStream(input);

    if(odds[0] == 1 && odds[1] == -4 && odds[2] == 4){
        cout << "| TEST 10\tSUCCESS |\n";
    }
    else{
        cout << "| TEST 10\tDENY\t |\n";
    }
    
    delete[] odds;
}

int main(){
    cout << "Start testing\n";

    QuadraticEqationSolveTwoRootsTest_1();

    QuadraticEqationSolveOneRootTest_2();

    QuadraticEqationSolveZeroRootsTest_3();

    QuadraticEqationSolveNegativeFirstOddTest_4();
    
    LinearEquationPositiveOddTest_5();

    LinearEquationNegativeOddTest_6();

    ConstantEquationZeroRootsTest_7();

    ConstantEquationInfinityRootsTest_8();

    WriteToFileTest_9();

    ReadFromFileTest_10();


}