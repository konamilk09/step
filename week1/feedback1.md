課題1

/**
 * 与えられた文字列のアナグラムを辞書ファイルから探して
 * 見つかった単語全部を答える
 */

#include <bits/stdc++.h>

bits/stdc++.h は環境依存なので、使いどころにはすごく気を使うと良いかと思います。
1回書きっきりのプログラムで自分しか使わない、等用途が明確であればよいかと思いますが、
そうでない場合は、使用を避けるか、もしくは意図を持って使用する場合はコメント等を書いておくと良いかと思います。
 
using namespace std;

using namespace std も、特に最初の頃に"おまじない"として導入されたりします。
が、今後より大きなプログラム(例えば複数ファイルにまたがるようなもの)を書く場合には注意する必要があります。
特に std の中に何が定義されているかは、仕様上決まっているものを除いて実装依存なので、
突然名前の衝突がおこったり、また予期しない overload 等の解決につながったりする原因になり、
謎の compile error に悩まされたりすることがあったりします。
 

void get_dic(vector<string> &dictionary, char *file){

get の多くは副作用を伴わない getter に使われることが多いかと思いますが、
ここでは file から読むので "read_dictionary" 等として見るほうが、よりこの関数が何をしているかを説明できるかもしれません。

char* file ですが、c++ を使っているので、const std::string& を使うほうが良いかと思います。

また vector<string>& を辞書を返す buffer として使っていますが、特に C++11 以降を使っていることとしてよいのであれば、
vector<string> read_dictionary(const std::string& file) 
のように返り値で扱うほうが、何が入力で何が出力かがはっきりしてよいかと思います。
古い C++ では、値で返す際に、中身のコピーが起きたので引数で buffer を渡す、ということが行われていましたが
(歴史のあるコードでは今でもたくさん見受けられます)、新しいコードではそこまでコストが高くないか、
もしくは NRVO (Named Return Value Optimization) という最適化等によって、コストがかからなくなっていたりします。
 
    string fin_buf; // ファイルから辞書の単語を受け取る用バッファ
    ifstream fin;
    fin.open(file, ios::in);

ここで、file が開けているかのチェックもできると良いかもしれません。
 
    while(!fin.eof()) {
        getline(fin, fin_buf);

eof の check は getline のあとに行う必要があるかと思います。
 
        dictionary.push_back(fin_buf);
    }
    fin.close();
}

// 辞書を並び替え、新しい辞書に入れる
void sort_dic(vector<string> &dictionary, vector<pair<string, string>> &new_dictionary) {
    // 辞書の単語ごとの並び替え
    // O(NlogN) + 単語自体のソートO(NlogM)

計算量のコメントがあるのは良いですね。
1 点付け加えるとすると、何が N で、何が M かを書いておくとよりよいかと思います。
 
    for(string &word : dictionary) {
        string old_word = word;
        sort(word.begin(), word.end());

ここの処理で、dictionary の中身のデータが書き換わっています。
これ以降 dictionary は使われないのですが、予期せぬ副作用はなるべく避けたほうがよいと思います。
この場合は、コピーしたあと、new_word を sort してやればよいかと思います。
また、引数で渡したデータが予期せず変わらない、ということを明示する意味でも、const をつけると良いかと思います。

std::vector<std::pair<std::string, std::string>> sort_dictionary(const std::vector<std::string>& dictionary) {
  ...
  for (const std::string& word : dictionary) {
   ...

 
        string new_word = word;
        new_dictionary.push_back(make_pair(new_word, old_word)); // key=並び替え後の単語、value=並び替え前の単語

Optional: ここでは emplace_back を使うことで、make_pair を省略できます。
また、コピーする必要はないので、move してしまうと文字列のコピーのコストを省略することができます。

new_dictionary.emplace_back(std::move(new_word), std::move(old_word));
 
    }
   
    // 辞書全体の並び替え pairだから計算量ちょっと多いと思う

単語の使い方ですが、計算量は N * M log M (N = 単語の長さ M = 辞書のサイズ) なので、計算量が増える、ことはないかと思います。
実測の計算時間はちょっと掛かるかもしれませんね。
 
    sort(new_dictionary.begin(), new_dictionary.end());
    return;
}

// anagramの単語のインデックスを返す
int binary_search(string &target, vector<pair<string, string>> &new_dictionary) {
    string ans;
    int lo=-1, hi=84093;

ここで、辞書のサイズを固定で決め打ちしていますが、ここは辞書のサイズから計算すると良いかと思います。
 
    int mid;
    while(hi-lo > 1) {
        mid = lo + (hi-lo) / 2;

ここの overflow を避ける technich は大変よいですね!
汎用的なプログラムを書くときに特に重要になってきます。
 
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


ここで、解が複数ある場合の処理をしていて、両側を探す処理をしていますね。
この方法でもよいのですが、他の方法として、binary_search を 1 つ見つかった場合その場で返すのではなく、
境界値を見つけるまで binary_search を続行する、という方法があります。
STL の std::lower_bound や upper_bound がそのあたりに当たります。
境界値が返ってくると、その後の探索は片側でよくなったり、また STL には std::equal_range というそのままな API もあります。
ぜひ調べて、手数を増やしてみていただけたらと思います。
 
    int count = 0; // アナグラムの数を数える
    cout << "anagram of \"" << random_word << "\": " << endl;
    for(auto ans : anagram) {

ここで、ans に対して文字列のコピーが起きていますが、この場合は必要ないかとおもうので、

for (const auto& ans: anagram)

等としてみるとよいかと思います。
 
        if(ans == random_word) continue;
        cout << ans << endl;
        count++;
    }
    if(count == 0) cout << "nothing" << endl;


ここの表示は counter を用いなくても、anagram.empty() を使うことで check できます。

 
    return 0;
}

課題2
 
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

ここの文字コードの差分から index に変換する方法は ASCII/UTF-8 等を仮定してよく用いられる方法ですね :-)

        }
        i++;

ここの loop は、push_back するのを内側の loop のあとにもってくることで、若干コードが単純になります。

for (const std::string& word : dictionary) {
  std::vector<int> vec(26, 0);
  for (char c : word)
    vec[c - 'a']++;
  new_dictionary.emplace_back(std::move(vec), word);
}

とすると、index access もなくなるので、i / j をうっかり取り違える、みたいなミスの可能性もなくなります。
 
    }
    return;
}


// 2つの文字列を受け取り、scoreが大きい方を返す
string compare_score(string &a, string &b, vector<int>& score_list) {
    int ac=0, bc=0;
    for(int i=0; i<(int)a.size(); i++) ac += score_list.at(int(a[i]-'a'));

この行で score の計算をしていますが、score の計算自体は独立した処理、と考えることができるので、
十分単純ではありますが、そこも関数に切り出してしまうのもありかとおもいます。
 
    for(int i=0; i<(int)b.size(); i++) bc += score_list.at(int(b[i]-'a'));
    if(ac > bc) return a;
    else return b;
}

// anagramの単語を返す

ここのコメントはもう少し詳しいと良いかもしれません。
この関数は単純に anagram を返すのではなく、score が最大のもの、を返すというのが一つ大事なポイントかと思います。
なので、そこにも触れたほうがより良くなるかと思います。
関数名も、find_best_score_anagram 的にしてしまってもよいかもしれません。
 
string search_dic(vector<int> &random_word, vector<pair<vector<int>, string>> &new_dictionary, vector<int> &score_list) {
    string res = "a";
    for(auto& [letter_list, word] : new_dictionary) {
        bool flag = true;

flag は、よく使われる名前ですが(将来大きなプログラム等を書くことを考えると)もう少し意味を持った名前にしてみてもよいかもしれません。
この場合は、辞書の単語が作れるか否か、みたいな使われ方をしているので、is_constructible みたいな名前でもよいかもしれません。

 
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

ここは return して置かないと、後続のプログラムが実行されてしまい、crash したり未定義動作をしたりすることになってしまいます。
 
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