#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <set>

// filepathのファイルから「ポケモン」という単語が含まれるページのみを読み込む
// pages[id] = title
// pages_r[title] = id
std::map<std::string, std::string> read_pages
    (const std::string &filepath, 
    std::map<std::string, std::string> &pages_r) 
{
    std::map<std::string, std::string> pages;
    
    std::ofstream fout("pokepages.txt");
    
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

        auto poke_index = title.find("ポケモン");
        if(poke_index != std::string::npos) {
            pages[id] = title;
            pages_r[title] = id;
        }
    }

    fout.close();

    return pages;
}

// pages に含まれるものの中のリンクのみ読み込む
std::map<std::string, std::set<std::string>> read_links
    (const std::string &filepath, 
    const std::map<std::string, std::string> &pages) 
{
    std::map<std::string, std::set<std::string>> links;
    std::ofstream fout("pokelinks.txt");

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
        if(pages.count(from) && pages.count(to)) {
            links[from].insert(to);
        }
    }
    fout.close();
    
    return links;
}

// pages を isolated に std::move し、isolated のうちリンクで繋がっているものを削除したものを返す
std::map<std::string, std::string> find_isolate
    (std::map<std::string, std::string> &pages, 
    const std::map<std::string, std::set<std::string>> &links) 
{
    std::map<std::string, std::string> isolated = std::move(pages);
    
    for(const auto &link : links) {
        for(const std::string &to : link.second) {
            if(isolated.count(to)) {
                isolated.erase(to);
            }
        }    
        if(isolated.count(link.first)) {
            isolated.erase(link.first);
        }
    }

    return isolated;
}

int main() {
    std::map<std::string, std::string> pages;
    std::map<std::string, std::string> pages_r;
    std::map<std::string, std::set<std::string>> links;
    pages = read_pages("data/pages.txt", pages_r);
    links = read_links("data/links.txt", pages);

    std::map<std::string, std::string> isolated;
    isolated = find_isolate(pages, links);

    std::ofstream fout("pokeiso.txt");
    for(const auto &iso : isolated) {
        fout << iso.second << "(" << iso.first << ")" << std::endl;
    }
    fout.close();

    return 0;
}