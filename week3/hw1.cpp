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
// line[index]から読み込んだ数字を返す
double read_number(const std::string &line, int &index) {
    double number = 0.0;
    // line[index]の1文字が数字なら以前のものを10倍して加える
    while(index<(int)line.size() && std::isdigit(line[index])) {
        number = number*10 + int(line[index]-'0');
        index++;
    }
    // 小数点が来たら
    if(index<(int)line.size() && line[index]=='.') {
        double decimal = 0.1;
        index++;
        // 小数点以下を1文字分ずつ number に加える
        while(index<(int)line.size() && std::isdigit(line[index])) {
            number += double(line[index] - '0') * decimal;
            decimal /= 10;
            index++;
        }
    }
    return number;
}

// index を一つ増やして token 用の operator_type を返す
operator_type read_plus(int &index) {
    index++;
    return PLUS;
}

operator_type read_minus(int &index) {
    index++;
    return MINUS;
}

operator_type read_multi(int &index) {
    index++;
    return MULTI;
}

operator_type read_divis(int &index) {
    index++;
    return DIVIS;
}

// line を tokens （数字と演算子を区別して保持している配列）に変換する
std::vector<std::variant<double, operator_type>> tokenize(const std::string &line) {

    // 返り値
    std::vector<std::variant<double, operator_type>> tokens;
    // 計算しやすいようにダミーの PLUS
    tokens.push_back(PLUS);
    
    std::variant<double, operator_type> token;
    // line を読む
    for(int index = 0; index < (int)line.size();) {
        // line[index] を double か operator_type にして token に入れる
        if(std::isdigit(line[index]) == 1) {
            token = read_number(line, index);
        }
        else if(line[index] == '+') {
            token = read_plus(index);
        }
        else if(line[index] == '-') {
            token = read_minus(index);
        }
        else if(line[index] == '*') {
            token = read_multi(index);
        }
        else if(line[index] == '/') {
            token = read_divis(index);
        }
        else {
            printf("invalid character found: %c\n", line[index]);
            exit(EXIT_FAILURE);
        }
        tokens.push_back(token);
    }
    return tokens;
}

// 乗除を計算する
double calc_multi_divis(const std::vector<std::variant<double, operator_type>> &tokens, int &index) {
    // 一つ目の乗除の前の数字を持っておく
    double result = std::get<double>(tokens[index-1]);

    // 乗除の記号があったら
    while (index < (int)tokens.size()-1 
            && (std::get<operator_type>(tokens[index]) == DIVIS
            || std::get<operator_type>(tokens[index]) == MULTI)) {

        // 割り算
        if(std::get<operator_type>(tokens[index]) == DIVIS) {
            // ゼロ除算処理
            if(std::get<double>(tokens[index+1]) == 0) {
                printf("Error: Zero division\n");
                exit(EXIT_FAILURE);
            }
            result = result / std::get<double>(tokens[index+1]);
        }
        // 掛け算
        if(std::get<operator_type>(tokens[index]) == MULTI) {
            result = result * std::get<double>(tokens[index+1]);
        }
        // 次の演算子へ
        index+=2;
    }
    return result;
}

// 乗除の部分を計算した tokens を返す
std::vector<std::variant<double, operator_type>> multiply_divide(std::vector<std::variant<double, operator_type>> &tokens) {
    // 返り値
    std::vector<std::variant<double, operator_type>> multi_divided_tokens;
    
    for(int index = 1; index <= (int)tokens.size(); index++) {
    
        std::variant<double, operator_type> token;
        // 乗除の演算子があったら
        if (index < (int)tokens.size()-1 
            && std::holds_alternative<operator_type>(tokens[index])
            && (std::get<operator_type>(tokens[index]) == DIVIS
            || std::get<operator_type>(tokens[index]) == MULTI)) {

            token = calc_multi_divis(tokens, index);
            // 返り値に token を追加
            multi_divided_tokens.push_back(token);
        }
        
        else {
            // 乗除する数字を計算する前に返り値に追加しないように tokens[index-1] を追加
            multi_divided_tokens.push_back(tokens[index-1]);
        }
    }

    return multi_divided_tokens;
}

// 加減算を行い、答えを返す
double add_subtract(std::vector<std::variant<double, operator_type>> &tokens) {
    double answer = 0;

    for(int index = 1; index < (int)tokens.size();) {
        // もし tokens[i] が数字なら
        if(std::holds_alternative<double>(tokens[index])) {
            double& number = std::get<double>(tokens[index]); 
            // 数字のひとつ前の演算子を見て計算する
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

int main() {
    // 入力受け取り
    std::cout << "> ";
    std::string line;
    std::cin >> line;

    // 文字列を数字と演算子で区切り、それぞれラベルをつける
    std::vector<std::variant<double, operator_type>> tokens;
    tokens = tokenize(line);

    // 乗除を計算する
    std::vector<std::variant<double, operator_type>> multi_divided_tokens;
    multi_divided_tokens = multiply_divide(tokens);
    
    // 加減を計算し、答えとなる
    double answer = add_subtract(multi_divided_tokens);
    std::cout << answer << std::endl;

    return 0;
}
