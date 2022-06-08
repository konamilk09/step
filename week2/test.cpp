#include<iostream>
#include<string>
#include<vector>

class Node {
    public:
        Node *prev;
        std::string url;
        std::string web;
        Node *next;

        Node(std::string url, std::string web) {
            prev = nullptr;
            url = url;
            web = web;
            next = nullptr;
        }
};

class HashTable {
    private:
        std::vector<Node> table(26);
    // public:
    //     HashTable() {
    //         for(int i=0; i<26: i++) {
    //             table[i] = new Node("a.com", "AAA");
    //         }
    //     }
    //     int HashFunc(int a) {
    //         return a % 26;
    //     }
        // url と webサイトを挿入
        // url があればそこを head にして、リストを繋ぎ直す
        // url の next を今の head にして、今の head の prev を urlにして、url の prev を null にして、url の prev と url の next を繋げて、head を url にする。
        // void insert(std::string url, std::string web) {
        //     int h = HashFunc(url[0]-'a');
        //     while(table[h] != NULL && table[h]->url != url) {
        //         h = HashFunc(h + 1);
        //     }
        //     if(table[h] == NULL) table[h] = new Node(url, web);
        // }
    //   int SearchKey(int k) {
    //      int h = HashFunc(k);
    //      while (table[h] != NULL && table[h]->k != k) {
    //         h = HashFunc(h + 1);
    //      }
    //      if (table[h] == NULL)
    //         return -1;
    //      else
    //         return table[h]->v;
    //   }
    //   void Remove(int k) {
    //      int h = HashFunc(k);
    //      while (table[h] != NULL) {
    //         if (table[h]->k == k)
    //            break;
    //         h = HashFunc(h + 1);
    //      }
    //      if (table[h] == NULL) {
    //         cout<<"No Element found at key "<<k<<endl;
    //         return;
    //      } else {
    //         delete table[h];
    //      }
    //      cout<<"Element Deleted"<<endl;
    //   }
};

int main() {
    // class HashTable cache = HashTable(4);
    return 0;
}