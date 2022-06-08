/**
 * 与えられた文字列のアナグラムを辞書ファイルから探して
 * 見つかった単語全部を答える
 */ 

#include <bits/stdc++.h>
using namespace std;

void get_dic(vector<string> &dictionary, char *file){
    string fin_buf; // ファイルから辞書の単語を受け取る用バッファ
    ifstream fin;
    fin.open(file, ios::in);
    while(!fin.eof()) {
        getline(fin, fin_buf);
        dictionary.push_back(fin_buf);
    }
    fin.close();
}

// 辞書を並び替え、新しい辞書に入れる
void sort_dic(vector<string> &dictionary, vector<pair<string, string>> &new_dictionary) {
    // 辞書の単語ごとの並び替え
    // O(NlogN) + 単語自体のソートO(NlogM)
    for(string &word : dictionary) {
        string old_word = word;
        sort(word.begin(), word.end());
        string new_word = word;
        new_dictionary.push_back(make_pair(new_word, old_word)); // key=並び替え後の単語、value=並び替え前の単語
    }
    
    // 辞書全体の並び替え
    sort(new_dictionary.begin(), new_dictionary.end());
    return;
}

// anagramの単語のインデックスを返す
int binary_search(string &target, vector<pair<string, string>> &new_dictionary) {
    string ans;
    int lo=-1, hi=84093;
    int mid;
    while(hi-lo > 1) {
        mid = lo + (hi-lo) / 2;
        if(target < new_dictionary[mid].first) hi = mid;
        else if(target > new_dictionary[mid].first)lo = mid;
        else break;
    }
    return mid;
}


int main(int argc, char** argv) {
    // 文字列の受け取り
    string random_word;
    cout << "write word: ";
    cin >> random_word;
    // 文字列のソート
    string sorted_random_word = random_word;
    sort(sorted_random_word.begin(), sorted_random_word.end());

    vector<string> dictionary; // 辞書の単語を入れる
    vector<pair<string, string>> new_dictionary; // 辞書の単語を並び変えたものを、それをさらに並び替えた

    // 辞書を配列に入れる
    get_dic(dictionary, argv[1]);

    // 辞書のそれぞれの単語と、辞書全体の並び替え
    sort_dic(dictionary, new_dictionary);

    // anagramの単語のインデックスを返す
    int base_idx = binary_search(sorted_random_word, new_dictionary);
    int idx = base_idx;
    string base_str = new_dictionary[idx].first; // 見つかったanagram
    vector<string> anagram; // 答えを入れる場所
    while (idx < (int)new_dictionary.size() && base_str == new_dictionary[idx].first) {
        anagram.push_back(new_dictionary[idx].second);
        idx++;
    }
    // 下のインデックスより小さい部分を考える
    idx = base_idx-1;
    while (idx > -1 && base_str == new_dictionary[idx].first) {
        anagram.push_back(new_dictionary[idx].second);
        idx--;
    }

    int count = 0; // アナグラムの数を数える
    cout << "anagram of \"" << random_word << "\": " << endl;
    for(auto ans : anagram) {
        if(ans == random_word) continue;
        cout << ans << endl;
        count++;
    }
    if(count == 0) cout << "nothing" << endl;

    // デバッグ用：new辞書の中身をファイルに出力
    ofstream fout;
    fout.open(argv[2], ios::out);
    for(const auto& [k, v] : new_dictionary) {
        fout << k << ", " << v << endl;
    }
    fout.close();

    return 0;
}
