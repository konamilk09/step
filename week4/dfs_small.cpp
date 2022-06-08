/**
 * STEP2022 week4
 * DFSで"Google"から"キットカット"までをたどる方法を探す
 * small版
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

std::list<std::string> dfs(const std::map<std::string, std::set<std::string>> &links, const std::string &start, const std::string &target, const std::map<std::string, std::string> &pages) {
    // 後でパスを辿るため、親を記録する
    std::map<std::string, std::string> parents;
    // 見たものを保持
    std::set<std::string> seen;
    // stack 次に見るものの待機列
    std::stack<std::string> container;

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

void print_list(const std::list<std::string> &route) {
    std::cout << "Route start";
    for(const std::string &page : route) {
        std::cout << "->" << page;
    }
    std::cout << std::endl;
    return;
}

int main() {
    // pages[name] = content
    std::map<std::string, std::string> pages;
    // links[name] = links
    std::map<std::string, std::set<std::string>> links;

    {
        std::ifstream file("data/pages_small.txt"); // page numbers and each contents
        std::string data;
        while (std::getline(file, data)) {
            auto index = data.find('\t');
            auto id = data.substr(0, index);
            auto title = data.substr(index + 1, data.size() - id.size() - 1);
            pages[id] = title;
        }
    }
  
    {
        std::ifstream file("data/links_small.txt");
        std::string data;
        while(std::getline(file, data)) {
            auto index = data.find('\t');
            auto from = data.substr(0, index);
            auto to = data.substr(index + 1, data.size() - from.size() - 1);
            links[from].insert(to);
        }
    }

    std::string start;
    std::string target;
  
    for(const auto& page : pages) {
        if(page.second == "Google") {
            std::cout << page.second << " " << page.first << std::endl;
            start = page.first;
        }
        if(page.second == "キットカット") {
            std::cout << page.second << " " << page.first << std::endl;
            target = page.first;
        }
    }

    std::list<std::string> route = dfs(links, start, target, pages);

    print_list(route);
  
  return 0;
}