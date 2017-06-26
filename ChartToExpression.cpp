//
//  ChartToExpression.cpp
//  mini_project
//
//  Created by sxb on 2017/4/9.
//  Copyright © 2017年 sxb. All rights reserved.

#include "ChartToExpression.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

using std::string;

//included in Class implication {}
//##########################################################################

bool implication::cal_weight(const std::string &Var){
    int sum{0};
    for (int i=0; i<Var.length(); i++){
        if (Var[i]=='1') sum++;
    }
    weight = sum;
    return true;
}

bool implication::set_Var(const std::string &Var){
    for (int i=0; i<Var.length(); i++){
        if (Var[i]=='1' || Var[i]=='0' || Var[i] == '-') var_imp.push_back(Var[i]);
    }
    return true; //H->L
}


implication* operator +(const implication& imp1,const implication& imp2){
    implication* tmp;
    string var="";
    std::vector <char> var1 = imp1.G_var();
    std::vector <char> var2 = imp2.G_var();
    
    for (int i=0; i<imp1.NumLen(); i++){
        if (var1[i] == var2[i]) var += var1[i];
        else if(var1[i] == '0' && var2[i] == '1'){
            var += '-';
        }
    }
    
    
    tmp = new implication(var);
    for (int i : imp1.G_incl()){ tmp->add_include(i); }
    for (int i : imp2.G_incl()){ tmp->add_include(i); }
    return tmp;
}
//##########################################################################


//included in Class ChartToExpr{}
//##########################################################################

string ChartToExpr::filter(const string& TmpTable) const{
    string result="";
    if ( TmpTable.length()==0 ) { throw TruEmptyError{}; }
    if ( TmpTable.length()>256 ) { throw TruOutRangeError{}; }
    for (int i=0; i<TmpTable.length(); i++){
        if (TmpTable[i] != '0' && TmpTable[i] != '1' && TmpTable[i] != '\n' && TmpTable[i] != ' ' && TmpTable[i] != 'd'){
            throw InvaidCharError{};
        }
        if (TmpTable[i]=='0' || TmpTable[i]=='1' || TmpTable[i] == 'd'){
            result += TmpTable[i];
        }
    }
    return result;
}


string ChartToExpr::TranToVar(const int& val,const int& Num){
    string result="";
    for (int i=0; i<Num; i++){
        result += std::to_string( (val&(1<<(Num-i-1)))>>(Num-i-1) );
    }
    return result; //H->L
}

bool ChartToExpr::compare(const implication* imp1, const implication* imp2){
    std::vector <char> var1 = imp1->G_var();
    std::vector <char> var2 = imp2->G_var();
    int flag=0;
    
    if (imp1->Weight() == imp2->Weight()-1){
        for (int i=0; i<var1.size(); i++){
            if ( (var2[i]-var1[i]) == 1 ) flag++;
            else if (var2[i]-var1[i] != 0 && var2[i]-var1[i] != -1 ){
                return false;
            }
        }
        if  (flag == 1) { return true; }
    }
    return false;
}

bool ChartToExpr::same(const implication* imp1, const implication* imp2){
    std::vector <char> var1 = imp1->G_var();
    std::vector <char> var2 = imp2->G_var();
    for (int i=0; i<var1.size(); i++){
        if (var1[i] != var2[i]) { return false; }
    }
    return true;
}


void ChartToExpr::Quine_McCluskey(const string& TmpTable/*H->L*/){
    int max = (1<<NumVar)-1;
    
    std::vector <implication*>::iterator it;
    std::vector <implication*>::iterator  jt;
    std::vector <implication*> used;
    
    std::vector <implication*> *tmp_imp_weight;
    std::map<int, std::vector<implication*>> imp_weight;
    std::map<int, std::vector<implication*>>::iterator iw_it;

    
    for (int i=0; i<=max; i++){
        if (TmpTable[max-i] == '1' || TmpTable[max-i] == 'd'){
            implication* mini = new implication(TranToVar(i,NumVar));
            if (TmpTable[max-i] == 'd') diff_pos.push_back(i);
            mini->add_include(i);
            imp.push_back(mini); //weight: L->H
        }
    }
    
    for (int i=1; i<=NumVar; i++){
        tmp_imp_weight = new std::vector<implication*> ;
        imp_weight.insert(std::pair<int,std::vector<implication*>>(i,(*tmp_imp_weight)));
    }
    
    tmp_imp.clear();
    tmp_imp = imp;
    imp.clear();
    while(1){
        int max_weight=0 ;
        for (int i=0; i<tmp_imp.size(); i++){
            if (tmp_imp[i]->Weight() > max_weight) max_weight = tmp_imp[i]->Weight();
            iw_it = imp_weight.find(tmp_imp[i]->Weight());
            imp_weight[tmp_imp[i]->Weight()].push_back(tmp_imp[i]);
        }
        tmp_imp.clear();
        for (int i=0; i<imp_weight.size(); i++){
            for(it=imp_weight[i].begin(); it!=imp_weight[i].end(); it++){
                for(jt=it+1; jt!=imp_weight[i].end();){
                    if ( same((*it),(*jt)) ){
                        imp_weight[i].erase(jt);
                    }else {jt++;}
                }
            }
        }
        
        int  used = 0;
        for(int i=0; i<=max_weight; i++){
            
            if (i<max_weight){
                while (!imp_weight[i].empty()){
                    for (int k=0; k<imp_weight[i+1].size(); k++)
                        if (compare(imp_weight[i][0], imp_weight[i+1][k])){
                            tmp_imp.push_back((*imp_weight[i][0])+(*imp_weight[i+1][k]));
                            imp_weight[i][0]->S_used();
                            imp_weight[i+1][k]->S_used();
                            used = 1;
                        }
                    if (imp_weight[i][0]->isused()){
                        imp_weight[i].erase(imp_weight[i].begin());
                    }else{
                        imp.push_back(imp_weight[i][0]);
                        imp_weight[i].erase(imp_weight[i].begin());
                    }
                }
            }else{
                while (!imp_weight[max_weight].empty()){
                    if (imp_weight[max_weight][0]->isused()){
                        imp_weight[max_weight].erase(imp_weight[i].begin());
                    }else{
                        imp.push_back(imp_weight[i][0]);
                        imp_weight[max_weight].erase(imp_weight[i].begin());
                    }
                }
                
            }
            
        }
        if (used == 0) { break; }
    }
}



string ChartToExpr::GetExpr(){
    string result{""};
    string connect{""};
    string connect_tmp;
    for (int i=0; i<imp.size(); i++){
        string tmp="";
        if ( i!=0 ) connect = "|";
        connect_tmp = "";
        long max = imp[i]->G_var().size()-1;
        for(long j=max; j>=0; j--){
            char ch_tmp[1]{static_cast<char>('A'+max-j)} ;
            string cs_tmp(ch_tmp,1);
            if (tmp!="") connect_tmp = "&";
            switch (imp[i]->G_var()[j]) {
                case '1': { tmp += connect_tmp + cs_tmp; break; }
                case '0': { tmp += connect_tmp + "~" + cs_tmp ; break; }
            }
            
        }
        if (tmp != "")  result += connect + tmp ;
    }
    if (imp.size() == 1){
        int Num = 0;
        for (int i=0; i<imp[0]->G_var().size(); i++){
            if (imp[0]->G_var()[i] == '1' || imp[0]->G_var()[0] == '0')
                Num++;
        }
        if (Num == 0) result = "1";
    }
    if (imp.empty()) result = "0";
    
    return result;
}


string ChartToExpr::petrik(){
    string result="";
    std::vector <implication*>::iterator it,jt,jt_tmp;
    std::vector <int> raw_tmp;
    std::vector <int> table_tmp;
    
    for(int i=0; i<(1<<NumVar); i++){
        for (int j=0; j<imp.size(); j++){
            table_tmp.push_back(0);
        }
        minitable.push_back(table_tmp);
        table_tmp.clear();
    }
    
    for (int i=0; i<imp.size(); i++){
        for(int j=0; j<imp[i]->G_incl().size(); j++){
            int k = imp[i]->G_incl()[j];
            bool isran=false;
            for (int diff_j; diff_j<diff_pos.size(); diff_j++){
                if (k == diff_pos[diff_j]) isran = true;
            }
            if (isran == false) minitable[k][i] = 1;
        }
    }
    
    tmp_imp = imp;
    imp.clear();
    while(1){
        int i=0;
        while(i<minitable.size()){
            int countones = 0;
            for (int j=0; j<minitable[i].size(); j++){
                if (minitable[i][j] == 1) countones++;
            }
            if (countones == 0) {
               minitable.erase(minitable.begin()+i);
            }else if (countones == 1){
                int raw;
                for(raw=0; raw<minitable[i].size(); raw++){
                    if (minitable[i][raw] == 1) break;
                }
                int existed = 0;
                for(int k=0; k<raw_tmp.size(); k++){
                    if (raw_tmp[k] == raw) existed++;
                }
                if (existed == 0) raw_tmp.push_back(raw);
                i++;
            }else{i++;}
           
        }
        sort(raw_tmp.begin(),raw_tmp.end());
        for (int k=0; k<raw_tmp.size(); k++){
            for(int j=0; j<minitable.size();){
                if (minitable[j][raw_tmp[k]-k] == 1)
                    minitable.erase(minitable.begin()+j);
                else {j++;}
            }
            for(int j=0; j<minitable.size();j++)
                minitable[j].erase(minitable[j].begin()+raw_tmp[k]-k);
            imp.push_back(tmp_imp[raw_tmp[k]-k]);
            tmp_imp.erase(tmp_imp.begin()+raw_tmp[k]-k);
            
        }
        raw_tmp.clear();
        
        //step 2
        int endloop = 0;
        for (int j=0; j<minitable.size(); j++){
            int countones = 0;
            for (int k=0; k<minitable[j].size(); k++){
                if (minitable[j][k] == 1) countones++;
            }
            if (countones==1 || countones==0 ) {endloop=1;}
        }
        if (endloop == 0) {break;}
    }
    
    
    //step5
    
    long min=minitable[0].size();
    int min_j=0;
    if (minitable.size() != 0){
        SOP = Simplify(0);
        SOP.erase(unique(SOP.begin(),SOP.end()),SOP.end());
        
        for (int j=0; j<SOP.size(); j++){
            if ( CountImps(minitable[0].size(),SOP[j])<min ){
                min = CountImps(minitable[0].size(),SOP[j]);
                min_j = j;
            }
        }
        for (int j=0; j<minitable[0].size(); j++){
            if (((SOP[min_j] & (1<<j)) >>j) == 1 ) imp.push_back(tmp_imp[j]);
        }
    }
    //Simplify();
    result = GetExpr();
    return result;
}


long ChartToExpr::CountImps(const long&len,const long& val){
    long NumOfImps=0;
    for (int i=0; i<len; i++){
        if (((val & (1<<i))>>i) == 1) NumOfImps++;
    }
    return NumOfImps;
}

std::vector<int>& ChartToExpr::Simplify(const int& col){
    std::vector<int> Col_SOP;
    std::vector<int> Tmp_SOP;
    int SOP_tmp;
    if(col+1 < minitable.size()){
        Tmp_SOP=Simplify(col+1);
    }else{
        Col_SOP.clear();
        for (int i=0; i<minitable[col].size(); i++){
            if(minitable[col][i] == 1)
                Col_SOP.push_back((1<<i));
        }
        return_SOP = Col_SOP;
        return return_SOP;
    }
    Col_SOP.clear();
    sort(Tmp_SOP.begin(),Tmp_SOP.end());
    Tmp_SOP.erase(unique(Tmp_SOP.begin(),Tmp_SOP.end()),Tmp_SOP.end());
    for (int i=0; i<minitable[col].size(); i++)
        if(minitable[col][i] == 1){
            for (int j=0; j<Tmp_SOP.size(); j++){
                SOP_tmp = Tmp_SOP[j] | (1<<i);
                Col_SOP.push_back(SOP_tmp);
            }
        }
    return_SOP = Col_SOP;
    return return_SOP;
}


string ChartToExpr::solve(const string& truthtable){
    string expression = "";
    string TmpTable;
    
    TmpTable = filter(truthtable);
    for (int i=0; (1 << i) < TmpTable.length(); i++,NumVar++);
    if (TmpTable.length() != (1<<NumVar)) { throw InvaidLenError{}; }
    
    Quine_McCluskey(TmpTable);
    expression = petrik();
    
    return  expression;
}
