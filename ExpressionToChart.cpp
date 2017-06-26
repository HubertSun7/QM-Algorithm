//
//  ExpressionToChart.cpp
//  mini_project
//
//  Created by sxb on 2017/4/9.
//  Copyright © 2017年 sxb. All rights reserved.
//

#include "ExpressionToChart.h"
using std::string;


bool ExprToChart::Isoperator(const char &ch){
    for (int i=0; i<4; i++){
        if (ch == expr_operator[i]){
            return true;
        }
    }
    return false;
}


bool ExprToChart::Errorcheck(const string& Tmpexpr){
    for (int i=0; i<Tmpexpr.length(); i++){
        if (isalpha(Tmpexpr[i])){
            if (i+1 < Tmpexpr.length()){
                if (Isoperator(Tmpexpr[i+1])==false && Tmpexpr[i+1] != ')') { throw SyntaxError{}; }
            }
            if (i!=0) {
                if (Isoperator(Tmpexpr[i-1])==false && Tmpexpr[i-1] != '(') { throw SyntaxError{}; }
            }
        }
        if (Tmpexpr[i] == '(' || Tmpexpr[i] == ')'){
            S_Bracket.push_back(Tmpexpr[i]);
        }
    }
        
    while(S_Bracket.empty()==false){
        if ((S_Bracket.size() % 2 )!= 0) { throw ExprBracketError{}; }
        if (S_Bracket.back() == ')') {
            if (S_Bracket.front() == '('){
                S_Bracket.pop_back();
                S_Bracket.pop_front();
            }else{
                throw ExprBracketError{};
            }
        }
        else { throw ExprBracketError{}; }
    }
    
    return true;
}

string ExprToChart::filter(const string& expr,const int& n){
    string result="";
    if (expr.length() == 0) { throw ExprEmptyError{}; }
    
    for (int i=0; i<expr.length(); i++){
        if (expr[i]==' ' || (expr[i]<='H' && expr[i]>='A') || Isoperator(expr[i]) || expr[i]=='(' || expr[i]==')' ){
            if (expr[i] != ' '){
                result += expr[i];
            }
            if (isalpha(expr[i])){
                int count=0;
                for (int k=0; k<Expralpha.size(); k++){
                    if (Expralpha[k] == expr[i]) count++;
                }
                if (count == 0) Expralpha.push_back(expr[i]);
            }
        }else
        {
            throw InvaidCharError{};
        }
    }
    
    if (Expralpha.size() != n) {
        throw WrongNumVarError{};
    }
    return result ;
}

int ExprToChart::GetPriority(const char &ch){
    int i;
    for (i=0; i<4; i++){
        if (expr_operator[i] == ch) { break; }
    }
    
    return i+1 ;
}

string ExprToChart::ExprOperatorOut(const int& priority,bool Isbracket){
    string result = "";
    while(Exproperator.empty() == false && (GetPriority(Exproperator.top()) >= priority) ){
        if (Exproperator.top() != '('){
            result += Exproperator.top();
            Exproperator.pop();
        }
        else{
            if (Isbracket == true) { Exproperator.pop(); }
            break;
        }
    }
    
    return result;
}


string ExprToChart::ExprTran(const string& TmpExpr){
    string result="";
    for (int i=0; i<TmpExpr.length(); i++){
        if (isalpha(TmpExpr[i])){
            result += TmpExpr[i];
        }
        if (Isoperator(TmpExpr[i])){
            if (Exproperator.empty()) { Exproperator.push(TmpExpr[i]); }
            else{
                if (GetPriority(Exproperator.top()) < GetPriority(TmpExpr[i])){
                    Exproperator.push(TmpExpr[i]);
                }
                else if (GetPriority(Exproperator.top()) >= GetPriority(TmpExpr[i])){
                    result += ExprOperatorOut(GetPriority(TmpExpr[i]),false);
                    Exproperator.push(TmpExpr[i]);
                }
            }
            
        }
        if (TmpExpr[i] == '(') { Exproperator.push(TmpExpr[i]); }
        if (TmpExpr[i] == ')') { result += ExprOperatorOut(0, true); }
    }
    if (Exproperator.empty() == false) result += ExprOperatorOut(0, false);
    
    return result;
}


string ExprToChart::CalTruthtable(const string& TmpExpr){
    string result = "";
    for (int i=0; i<TmpExpr.length(); i++){
        if (isalpha(TmpExpr[i])){
            ExprNum.push(VarNum[TmpExpr[i]-'A']);
        }
        if (Isoperator(TmpExpr[i])){
            if (TmpExpr[i] == '~') {
                int tmp = ~ExprNum.top();
                ExprNum.pop();
                ExprNum.push(tmp & 1);
            }else{
                int tmp_right = ExprNum.top();
                ExprNum.pop();
                if (ExprNum.empty()) { throw SyntaxError{}; }
                int tmp_left = ExprNum.top();
                ExprNum.pop();
                switch (TmpExpr[i]) {
                    case '|':  ExprNum.push((tmp_left|tmp_right)&1);  break;
                    case '&':  ExprNum.push((tmp_left&tmp_right)&1);  break;
                    case '^':  ExprNum.push((tmp_left^tmp_right)&1);  break;
                }
            }
        }
    }
    
    result += std::to_string(ExprNum.top());
    ExprNum.pop();
    if (!ExprNum.empty()) { throw SyntaxError{}; }
    
    return result;
}


string ExprToChart::ExprTruthtable(const int& n,const string& TmpExpr){
    string result = "";
    for (int mark = (1 << n)-1; mark >=0 ; mark--){
        for (int i=0; i<n; i++){
            int tmp_ch = Expralpha[i] - 'A';
            int tmp_data = (mark & (1 << tmp_ch)) >> tmp_ch  ;
            VarNum[tmp_ch] = tmp_data;
        }
        result += CalTruthtable(TmpExpr);
    }
    
    return result;
}


string ExprToChart::solve(int n, const string &expr){
    string result="";
    string tmp="";
    
    tmp = filter(expr,n);
    Errorcheck(tmp);
    tmp = ExprTran(tmp);
    result = ExprTruthtable(n,tmp);
    
    return result;
}
