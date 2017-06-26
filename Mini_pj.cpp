//
//  main.cpp
//  mini_project
//
//  Created by sxb on 2017/4/9.
//  Copyright © 2017年 sxb. All rights reserved.
//

#include <iostream>
#include "ExpressionToChart.h"
#include "ChartToExpression.h"
#include "simple_test.h"

std::string expr_to_truthtable(int n, const std::string& expr){
    ExprToChart ExprToChart;
    return ExprToChart.solve(n,expr);
}

std::string truthtable_to_expr(const std::string& truthtable){
    ChartToExpr ChartToExpr;
    return ChartToExpr.solve(truthtable);
}

int test_main(){
    //1、对expr_to_truthtable()进行测试
    //鲁棒性测试
    //(1)等价类1:输入字符错误
    CHECK_THROW(expr_to_truthtable(1, "I"), InvaidCharError);
    CHECK_THROW(expr_to_truthtable(3, "~A | B9 |C"),InvaidCharError);
    //(2)等价类2:语法错误
    CHECK_THROW(expr_to_truthtable(3, "~A | B C"),SyntaxError);
    CHECK_THROW(expr_to_truthtable(3, "~A | B ~C"),SyntaxError);
    //(3)等价类3:输入变量数量错误
    CHECK_THROW(expr_to_truthtable(3, "~A | B| C |D"),WrongNumVarError);
    //(4)等价类4:输入括号不对齐
    CHECK_THROW(expr_to_truthtable(3, "~A | (B |C"),ExprBracketError);
    //(5)等价类5:输入字符串为空
    CHECK_THROW(expr_to_truthtable(3, ""),ExprEmptyError);
    
    //正确性测试
    //(1)等价类1：单个输入变量测试
    CHECK_EQUAL(expr_to_truthtable(1, "A"), "10");
    //(2)等价类2：对四种运算进行测试
    CHECK_EQUAL(expr_to_truthtable(1, "~A"), "01");
    CHECK_EQUAL(expr_to_truthtable(2, "A | B"), "1110");
    CHECK_EQUAL(expr_to_truthtable(2, "A & B"), "1000");
    CHECK_EQUAL(expr_to_truthtable(2, "A ^ B"), "0110");
    //(3)等价类3：复杂不带括号的表达式
    CHECK_EQUAL(expr_to_truthtable(3, "~A | B & C"), "11010101");
    CHECK_EQUAL(expr_to_truthtable(4, "A & ~B ^ C & D"), "1101001000100010");
    CHECK_EQUAL(expr_to_truthtable(6, "~A | B | C & D | ~E ^ F"),"1111111111111111111111011101110111111101110111011111111111111111");
    CHECK_EQUAL(expr_to_truthtable(8, "~A | B | C & D | ~E & F | G &H"), "1111111111111111111111111111111111111111111111111111111111111111111111011101110111111111111111111111110111011101111111011101110111111101110111011111111111111111111111011101110111111101110111011111110111011101111111111111111111111101110111011111110111011101");
    //(4)等价类4：复杂带括号的表达式
    CHECK_EQUAL(expr_to_truthtable(3, "~(A | B) & C"), "00010000");
    CHECK_EQUAL(expr_to_truthtable(4, "A & ~(B | C) & D"), "0000001000000000");
    //带多个括号的表达式
    CHECK_EQUAL(expr_to_truthtable(8, "~((A | B) ^ C) & D | ~E & F | G &H"),"1111111111111111111111111111111111111111111111111111111111111111111000010000000011111111111111111110000100000000111000010000000011100001000000001111111111111111111000010000000011100001000000001110000100000000111111111111111111100001000000001110000100000000");
    
    
    
    //2、truthtable_to_expr()的测试
    //鲁棒性测试
    //(1)等价类1：输入字符串为空
    CHECK_THROW(truthtable_to_expr(""),TruEmptyError);
    //(2)等价类2：输入字符串超出范围
    CHECK_THROW(truthtable_to_expr("111111111111111111111111111111111111111111111111111111111111111111100001000000001111111111111111111000010000000011100001000000001110000100000000111111111111111111100001000000001110000100000000111000010000000011111111111111111110000100000000111000010000000000"),TruOutRangeError);
    //(3)等价类3：输入字符串包含非法字符
    CHECK_THROW(truthtable_to_expr("11001B11"),InvaidCharError );
    //(4)等价类4：输入真值表位数不正确
    CHECK_THROW(truthtable_to_expr("101111001"),InvaidLenError );
    
    
    //正确性测试
    //(1)输入字符串全1                        //边界值
    CHECK_EQUAL(truthtable_to_expr("1111"), "1");
    //(2)输入字符串全0                        //边界值
    CHECK_EQUAL(truthtable_to_expr("0000"), "0");
    //(3)输入字符串全为d                      //边界值
    CHECK_EQUAL(truthtable_to_expr("dddd"), "1");
    //(3)输入字符串最小(2位)                        //边界值
    CHECK_EQUAL(truthtable_to_expr("10"), "A");
    CHECK_EQUAL(truthtable_to_expr("01"), "~A");
    CHECK_EQUAL(truthtable_to_expr("1d"), "1");
    //(4)输入字符串最大（256位）                      //边界值
    //CHECK_EQUAL(truthtable_to_expr("0001111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"), "~A&~B|~H|~G|~F|~E|~D|~C");
        //该用例较为特殊，耗时大约 1s
    //CHECK_EQUAL(truthtable_to_expr("0001111011101111111110111101110111011111011110111111111111011111111111011111111011111111101111011011111111110111111111101111111101101111111110111101111111101111111111011011111101111011110111111110111101101110111011111111110111101111110111110111101111110111"), "");
        //该用例较为复杂，耗时大于 15min，且内存占用较大，故暂停了程序，没有得到结果。
    
    //(5)正常输入字符串
    CHECK_EQUAL(truthtable_to_expr("11011001"), "~A&~B|A&B|~A&C");
    CHECK_EQUAL(truthtable_to_expr("1101dd01"), "~A|B");
    CHECK_EQUAL(truthtable_to_expr("1011011101010100"), "A&C&D|~B&D|~A&B&~C|~A&C&~D");
    CHECK_EQUAL(truthtable_to_expr("00101010111011010110101111111100001100010010100100110101000100000010111011101101011010111111111100110001011010010011010110010001"),"A&B&C&~D&~E&~G|~A&B&C&~D&E&~G|~A&~C&D&~E&~F|A&B&~C&~D&E|~A&B&C&~E&F|B&~C&E&F&~G|A&~B&C&E|B&~D&F|~A&~B&~E&~F&~G|~A&~B&C&~E&~F|~A&~B&~C&D&~E|~A&~B&~C&~D&E|~B&~C&~E&F&~G|~A&~B&D&~F|C&~D&~E&F|A&~B&C&D|A&~C&D&F");
        //128位真值表，耗时大概为12s

    
    
    //3、交叉测试
    //(1)truthtable_to_expr(expr_to_truthtable())
    CHECK_EQUAL(truthtable_to_expr(expr_to_truthtable(4, "A & ~B | C & D")), "A&~B|C&D");
    
    //(2)expr_to_truthtable(truthtable_to_expr())
    CHECK_EQUAL(expr_to_truthtable(4, truthtable_to_expr("1111001000100010")),"1111001000100010");
    
    
    return 0;
}
