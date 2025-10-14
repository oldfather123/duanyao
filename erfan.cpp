#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
#include"defs.hpp"
using namespace std;

bool sansetongke(const Tiles &tile) {
    if (tile.shunzi.size() >= 2)
        return false;
    vector<int> all;
    for (const auto &k : tile.kezi) {
        if (k[1] != 'z')
            all.push_back(k[0] - '0');
    }
    for (const auto &g : tile.gangzi) {
        if (g[1] != 'z')
            all.push_back(g[0] - '0');
    }
    if (!tile.duizi.empty()) {
        for (const auto &d : tile.duizi) {
            if (hupai == d.substr(0, 2) && d[1] != 'z') 
                all.push_back(d[0] - '0');
        }
    }
    for (const auto &a : all) {
        if (count(all.begin(), all.end(), a) >= 3)
            return true;
    }
    return false;
}

bool sangangzi(const Tiles &tile) {
    if (tile.gangzi.size() == 3)
        return true;
    return false;
}

bool duiduihu(const Tiles &tile) {
    if (tile.shunzi.size() > 0)
        return false;
    if (tile.kezi.size() + tile.gangzi.size() == 3 && tile.duizi.size() == 2)
        return true;
    if (tile.kezi.size() + tile.gangzi.size() == 4 && tile.danpai.size() == 1)
        return true;
    return false;
}

bool sananke(const Tiles &tile, const FTiles &ftile) {
    if (tile.shunzi.size() >= 2)
        return false;
    if (tile.kezi.size() + tile.gangzi.size() == 2 && tile.duizi.size() == 2) {
        for (const auto &k : tile.kezi) {
            if (count(ftile.kezi.begin(), ftile.kezi.end(), k) > 0)
                return false;
        }
        for (const auto &g : tile.gangzi) {
            if (count(ftile.gangzi.begin(), ftile.gangzi.end(), g) > 0)
                return false;
        }
        if (zimo)
            return true;
    }
    if (tile.kezi.size() + tile.gangzi.size() == 3) {
        for (const auto &k : tile.kezi) {
            if (count(ftile.kezi.begin(), ftile.kezi.end(), k) > 0)
                return false;
        }
        for (const auto &g : tile.gangzi) {
            if (count(ftile.gangzi.begin(), ftile.gangzi.end(), g) > 0)
                return false;
        }
        return true;
    }
    if (tile.kezi.size() + tile.gangzi.size() == 4) {
        int cnt = 0;
        for (const auto &k : tile.kezi) {
            if (count(ftile.kezi.begin(), ftile.kezi.end(), k) > 0)
                cnt++;
        }
        for (const auto &g : tile.gangzi) {
            if (count(ftile.gangzi.begin(), ftile.gangzi.end(), g) > 0)
                cnt++;
        }
        if (cnt == 1)
            return true;
    }
    return false;
}

bool xiaosanyuan(const Tiles &tile) {
    int kcnt = 0, dcnt = 0;
    for (const auto &g : tile.gangzi) {
        if (g[0] >= '5' && g[1] == 'z')
            kcnt++;
    }
    for (const auto &k : tile.kezi) {
        if (k[0] >= '5' && k[1] == 'z')
            kcnt++;
    }
    for (const auto &d : tile.duizi) {
        if (d[0] >= '5' && d[1] == 'z') {
            if (hupai == d.substr(0, 2))
                kcnt++;
            else
                dcnt++;
        }
    }
    for (const auto &d : tile.danpai) {
        if (d[0] >= '5' && d[1] == 'z') {
            if (hupai == d.substr(0, 2))
                dcnt++;
        }
    }
    if (kcnt == 2 && dcnt == 1)
        return true;
    return false;
}

bool hunlaotou(const Tiles &tile) {
    if (tile.shunzi.size() > 0 || tile.dazi.size() > 0)
        return false;
    int mpscnt = 0, zcnt = 0;
    for (const auto &k : tile.kezi) {
        if (!is_yaojiu(k.substr(0, 2))) 
            return false;
        if (k[1] == 'z')
            zcnt++;
        else
            mpscnt++;
    }
    for (const auto &g : tile.gangzi) {
        if (!is_yaojiu(g.substr(0, 2))) 
            return false;
        if (g[1] == 'z')
            zcnt++;
        else
            mpscnt++;
    }
    for (const auto &d : tile.duizi) {
        if (!is_yaojiu(d.substr(0, 2))) 
            return false;
        if (d[1] == 'z')
            zcnt++;
        else
            mpscnt++;
    }
    for (const auto &d : tile.danpai) {
        if (!is_yaojiu(d))
            return false;
        if (d[1] == 'z')
            zcnt++;
        else
            mpscnt++;
    }
    if (!is_yaojiu(hupai))
        return false;
    if (mpscnt == 0 || zcnt == 0)
        return false;
    return true;
}

bool qiduizi(const Tiles &tile) {
    if (tile.duizi.size() == 6)
        return true;
    return false;
}

bool hunquan(const Tiles &tile) {
    int mpscnt = 0, zcnt = 0;
    for (const auto &k : tile.kezi) {
        if (!is_yaojiu(k.substr(0, 2))) 
            return false;
        if (k[1] == 'z')
            zcnt++;
        else
            mpscnt++;
    }
    for (const auto &g : tile.gangzi) {
        if (!is_yaojiu(g.substr(0, 2))) 
            return false;
        if (g[1] == 'z')
            zcnt++;
        else
            mpscnt++;
    }
    for (const auto &d : tile.duizi) {
        if (!is_yaojiu(d.substr(0, 2))) 
            return false;
        if (d[1] == 'z')
            zcnt++;
        else
            mpscnt++;
    }
    for (const auto &d : tile.danpai) {
        if (!is_yaojiu(d))
            return false;
        if (d[1] == 'z')
            zcnt++;
        else
            mpscnt++;
    }
    for (const auto &s : tile.shunzi) {
        if (!is_yaojiu(s.substr(0, 2)) && !is_yaojiu(s.substr(4, 2)))
            return false;
        mpscnt++;
    }
    for (const auto &d : tile.dazi) {
        if (!is_yaojiu(d.substr(0, 2)) && !is_yaojiu(d.substr(2, 2)))
            return false;
        mpscnt++;
    }
    if (mpscnt == 0 || zcnt == 0)
        return false;
    return true;
}

bool tongse(vector<string> vec) {
    char type = vec[0][1];
    for (const auto &v : vec) {
        if (v[1] != type)
            return false;
    }
    return true;
}

bool onetonine(vector<string> vec) {
    int cnt = 0;
    for (const auto &v : vec) {
        if (v == "123" || v == "456" || v == "789")
            cnt++;
    }
    if (cnt >= 3)
        return true;
    return false;
}

string addhupai(const string &s) {
    string result;
    if (s[2] - s[0] == 1) {
        if (hupai == prev1(s.substr(0, 2)))
            result = hupai + s;
        if (hupai == next1(s.substr(2, 2)))
            result = s + hupai;
    }
    if (s[2] - s[0] == 2) {
        if (hupai == next1(s.substr(0, 2)))
            result = s.substr(0, 2) + hupai + s.substr(2, 2);
    }
    return result;
}

bool yiqi(const Tiles &tile) {
    if (tile.shunzi.size() < 2) 
        return false;
    if (tile.duizi.size() == 1 && tile.dazi.size() == 1) {
        char type = tile.dazi[0][1];
        int cnt = 0;
        vector<string> ts;
        for (const auto &s : tile.shunzi) {
            if (s[1] == type) {
                cnt++;
                ts.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
            }
        }
        if (cnt == 1)
            return false;
        if (cnt == 0) {
            if (!tongse(tile.shunzi) || tile.shunzi.size() <= 2)
                return false;
            if (tile.shunzi[0][0] == '1' && tile.shunzi[1][0] == '4' && tile.shunzi[2][0] == '7')
                return true;
        }
        if (cnt >= 2) {
            string s = addhupai(tile.dazi[0]);
            ts.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
            if (onetonine(ts))
                return true;
        }
    }
    if (tile.duizi.size() == 2) {
        if (tile.shunzi.size() < 3 || !tongse(tile.shunzi))
            return false;
        vector<string> ts;
        for (const auto &s : tile.shunzi) {
            ts.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
        }
        if (onetonine(ts))
            return true;
    }
    if (tile.danpai.size() == 1) {
        if (tile.shunzi.size() < 3)
            return false;
        vector<string> mt, pt, st;
        for (const auto &s : tile.shunzi) {
            if (s[1] == 'm')
                mt.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
            else if (s[1] == 'p')
                pt.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
            else if (s[1] == 's')
                st.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
        }
        if (mt.size() != 3 && pt.size() != 3 && st.size() != 3)
            return false;
        if (mt.size() == 3)
            return onetonine(mt);
        if (pt.size() == 3)
            return onetonine(pt);
        if (st.size() == 3)
            return onetonine(st);
    }
    return false;
}

bool sansetongshun(const Tiles &tile) {
    if (tile.shunzi.size() < 2) 
        return false;
    vector<string> mt, pt, st;
    for (const auto &s : tile.shunzi) {
        if (s[1] == 'm')
            mt.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
        else if (s[1] == 'p')
            pt.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
        else if (s[1] == 's')
            st.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
    }
    for (const auto &d : tile.dazi) {
        string s = addhupai(d);
        if (d[1] == 'm') 
            mt.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
        else if (d[1] == 'p')
            pt.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
        else if (d[1] == 's')
            st.push_back(s.substr(0, 1) + s.substr(2, 1) + s.substr(4, 1));
    }
    if (mt.size() == 0 || pt.size() == 0 || st.size() == 0)
        return false;
    for (const auto &m : mt) {
        if (count(pt.begin(), pt.end(), m) > 0 && count(st.begin(), st.end(), m) > 0)
            return true;
    }
    for (const auto &p : pt) {
        if (count(mt.begin(), mt.end(), p) > 0 && count(st.begin(), st.end(), p) > 0)
            return true;
    }
    for (const auto &s : st) {
        if (count(mt.begin(), mt.end(), s) > 0 && count(pt.begin(), pt.end(), s) > 0)
            return true;
    }
    return false;
}