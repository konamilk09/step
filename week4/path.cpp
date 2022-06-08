/**
 * STEP2022 week4
 * 2点間の経路を result.txt に出力する。
 * DFS を使うか BFS を使うか選べる
 */ 

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <sys/time.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

std::map<std::string, std::string> read_pages
    (const std::string &filepath, 
    std::map<std::string, std::string> &pages_r) {
    
    std::map<std::string, std::string> pages;
    
    std::ifstream file(filepath);
    if(!file) {
        std::cerr << "ファイルを開けませんでした。" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string data;
    while (std::getline(file, data)) {
        auto index = data.find('\t');
        auto id = data.substr(0, index);
        auto title = data.substr(index + 1, data.size() - id.size() - 1);
        pages[id] = title;
        pages_r[title] = id;
    }

    return pages;
}

std::map<std::string, std::set<std::string>> read_links(const std::string &filepath) {
    
    std::map<std::string, std::set<std::string>> links;

    std::ifstream file(filepath);
    if(!file) {
        std::cerr << "ファイルを開けませんでした。" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string data;
    while(std::getline(file, data)) {
        auto index = data.find('\t');
        auto from = data.substr(0, index);
        auto to = data.substr(index + 1, data.size() - from.size() - 1);
        links[from].insert(to);
    }

    return links;
}

std::list<std::string> bfs
    (const std::map<std::string, std::set<std::string>> &links, 
    const std::string &start, 
    const std::string &target, 
    const std::map<std::string, std::string> &pages) 
{
    // 後でパスを辿るため、親を記録する
    std::map<std::string, std::string> parents;
    // 見たものを保持
    std::set<std::string> seen;
    // stack 次に見るものの待機列
    std::queue<std::string> container;
    // 経路を保持するリスト（返り値）
    std::list<std::string> route;

    container.push(start);
    parents[start] = "root";
    while(!container.empty()) {

        std::string v = container.front();
        container.pop();

        if(seen.count(v)) continue;
        seen.insert(v);
        if(v == target) {
            // 親を辿ってパスを返す
            while(v != "root") {
                route.push_front(pages.at(v));
                v = parents[v];
            }
            return route;
        }

        if(links.count(v)) {
            for(const std::string &link : links.at(v)) {
                if(seen.count(link)) continue;
                container.push(link);
                parents[link] = v;
            }
        }

    }

    return route;
}

std::list<std::string> dfs
    (const std::map<std::string, 
    std::set<std::string>> &links, 
    const std::string &start, 
    const std::string &target, 
    const std::map<std::string, std::string> &pages) 
{
    // 後でパスを辿るため、親を記録する
    std::map<std::string, std::string> parents;
    // 見たものを保持
    std::set<std::string> seen;
    // stack 次に見るものの待機列
    std::stack<std::string> container;
    // 返り値の経路を保持するリスト
    std::list<std::string> route;

    container.push(start);
    parents[start] = "root";

    while(!container.empty()) {

        // stack の一番上を取り出す
        std::string v = container.top();
        container.pop();

        if(seen.count(v)) continue;
        seen.insert(v);

        if(v == target) {
            // 親を辿ってパスを作成
            while(v != "root") {
                route.push_front(pages.at(v));
                v = parents[v];
            }
            return route;
        }

        if(links.count(v)) {
            for(const std::string &link : links.at(v)) {
                if(seen.count(link)) continue;
                container.push(link);
                parents[link] = v;
            }
        }

    }

    return route;
}

int main() {
    // pages[id] = title
    std::map<std::string, std::string> pages;
    // pages_r[title] = id
    std::map<std::string, std::string> pages_r;
    // links[id] = links connected with id
    std::map<std::string, std::set<std::string>> links;
    // small data を使うか
    char if_small;
    // DFS or BFS
    char dfs_or_bfs;
    // 始点と終点
    std::string start;
    std::string target;
    // 経路
    std::list<std::string> route;

    // 条件分岐用の情報受け取り
    printf("Use small data?(y/n)> ");
    std::cin >> if_small;
    
    printf("start> ");
    std::cin >> start;
    printf("goal> ");
    std::cin >> target;

    printf("DFS or BFS?(d/b)> ");
    std::cin >> dfs_or_bfs;

    printf("Searching...\n");

    double begin = get_time();

    // if_small に合わせてファイル読み込み
    if(if_small == 'y') {
        pages = read_pages("data/pages_small.txt", pages_r);
        links = read_links("data/links_small.txt");
    }
    else if(if_small == 'n') {
        pages = read_pages("data/pages.txt", pages_r);
        links = read_links("data/links.txt");
    }
    else {
        printf("Invalid Input at Use small data?: %c\n",if_small);
        return 1;
    }

    // 入力した始点と終点がなかったら
    if(!pages_r.count(start)) {
        printf("No start word: %s\n", start.c_str());
        return 1;
    }
    if(!pages_r.count(target)) {
        printf("No target word: %s\n", target.c_str());
        return 1;
    }

    // 始点と終点の番号を結果のファイルに書き込む
    std::ofstream fout("result.txt");
    fout << "start: " << start << "(" << pages_r[start] << ")" << std::endl;
    fout << "goal: " << target << "(" << pages_r[target] << ")" << std::endl;

    if(dfs_or_bfs == 'd') {
        route = dfs(links, pages_r[start], pages_r[target], pages);
    }
    else if(dfs_or_bfs == 'b') {
        route = bfs(links, pages_r[start], pages_r[target], pages);
    } 
    else {
        printf("Invalid Input at DFS or BFS?: %c\n",dfs_or_bfs);
        return 1;
    }

    double end = get_time();

    // route をファイルに書き込む
    fout << "Route start: ";
    if(route.empty()) {
        printf("No path found.\n");
    }
    else { 
        for(const std::string &page : route) {
            fout << "->" << page << std::endl;
        }    
        printf("Path has written in result.txt\n");
    }

    printf("time: %.6lf sec\n", end - begin);
  
  return 0;
}