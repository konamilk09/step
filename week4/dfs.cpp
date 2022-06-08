/**
 * STEP2022 week4
 * DFSで"Google"から"渋谷"までをたどる方法を探す
 */ 

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <list>

std::map<std::string, std::string> read_pages(const std::string &filepath) {
    
    std::map<std::string, std::string> pages;
    
    std::ifstream file(filepath); // page numbers and each contents
    std::string data;
    while (std::getline(file, data)) {
        auto index = data.find('\t');
        auto id = data.substr(0, index);
        auto title = data.substr(index + 1, data.size() - id.size() - 1);
        pages[id] = title;
    }

    return pages;

}

std::map<std::string, std::set<std::string>> read_links(const std::string &filepath) {
    
    std::map<std::string, std::set<std::string>> links;

    std::ifstream file(filepath);
    std::string data;
    while(std::getline(file, data)) {
        auto index = data.find('\t');
        auto from = data.substr(0, index);
        auto to = data.substr(index + 1, data.size() - from.size() - 1);
        links[from].insert(to);
    }

    return links;

}

std::list<std::string> dfs(const std::map<std::string, std::set<std::string>> &links, const std::string &start, const std::string &target, const std::map<std::string, std::string> &pages) {
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

        std::string v = container.top();
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

int main() {
    // pages[name] = content
    std::map<std::string, std::string> pages;
    // links[name] = links connect with name
    std::map<std::string, std::set<std::string>> links;

    pages = read_pages("data/pages.txt");
    links = read_links("data/links.txt");

    std::string start;
    std::string target;
  
    // スタートとターゲットの番号を見つける
    for(const auto& page : pages) {
        if(page.second == "Google") {
            std::cout << page.second << " " << page.first << std::endl;
            start = page.first;
        }
        if(page.second == "渋谷") {
            std::cout << page.second << " " << page.first << std::endl;
            target = page.first;
        }
    }

    std::list<std::string> route = dfs(links, start, target, pages);

    // route をファイルに書き込む
    std::ofstream fout("dfs.txt");
    fout << "Route start: \n";
    if(route.empty()) {
        std::cout << "No path found." << std::endl;
    }
    else { 
        for(const std::string &page : route) {
            fout << "->" << page;
        }    
    }
    fout << std::endl;
  
  return 0;
}