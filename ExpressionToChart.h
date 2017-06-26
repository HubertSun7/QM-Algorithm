//
//  ExpressionToChart.h
//  mini_project
//
//  Created by sxb on 2017/4/9.
//  Copyright © 2017年 sxb. All rights reserved.
//

#ifndef EXPRESSIONTOCHART_H_
#define EXPRESSIONTOCHART_H_

#include <stdio.h>
#include "ErrorConst.h"
#include <stack>
#include <vector>
#include <string>
#include <deque>

const char expr_operator[] = {'|','^','&','~'};

class ExprToChart{
private:
    std::vector <char> Expralpha;
    std::stack <char> Exproperator;
    std::deque <char> S_Bracket;
    std::stack <int> ExprNum;
    
    int VarNum[8]{-1,-1,-1,-1,-1,-1,-1,-1};
    

    std::string filter(const std::string&InExpr,const int& n);
    bool Isoperator(const char& ch);
    int GetPriority(const char& ch);
    std::string ExprTran(const std::string&TmpExpr);
    std::string ExprOperatorOut(const int& priority,const bool Isbracket);
    bool Errorcheck(const std::string& TmpExpr);
    std::string ExprTruthtable(const int&n,const std::string& TmpExpr);
    std::string CalTruthtable(const std::string& TmpExpr);
    
public:
    std::string solve(int n,const std::string& expr);
    
};


#endif
