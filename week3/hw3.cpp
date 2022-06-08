/**
 * STEP2022 week3 homework3
 * 加減乗除ができる電卓を、モジュール化を意識して作る。
 * variant を使った版
 */
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <stdio.h>

typedef enum operator_type {PLUS, MINUS, MULTI, DIVIS} operator_type;
typedef enum bracket {BEGIN, END} bracket;

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

bracket read_bracket_begin(int &index) {
    index++;
    return BEGIN;
}

bracket read_bracket_end(int &index) {
    index++;
    return END;
}

// line を tokens （数字と演算子を区別して保持している配列）に変換する
std::vector<std::variant<double, operator_type, bracket>> tokenize(const std::string &line) {

    // 返り値
    std::vector<std::variant<double, operator_type, bracket>> tokens;
    // 計算しやすいようにダミーの PLUS
    tokens.push_back(PLUS);
    
    std::variant<double, operator_type, bracket> token;
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
        else if(line[index] == '(') {
            token = read_bracket_begin(index);
        }
        else if(line[index] == ')') {
            token = read_bracket_end(index);
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
double calc_multi_divis(const std::vector<std::variant<double, operator_type, bracket>> &tokens, int &index) {
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
std::vector<std::variant<double, operator_type, bracket>> multiply_divide(const std::vector<std::variant<double, operator_type, bracket>> &tokens) {
    // 返り値
    std::vector<std::variant<double, operator_type, bracket>> multi_divided_tokens;
    
    for(int index = 1; index <= (int)tokens.size(); index++) {
    
        std::variant<double, operator_type, bracket> token;
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
double add_subtract(const std::vector<std::variant<double, operator_type, bracket>> &tokens) {
    double answer = 0;

    for(int index = 1; index < (int)tokens.size();) {
        // もし tokens[i] が数字なら
        if(std::holds_alternative<double>(tokens[index])) {
            double number = std::get<double>(tokens[index]); 
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

// 再帰
// 括弧の始まりの1つ次の index と tokens を受け取って、括弧の終わりまでを計算して、token にして返す
std::variant<double, operator_type, bracket> calc_bracket(
    const std::vector<std::variant<double, operator_type, bracket>> &tokens, int &index) {
    
    // これから計算する tokens 
    std::vector<std::variant<double, operator_type, bracket>> to_calc_tokens;
    // 最初の数字を見た時にひとつ前の operator_type を見て計算できるように、PLUSのダミーを入れる
    to_calc_tokens.push_back(PLUS);
    std::variant<double, operator_type, bracket> token;
    double answer;

    for(;index < (int)tokens.size(); index++) {
        if(std::holds_alternative<bracket>(tokens[index])){
            // 停止性：終わりの括弧を受け取ったら、計算してループを抜ける
            if(std::get<bracket>(tokens[index]) == END) {
                answer = add_subtract(multiply_divide(to_calc_tokens));
                break;
            }
            // 始まりの括弧を受け取ったら、
            else if(std::get<bracket>(tokens[index]) == BEGIN) {
                index++;
                // 再帰（=括弧の終わりまでを計算して、token にして返す）
                token = calc_bracket(tokens, index);
                to_calc_tokens.push_back(token);
            }
        }
        else to_calc_tokens.push_back(tokens[index]);
    }

    token = answer;
    return token;
}


// tokens を受け取って、括弧の中身を計算した tokens を返す
std::vector<std::variant<double, operator_type, bracket>> no_bracket(
    const std::vector<std::variant<double, operator_type, bracket>> &tokens) {

    std::vector<std::variant<double, operator_type, bracket>> no_bracket_tokens;
    std::variant<double, operator_type, bracket> token;

    for(int index = 0; index < (int)tokens.size(); index++) {

        // 括弧の始まりを受け取ったら
        if(std::holds_alternative<bracket>(tokens[index]) 
            && std::get<bracket>(tokens[index]) == BEGIN) {
            // ( の次の数を見る
            index++;
            // 括弧の中身を計算し、token にする
            token = calc_bracket(tokens, index);
            no_bracket_tokens.push_back(token);
        }

        else {
            no_bracket_tokens.push_back(tokens[index]);
        }
    }

    return no_bracket_tokens;
}

// テストを行う
void test(const std::string line) {

    // 文字列を数字と演算子で区切り、それぞれラベルをつける
    std::vector<std::variant<double, operator_type, bracket>> tokens;
    tokens = tokenize(line);
    
    // 括弧の中身を計算する
    std::vector<std::variant<double, operator_type, bracket>> no_bracket_tokens;
    no_bracket_tokens = no_bracket(tokens);

    // 乗除を計算する
    std::vector<std::variant<double, operator_type, bracket>> multi_divided_tokens;
    multi_divided_tokens = multiply_divide(no_bracket_tokens);

    // 加減を計算し、答えとなる
    double answer = add_subtract(multi_divided_tokens);

    // 出力
    std::cout << line << " = ";
    std::cout << answer << std::endl;
}

// テストケース
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

    test("2*(2-1)"); // 括弧が1つ
    test("(2-1)*2"); // 括弧が最初に来る
    test("3+4*(2-1)"); // 少し長く
    test("2*(5+2*(3-1))"); // 括弧が2つ
    test("(3+4*(2-1))/5"); // 括弧が2つの後に割り算
    printf("==== Test finished! ====\n");
}

int main() {

    run_test();

    return 0;
}
