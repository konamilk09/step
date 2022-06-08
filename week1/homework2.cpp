/**
 * 与えられた文字列のすべての文字を使わなくてもいいようにする
 * 単語について最大のスコアを持つアナグラムを出力
 */ 

#include <bits/stdc++.h>
using namespace std;

void get_dic(char *file, vector<string> &dictionary){
    string fin_buf; // ファイルから辞書の単語を受け取る用バッファ
    ifstream fin;
    fin.open(file, ios::in);
    while(!fin.eof()) {
        getline(fin, fin_buf);
        dictionary.push_back(fin_buf);
    }
    fin.close();
}

// 文字ごとに何個あるか保持する
void count_dic(vector<string> &dictionary, vector<pair<vector<int>, string>> &new_dictionary) {
    int i=0;
    for(string& word : dictionary) {
        vector<int> vec(26, 0);
        new_dictionary.push_back(make_pair(vec,word));
        for(int j=0; j<(int)word.size(); j++) {
            char a = word[j];
            new_dictionary[i].first.at(int(a-'a'))++;
        }
        i++;
    }
    return;
}


// 2つの文字列を受け取り、scoreが大きい方を返す
string compare_score(string &a, string &b, vector<int>& score_list) {
    int ac=0, bc=0;
    for(int i=0; i<(int)a.size(); i++) ac += score_list.at(int(a[i]-'a'));
    for(int i=0; i<(int)b.size(); i++) bc += score_list.at(int(b[i]-'a'));
    if(ac > bc) return a;
    else return b;
}

// anagramの単語を返す
string search_dic(vector<int> &random_word, vector<pair<vector<int>, string>> &new_dictionary, vector<int> &score_list) {
    string res = "a";
    for(auto& [letter_list, word] : new_dictionary) {
        bool flag = true;
        for(int i=0; i<26; i++) {
            if(random_word[i] < letter_list[i]) { flag = false; break;} // dicのよりrandom_wordの数が少ないとき
        }
        if(flag) res = compare_score(res, word, score_list); // scoreが大きい方をresに入れる
    }
    return res;
}

void search_vec(vector<pair<vector<int>, string>> &new_str, vector<pair<vector<int>, string>> &new_dictionary, vector<string> &answers, vector<int>& score_list) {

    for(int i=0; i<(int)new_str.size(); i++) {
        answers.push_back(search_dic(new_str[i].first, new_dictionary, score_list));
    }

    return;
}

// 答えをファイルに出力する
void output_answers(vector<string> &vec, char *file) {
    ofstream fout;
    fout.open(file, ios::out);
    for(auto ans : vec) fout << ans << endl;
    fout.close();
}


int main(int argc, char** argv) {

    if(argc != 4) {
        printf("Usage: $ ./a dic_file word_file out_file\n");
    }

    vector<int> score_list = {1, 3, 2, 2, 1, 3, 3, 1, 1, 4, 4, 2, 2, 1, 1, 3, 4, 1, 1, 1, 2, 3, 3, 4, 3, 4};

    // 辞書対応
    vector<string> dictionary; // 辞書の単語を入れる
    vector<pair<vector<int>, string>> new_dictionary; //new_dictionaryをアルファベットごとにカウントした

    // 辞書を配列に入れる
    get_dic(argv[1], dictionary);
    // アルファベットごとに何個あるかを保持
    count_dic(dictionary, new_dictionary);


    // 文字列対応
    vector<string> strs; // ファイルの文字列を入れる
    vector<pair<vector<int>, string>> new_str; // それぞれの文字列をアルファベットごとにカウントした

    // 文字列の受け取り
    get_dic(argv[2], strs);
    // それぞれの文字列をアルファベットごとにカウント
    count_dic(strs, new_str);


    // それぞれの文字列に対して、anagramを探す。
    vector<string> ans; // 答えを入れる場所
    search_vec(new_str, new_dictionary, ans, score_list);

    // 答えをファイルに出力
    output_answers(ans, argv[3]);

    return 0;
}
