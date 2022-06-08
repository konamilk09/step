/**
 * STEP2022 week4
 * BFSで"Google"から"キットカット"までをたどる方法を探す
 * small版
 */ 

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
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
    file.close();
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
    file.close();
    return links;

}

std::list<std::string> bfs(const std::map<std::string, std::set<std::string>> &links, const std::string &start, const std::string &target, const std::map<std::string, std::string> &pages) {
    // 後でパスを辿るため、親を記録する
    std::map<std::string, std::string> parents;
    // 見たものを保持
    std::set<std::string> seen;
    // stack 次に見るものの待機列
    std::queue<std::string> container;
    // 経路を保持するリスト（帰り値）
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

int main() {
    // pages[name] = content
    std::map<std::string, std::string> pages;
    // links[name] = links connect with name
    std::map<std::string, std::set<std::string>> links;

    pages = read_pages("data/pages_small.txt");
    links = read_links("data/links_small.txt");

    std::string start;
    std::string target;
  
    // スタートとターゲットの番号を見つける
    std::ofstream fout("bfs.txt");
    for(const auto& page : pages) {
        if(page.second == "Google") {
            fout << page.second << " " << page.first << std::endl;
            start = page.first;
        }
        if(page.second == "キットカット") {
            fout << page.second << " " << page.first << std::endl;
            target = page.first;
        }
    }

    std::list<std::string> route = bfs(links, start, target, pages);

    // route をファイルに書き込む
    fout << "Route start: \n";
    for(const std::string &page : route) {
        fout << "->" << page;
    }
    fout << std::endl;
    fout.close();
  
  return 0;
}