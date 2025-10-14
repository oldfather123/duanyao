#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
#include"defs.hpp"
using namespace std;

bool erbeikou(const Tiles &tile) {
    if (!(tile.shunzi.size() == 3 && tile.duizi.size() == 1 && tile.dazi.size() == 1))
        return false;
    vector<string> t = tile.shunzi;
    string s = addhupai(tile.dazi[0]);
    t.push_back(s);
    int cnt = 0;
    for (const auto &x :t) {
        if (count(t.begin(), t.end(), x) >= 2)
            cnt++;
    }
    if (cnt == 4)
        return true;
    return false;
}

bool chunquan(const Tiles &tile) {
    for (const auto &k : tile.kezi) {
        if (!is_yaojiu(k.substr(0, 2)) || k[1] == 'z') 
            return false;
    }
    for (const auto &g : tile.gangzi) {
        if (!is_yaojiu(g.substr(0, 2)) || g[1] == 'z') 
            return false;
    }
    for (const auto &d : tile.duizi) {
        if (!is_yaojiu(d.substr(0, 2)) || d[1] == 'z') 
            return false;
    }
    for (const auto &d : tile.danpai) {
        if (!is_yaojiu(d.substr(0, 2)) || d[1] == 'z') 
            return false;
    }
    for (const auto &s : tile.shunzi) {
        if (!is_yaojiu(s.substr(0, 2)) && !is_yaojiu(s.substr(4, 2)))
            return false;
    }
    for (const auto &d : tile.dazi) {
        string s = addhupai(d);
        if (!is_yaojiu(s.substr(0, 2)) && !is_yaojiu(s.substr(4, 2)))
            return false;
    }
    return true;
}

bool hunyise(const Tiles &tile) {
    vector<char> types;
    for (const auto &k : tile.kezi) {
        types.push_back(k[1]);
    }
    for (const auto &g : tile.gangzi) {
        types.push_back(g[1]);
    }
    for (const auto &d : tile.duizi) {
        types.push_back(d[1]);
    }
    for (const auto &d : tile.danpai) {
        types.push_back(d[1]);
    }
    for (const auto &s : tile.shunzi) {
        types.push_back(s[1]);
    }
    for (const auto &d : tile.dazi) {
        types.push_back(d[1]);
    }
    types.push_back(hupai[1]);
    sort(types.begin(), types.end());
    types.erase(unique(types.begin(), types.end()), types.end());
    if (types.size() != 2)
        return false;
    if (types[0] == 'z' || types[1] == 'z')
        return true;
    return false;
}

bool qingyise(const Tiles &tile) {
    char type = hupai[1];
    for (const auto &t : tile.kezi) {
        if (t[1] != type) 
            return false;
    }
    for (const auto &t : tile.shunzi) {
        if (t[1] != type) 
            return false;
    }
    for (const auto &t : tile.gangzi) {
        if (t[1] != type)
            return false;
    }
    for (const auto &t : tile.duizi) {
        if (t[1] != type)
            return false;
    }
    for (const auto &t : tile.danpai) {
        if (t[1] != type)
            return false;
    }
    return true;
}