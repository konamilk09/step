/**
 * STEP2022 week2 
 * homework4
 * <URL, webページ>が与えられたときに、
 * キャッシュにそれが存在した場合、それを最新のものとして保持する。
 * キャッシュに存在しない場合、一番古いものを削除し、
 * 与えられたものをキャッシュの最新に入れる。
 * このようなデータ構造を、ハッシュテーブルに双方向連結リストを加えたものとして書いた。
 * *挿入時に最初に個数で場合分けしている版
 */ 
#include <iostream>
#include <string>

class Node {
public:
    std::string url;
    std::string web;
    Node *prev;
    Node *next;
    Node(std::string, std::string);
};

Node::Node(std::string Url, std::string Web) {
    url = Url;
    web = Web;
    prev = nullptr;
    next = nullptr;
}

class HashTable {
    Node **table;
    int size;
public:
    Node *head;
    Node *tail;
    int count;
        
    HashTable(int); // 初期化
    int HashFunc(int) const; // ハッシュ関数
    void update_exist(int); // 挿入で使う。
    void insert(std::string, std::string); // 挿入
    void displayHash(); // 表示。デバッグ用
};

// ハッシュ関数
int HashTable::HashFunc(int key) const{
    return (key % 26);
}

// ハッシュテーブルの初期化。空のハッシュテーブルを作る
HashTable::HashTable(int n) {
    table = new Node*[26];
    for(int i=0; i<26; i++) {
        table[i] = nullptr;
    }
    head = nullptr;
    tail = nullptr;
    size = n;
    count = 0;
}

// 新しいものがすでにハッシュテーブルにあったときの、ハッシュテーブルとリストの更新
void HashTable::update_exist(int index) {
    // 新しいものが tail だったら
    if(table[index] == tail) {
        // tail を更新する 
        tail = tail->prev;
        tail->next = nullptr;
        // table[index] をリストの元の位置から削除
        table[index]->prev->next = nullptr;
        
    }
    // 新しいものが head だったらなにもしない
    // tail でも head でもなければ
    else if(table[index] != head) {
        // table[index] をリストの元の位置から削除
        table[index]->prev->next = table[index]->next;
        table[index]->next->prev = table[index]->prev;
    }
    // 新しいものが head でなければ
    if(table[index] != head) {
        // head を更新
        table[index]->next = head;
        head->prev = table[index];
        head = table[index];
        table[index]->prev = nullptr;
    }
}

// 挿入
void HashTable::insert(std::string url, std::string web) {
    // 今回は、簡単のためサイズが26のハッシュテーブルにしたのでハッシュ関数を通さなくても可能だが、
    // ハッシュテーブルのサイズを変えてもできるようにしたいし、ハッシュテーブルっぽくしたい、のでハッシュ関数に通す。
    int index = HashFunc(int(url[0]-'a'));
    // 保持する url の数に達していないとき
    if(count < size) {
        // ハッシュテーブルになかった場合
        if(table[index] == nullptr) {
            table[index] = new Node(url, web);
            if(head != nullptr) {
                table[index]->next = head;
                head->prev = table[index];
            }
            head = table[index];
            if(tail == nullptr) tail = table[index];
            count++;
        }
        // ハッシュテーブルにすでに存在していた場合
        else if(table[index]->url == url) {
            // 新しいものを head にし、新しいものがもともとあった場所を繋ぎ直す。
            update_exist(index);
        }
    }
    else {
        // ハッシュテーブルになかった場合
        if(table[index] == nullptr) {
            table[index] = new Node(url, web);
            table[index]->next = head;
            head->prev = table[index];
            head = table[index];
            // tail を table から削除し、新しいものに変更する。
            int tail_index = HashFunc(int(tail->url[0]-'a'));
            table[tail_index] = nullptr;
            tail = tail->prev;
            tail->next = nullptr;
        }
        // ハッシュテーブルにすでに存在していた場合
        else if(table[index]->url == url) {
            // 新しいものを head にし、新しいものがもともとあった場所を繋ぎ直す。
            update_exist(index);
        }
    }
    return;
}

void HashTable::displayHash() {
    // ハッシュテーブルの表示
    std::cout << "HashTable:" << std::endl;
    for(int i = 0; i < 26; i++) {
        std::cout << "table[" << i << "]";
        if(table[i] != nullptr) std::cout << table[i]->url;
        std::cout << std::endl;
    }
    // 前から連結リストを表示
    std::cout << "head -> tail:" << std::endl;
    Node* i;
    i = head;
    std::cout << "[";
    while(i != tail) {
        std::cout << i->url << ", ";
        i = i->next;
    }
    std::cout << i->url << "]";
    std::cout << std::endl;
    // 後ろから連結リストを表示
    std::cout << "tail -> head:" << std::endl;
    i = tail;
    std::cout << "[";
    while(i != head) {
        std::cout << i->url << ", ";
        i = i->prev;
    }
    std::cout << i->url << "]";
    std::cout << std::endl;
}

int main() {
    // テスト
    HashTable h(4);
    h.insert("a.com", "AAA");
    h.insert("b.com", "BBB");
    h.insert("c.com", "CCC");
    h.displayHash();
    
    h.insert("a.com", "AAA");
    h.displayHash();

    h.insert("e.com", "EEE");
    h.displayHash();

    h.insert("s.com", "SSS");
    h.displayHash();
    
    h.insert("s.com", "SSS");
    h.displayHash();

    h.insert("a.com", "AAA");
    h.displayHash();

    return 0;
}
