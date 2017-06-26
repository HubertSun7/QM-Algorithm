//
//  ChartToExpression.h
//  mini_project
//
//  Created by sxb on 2017/4/9.
//  Copyright © 2017年 sxb. All rights reserved.
//

#ifndef ChartToExpression_h_
#define ChartToExpression_h_


#include <stdio.h>
#include "ErrorConst.h"
#include <string>
#include <vector>


const char alpha[8]{'A','B','C','D','E','F','G','H'};

class implication{
private:
    std::vector <int> M_include;
    std::vector <char> var_imp; //H->L
    int weight;
    bool used;
    
    bool cal_weight(const std::string& Var);
    bool set_Var(const std::string& Var);
public:
    implication(const std::string& Var) : weight{0}, used{false}{
        cal_weight(Var);
        set_Var(Var);
    } ;
    bool isused() const { return used; }
    void S_used() { used = true; }
    
    void add_include(const int& index) { M_include.push_back(index);}
    long NumLen() const { return var_imp.size(); }
    int Weight() const { return weight; }
    const std::vector<char> G_var() const { return var_imp; }
    const std::vector <int> G_incl() const { return M_include; }
    
 };


class ChartToExpr{
private:
    int NumVar{0};
    std::vector<int> diff_pos;
    std::vector<implication*> imp;
    std::vector<implication*> tmp_imp;
    std::vector<std::vector<int> > minitable;
    std::vector<int> SOP;
    std::vector<int> return_SOP;
    
    //Truthtable Input
    std::string filter(const std::string& TmpTable) const;
    std::string TranToVar(const int& Val,const int& Num) ;
    
    //QM
    bool compare(const implication* imp1,const implication* imp2);
    bool same(const implication* imp1,const implication* imp2);
    void Quine_McCluskey(const std::string& truthtable);
    
    //Petrik
    std::string petrik();
    std::vector<int>& Simplify(const int& col);
    long CountImps(const long&len,const long& val);
    
    //Output Expression
    std::string GetExpr();
    
    //for test
    void showtable();
    
public:
    std::string solve(const std::string& truthtable);
    
};

implication* operator +(const implication& imp1,const implication& imp2);



#endif /* ChartToExpression_hpp */
