/**
 * STEP2022 week3 homework1
 * 加減乗除ができる電卓を、モジュール化を意識して作る。
 * variant を使った版
 */
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <stdio.h>

typedef enum operator_type {PLUS, MINUS, MULTI, DIVIS} operator_type;

// read a number from the line[index] and return a double number
double read_number(const std::string &line, int &index) {
    double number = 0.0;
    while(index<(int)line.size() && std::isdigit(line[index])) {
        number = number*10 + int(line[index]-'0');
        index++;
    }
    if(index<(int)line.size() && line[index]=='.') {
        double decimal = 0.1;
        index++;
        while(index<(int)line.size() && std::isdigit(line[index])) {
            number += double(line[index] - '0') * decimal;
            decimal /= 10;
            index++;
        }
    }
    return number;
}

operator_type read_plus(const std::string &line, int &index) {
    index++;
    return PLUS;
}

operator_type read_minus(const std::string &line, int &index) {
    index++;
    return MINUS;
}

operator_type read_multi(const std::string &line, int &index) {
    index++;
    return MULTI;
}

operator_type read_divis(const std::string &line, int &index) {
    index++;
    return DIVIS;
}

// change the line into tokens 
std::vector<std::variant<double, operator_type>> tokenize(const std::string &line) {
    std::vector<std::variant<double, operator_type>> tokens;
    tokens.push_back(PLUS);
    // a token is a number or an operator
    std::variant<double, operator_type> token;
    // read line
    for(int index = 0; index < (int)line.size();) {
        // if line[index] is a number
        if(std::isdigit(line[index]) == 1) {
            token = read_number(line, index);
        }
        else if(line[index] == '+') {
            token = read_plus(line, index);
        }
        else if(line[index] == '-') {
            token = read_minus(line, index);
        }
        else if(line[index] == '*') {
            token = read_multi(line, index);
        }
        else if(line[index] == '/') {
            token = read_divis(line, index);
        }
        else {
            printf("invalid character found: %c\n", line[index]);
            exit(EXIT_FAILURE);
        }
        tokens.push_back(token);
    }
    return tokens;
}

double calc_multi_divis(const std::vector<std::variant<double, operator_type>> &tokens, int &index) {
    double result = std::get<double>(tokens[index-1]);

    while (index < (int)tokens.size()-1 
            && (std::get<operator_type>(tokens[index]) == DIVIS
            || std::get<operator_type>(tokens[index]) == MULTI)) {

        if(std::get<operator_type>(tokens[index]) == DIVIS) {
            if(std::get<double>(tokens[index+1]) == 0) {
                printf("Error: Zero division\n");
                exit(EXIT_FAILURE);
            }
            result = result / std::get<double>(tokens[index+1]);
        }
        if(std::get<operator_type>(tokens[index]) == MULTI) {
            result = result * std::get<double>(tokens[index+1]);
        }
        index+=2;
    }
    return result;
}

std::vector<std::variant<double, operator_type>> multiply_divide(std::vector<std::variant<double, operator_type>> &tokens) {
    
    std::vector<std::variant<double, operator_type>> multi_divided_tokens;
    
    for(int index = 1; index <= (int)tokens.size(); index++) {
    
        std::variant<double, operator_type> token;
        // if tokens[i] is a operator type MULTI or DIVIS
        if (index < (int)tokens.size()-1 
            && std::holds_alternative<operator_type>(tokens[index])
            && (std::get<operator_type>(tokens[index]) == DIVIS
            || std::get<operator_type>(tokens[index]) == MULTI)) {

            token = calc_multi_divis(tokens, index);
            multi_divided_tokens.push_back(token);
        }
        
        else {
            multi_divided_tokens.push_back(tokens[index-1]);

            // if(std::holds_alternative<operator_type>(tokens[index-1]))
            //     printf("push_back this: ope: %d\n", std::get<operator_type>(tokens[index-1]));

            // if(std::holds_alternative<double>(tokens[index-1]))
            //     printf("push_back this: num: %f\n", std::get<double>(tokens[index-1]));
        }
    }

    return multi_divided_tokens;
}

double add_subtract(std::vector<std::variant<double, operator_type>> &tokens) {
    double answer = 0;
    for(int index = 1; index < (int)tokens.size();) {
        // if tokens[i] is a number
        if(std::holds_alternative<double>(tokens[index])) {
            double& number = std::get<double>(tokens[index]); 
            // look at the tokens[i-1]'s operator type
            if(std::holds_alternative<operator_type>(tokens[index-1])) {
                if(std::get<operator_type>(tokens[index-1]) == PLUS) {
                    answer += number;
                }
                else if(std::get<operator_type>(tokens[index-1]) == MINUS) {
                    answer -= number;
                }
                else {
                    std::cout << "Invalid syntax" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        index++;
    }
    return answer;
}

void test(const std::string line) {
    std::vector<std::variant<double, operator_type>> tokens;
    tokens = tokenize(line);
    // 
    std::vector<std::variant<double, operator_type>> multi_divided_tokens;

    multi_divided_tokens = multiply_divide(tokens);

    double answer = add_subtract(multi_divided_tokens);

    std::cout << line << " = ";
    std::cout << answer << std::endl;
}

void run_test() {
    printf("==== Test started! ====\n");
    test("1+2-3"); // 小数の加減算
    test("12.34+20-3.57"); // 小数の加減算
    test("1/2"); // 除算
    test("5*2"); // 乗算
    test("1+6/3-5*2"); // 加減乗除の結合
    test("1+20/5*2/4"); // 乗除が連続している時
    test("1+20/5*2/4+2"); // 乗除が連続している時+加減
    test("1/3*1.1+12+300"); // 小数の乗除
    test("1/3*1.1/0"); // 0除算
    printf("==== Test finished! ====\n");
}

int main() {
    // get line
    // std::cout << "> ";
    // std::string line;
    // std::cin >> line;

    run_test();

    // // for debug 
    // for(int i=0; i<(int)tokens.size(); i++) {
    //     // if tokens[i] has type double
    //     if(tokens[i].index()==0) {
    //         double& x = std::get<double>(tokens[i]); 
    //         std::cout << "num: " << x << std::endl;
    //     }
    //     // if tokens[i] has type operator_type
    //     else {
    //         operator_type& x = std::get<operator_type>(tokens[i]); 
    //         std::cout << "ope: " << x << std::endl;
    //     }
    // }

    // std::vector<std::variant<double, operator_type>> tokens;
    // tokens = tokenize(line);
    // double answer = add_subtract(tokens);
    // std::cout << answer << std::endl;

    // std::vector<std::variant<double, operator_type>> multi_divided_tokens;
    // multi_divided_tokens = multiply_divide(tokens);
    
    // double answer = add_subtract(multipled_divided_tokens);
    // std::cout << answer << std::endl;
    return 0;
}
