/**
 * STEP2022 week3 homework1
 * 加減乗除ができる電卓を、モジュール化を意識して作る。
 */
#include <iostream>
#include <string>
#include <vector>
#include <utility>

typedef enum element_type {NUMBER, PLUS, MINUS} element_type;

// a structure to have a number or a operator(+-*/)
typedef struct element {
    element_type type;
    union {
        float number; // number
        element_type t; // operator
    } content;
        
} element;

// change the line into tokens 
// a token is a number or an operator
std::vector<element> tokenize(std::string line) {
    std::vector<element> tokens;
    element token;
    for(int i=0; i<(int)line.size(); i++) {
        if(line[i].isdigit()) {
            token
        }
    }
    return tokens;
}

// std::vector<element> multiply_devide(std::vector<element> tokens) {
//     std::vector<element> multipled_devided_tokens;
//     return multipled_devided_tokens;
// }

// float add_subtract(std::vector<element> multipled_devided_tokens) {
//     float answer = 0;
//     return answer;
// }


int main() {
    std::cout << "> ";
    std::string line;
    std::cin >> line;

    element a(NUMBER, 1.0);

    // 符号は符号の種類、数字は数を持つ
    std::vector<element> tokens;
    tokens = tokenize(line);
    // std::vector<element> multipled_devided_tokens;
    // multipled_devided_tokens = multiply_devide(tokens);
    
    // float answer = add_subtract(multipled_devided_tokens);
    // std::cout << answer << std::endl;
    return 0;
}
