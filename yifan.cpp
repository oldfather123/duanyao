#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
#include"defs.hpp"
using namespace std;

bool is_yaojiu(const string &t) {
    if (t.size() != 2) 
        return false;
    if (t[1] == 'z') 
        return true;
    return t[0] == '1' || t[0] == '9';
}

bool duanyaojiu(const Tiles& tile) {
    for (const auto &k : tile.kezi) {
        if (is_yaojiu(k.substr(0, 2))) 
            return false;
    }
    for (const auto &s : tile.shunzi) {
        if (is_yaojiu(s.substr(0, 2)) || is_yaojiu(s.substr(4, 2)))
            return false;
    }
    for (const auto &g : tile.gangzi) {
        if (is_yaojiu(g.substr(0, 2))) 
            return false;
    }
    for (const auto &d : tile.duizi) {
        if (is_yaojiu(d.substr(0, 2))) 
            return false;
    }
    for (const auto &d : tile.dazi) {
        if (is_yaojiu(d.substr(0, 2)) || is_yaojiu(d.substr(2, 2)))
            return false;
    }
    for (const auto &d : tile.danpai) {
        if (is_yaojiu(d))
            return false;
    }
    if (is_yaojiu(hupai))
        return false;
    return true;
}

bool pinghu(const Tiles& tile) {
    if (tile.dazi.size() != 1)
        return false;
    auto is_yipai = [](const string &t) -> bool {
        if (t.size() != 2) return false;
        return t == zifeng || t == changfeng || t == "5z" || t == "6z" || t == "7z";
    };
    if (tile.shunzi.size() == 3 && tile.duizi.size() == 1 && (tile.dazi[0][2] - tile.dazi[0][0] == 1) && !is_yipai(tile.duizi[0].substr(0, 2)))
        return true;
    return false;
}

bool zifengpai(const Tiles& tile) {
    for (const auto &g : tile.gangzi) {
        if (g.substr(0, 2) == zifeng)
            return true;
    }
    for (const auto &k : tile.kezi) {
        if (k.substr(0, 2) == zifeng)
            return true;
    }
    for (const auto &d : tile.duizi) {
        if (d.substr(0, 2) == zifeng && hupai == zifeng)
            return true;
    }
    return false;
}

bool changfengpai(const Tiles& tile) {
    for (const auto &g : tile.gangzi) {
        if (g.substr(0, 2) == changfeng)
            return true;
    }
    for (const auto &k : tile.kezi) {
        if (k.substr(0, 2) == changfeng)
            return true;
    }
    for (const auto &d : tile.duizi) {
        if (d.substr(0, 2) == changfeng && hupai == changfeng)
            return true;
    }
    return false;
}

bool bai(const Tiles& tile) {
    for (const auto &g : tile.gangzi) {
        if (g.substr(0, 2) == "5z")
            return true;
    }
    for (const auto &k : tile.kezi) {
        if (k.substr(0, 2) == "5z")
            return true;
    }
    for (const auto &d : tile.duizi) {
        if (d.substr(0, 2) == "5z" && hupai == "5z")
            return true;
    }
    return false;
}

bool fa(const Tiles& tile) {
    for (const auto &g : tile.gangzi) {
        if (g.substr(0, 2) == "6z")
            return true;
    }
    for (const auto &k : tile.kezi) {
        if (k.substr(0, 2) == "6z")
            return true;
    }
    for (const auto &d : tile.duizi) {
        if (d.substr(0, 2) == "6z" && hupai == "6z")
            return true;
    }
    return false;
}

bool zhong(const Tiles& tile) {
    for (const auto &g : tile.gangzi) {
        if (g.substr(0, 2) == "7z")
            return true;
    }
    for (const auto &k : tile.kezi) {
        if (k.substr(0, 2) == "7z")
            return true;
    }
    for (const auto &d : tile.duizi) {
        if (d.substr(0, 2) == "7z" && hupai == "7z")
            return true;
    }
    return false;
}

bool yibeikou(const Tiles& tile) {
    if (tile.shunzi.size() < 2)
        return false;
    for (int i = 0; i < tile.shunzi.size(); i++) {
        for (int j = i + 1; j < tile.shunzi.size(); j++) {
            if (tile.shunzi[j] == tile.shunzi[i])
                return true;
        }
        if (!tile.dazi.empty()) {
            if ((tile.dazi[0] == tile.shunzi[i].substr(0, 4) && hupai == tile.shunzi[i].substr(4, 2)) ||
                (tile.dazi[0] == tile.shunzi[i].substr(2, 4) && hupai == tile.shunzi[i].substr(0, 2)) ||
                (tile.dazi[0] == tile.shunzi[i].substr(0, 2) + tile.shunzi[i].substr(4, 2) && hupai == tile.shunzi[i].substr(2, 2)))
                return true;
        }
    }
    return false;
}