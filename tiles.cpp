#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
#include"defs.hpp"

// 字符串转换为数组
vector<string> parse_tiles(const string& input) {
    vector<string> tiles;
    for (size_t i = 0; i < input.size(); i += 2) {
        tiles.push_back(input.substr(i, 2));
    }
    return tiles;
}

// 牌面排序函数（雀魂应该就这么排）
bool cmptile(const string& a, const string& b) {
    if (a[1] != b[1]) 
        return a[1] < b[1];
    return a[0] < b[0];
}

vector<string> sort_tiles(vector<string> tiles) {
    sort(tiles.begin(), tiles.end(), cmptile);
    return tiles;
}

// 下一张
string next1(const string &tile) {
    if (tile.empty() || tile[0] == '9') 
        return string();
    return string{ char(tile[0] + 1), tile[1] };
};
// 下两张
string next2(const string &tile) {
    string t1 = next1(tile);
    if (t1.empty() || t1[0] == '9') 
        return string();
    return string{ char(t1[0] + 1), t1[1] };
};
// 前一张
string prev1(const string &tile) {
    if (tile.empty() || tile[0] == '1') 
        return string();
    return string{ char(tile[0] - 1), tile[1] };
};

void get_daixuan(Tiles& tile) {
    // 两面&边张
    if (!tile.dazi.empty() && (tile.dazi[0][2] - tile.dazi[0][0] == 1)) {
        string prev = prev1(tile.dazi[0].substr(0,2));
        string next = next1(tile.dazi[0].substr(2,2));
        if (!prev.empty())    
            tile.daixuan.push_back(prev);
        if (!next.empty())
            tile.daixuan.push_back(next);
    }
    // 坎张
    if (!tile.dazi.empty() && (tile.dazi[0][2] - tile.dazi[0][0] == 2)) {
        string next = next1(tile.dazi[0].substr(0,2));
        if (!next.empty())
            tile.daixuan.push_back(next);
    }
    // 双碰
    if (tile.duizi.size() == 2) {
        tile.daixuan.push_back(tile.duizi[0].substr(0,2));
        tile.daixuan.push_back(tile.duizi[1].substr(0,2));
    }
    // 单吊
    if (tile.danpai.size() == 1) {
        tile.daixuan.push_back(tile.danpai[0]);
    }
    // 国士十三面
    if (tile.danpai.size() == 13) {
        for (const auto &d : tile.danpai) {
            tile.daixuan.push_back(d);
        }
    }
    // 国士普通
    if (tile.duizi.size() == 1 && tile.danpai.size() == 11) {
        vector<string> yaojiu = {
            "1m","9m","1p","9p","1s","9s",
            "1z","2z","3z","4z","5z","6z","7z"
        };
        vector<string> diff;
        set_difference(yaojiu.begin(), yaojiu.end(), tile.danpai.begin(), tile.danpai.end(), back_inserter(diff));
        for (const auto &d : diff) {
            if (d != tile.duizi[0].substr(0,2))
                tile.daixuan.push_back(d);
        }
    }
}

// 分割牌型
vector<Tiles> split_tiles(const vector<string>& tiles) {
    unordered_map<string,int> counts;
    for (const auto &t : tiles) 
        counts[t]++;

    vector<string> keys;
    for (const auto &kv : counts) 
        keys.push_back(kv.first);
    sort(keys.begin(), keys.end(), cmptile);

    vector<Tiles> results;
    Tiles cur;

    // 找到第一个还有余量的牌
    auto first_nonzero = [&]() -> string {
        for (const auto &k : keys) 
            if (counts[k] > 0) 
                return k;
        return string();
    };

    // 回溯dfs
    function<void()> dfs = [&]() {
        string k = first_nonzero();
        if (k.empty()) {
            // 所有牌都用完，记录一个牌型
            results.push_back(cur);
            return;
        }

        int c = counts[k];
        // 尝试杠子
        if (c >= 4) {
            counts[k] -= 4;
            cur.gangzi.push_back(k + k + k + k);
            dfs();
            cur.gangzi.pop_back();
            counts[k] += 4;
        }

        // 尝试刻子
        if (c >= 3) {
            counts[k] -= 3;
            cur.kezi.push_back(k + k + k);
            dfs();
            cur.kezi.pop_back();
            counts[k] += 3;
        }

        // 尝试顺子
        if (k.size() == 2 && (k[1] == 'm' || k[1] == 'p' || k[1] == 's')) {
            string k1 = next1(k), k2 = next2(k);
            if (!k1.empty() && !k2.empty() && counts[k1] > 0 && counts[k2] > 0) {
                counts[k]--; counts[k1]--; counts[k2]--;
                cur.shunzi.push_back(k + k1 + k2);
                dfs();
                cur.shunzi.pop_back();
                counts[k]++; counts[k1]++; counts[k2]++;
            }
        }

        // 尝试搭子
        if (k.size() == 2 && (k[1] == 'm' || k[1] == 'p' || k[1] == 's') && cur.dazi.size() < 1) {
            string k1 = next1(k);
            string k2 = next2(k);
            // k 与 k+1
            if (!k1.empty() && counts[k] > 0 && counts[k1] > 0) {
                counts[k]--; counts[k1]--;
                cur.dazi.push_back(k + k1);
                dfs();
                cur.dazi.pop_back();
                counts[k]++; counts[k1]++;
            }
            // k 与 k+2
            if (!k2.empty() && counts[k] > 0 && counts[k2] > 0) {
                counts[k]--; counts[k2]--;
                cur.dazi.push_back(k + k2);
                dfs();
                cur.dazi.pop_back();
                counts[k]++; counts[k2]++;
            }
        }

        // 尝试对子，最多允许两个对子
        if (c >= 2 && cur.duizi.size() < 2) {
            counts[k] -= 2;
            cur.duizi.push_back(k + k);
            dfs();
            cur.duizi.pop_back();
            counts[k] += 2;
        }

        // 尝试单张，最多允许一个单张
        if (c >= 1 && cur.danpai.size() < 1) {
            counts[k] -= 1;
            cur.danpai.push_back(k);
            dfs();
            cur.danpai.pop_back();
            counts[k] += 1;
        }
    };

    dfs();
    
    // 去除错误分割的牌型
    vector<Tiles> filtered;
    filtered.reserve(results.size());
    for (const auto &t : results) {
        if (t.duizi.size() == 2 && (!t.dazi.empty() || !t.danpai.empty())) {
            continue;
        }
        if (!t.dazi.empty() && !t.danpai.empty()) {
            continue;
        }
        filtered.push_back(t);
    }
    results.swap(filtered);

    // 七对子
    if (results.empty()) {
        Tiles tmp;
        int qidui = 0;
        for (const auto &k : keys) {
            if (counts[k] == 2) {
                qidui++;
                tmp.duizi.push_back(k + k);
            }
            else if (counts[k] == 1)
                tmp.danpai.push_back(k);
        }
        if (qidui == 6) 
            results.push_back(tmp);
    }

    // 国士无双
    if (results.empty()) {
        Tiles tmp;
        for (const auto &k : keys) {
            if (((k[0] == '1' || k[0] == '9') && (k[1] == 'm' || k[1] == 'p' || k[1] == 's')) || k[1] == 'z') {
                if (counts[k] == 1) 
                    tmp.danpai.push_back(k);
                else if (counts[k] == 2 && cur.duizi.size() < 1)
                    tmp.duizi.push_back(k + k);
            }
        }
        results.push_back(tmp);
    }

    // 去除不符合胡牌的牌型
    for (auto &t : results) {
        get_daixuan(t);
    }
    filtered.clear();
    filtered.reserve(results.size());
    for (const auto &t : results) {
        bool found = false;
        for (const auto &d : t.daixuan) {
            if (d == hupai) {
                found = true;
                break;
            }
        }
        if (found) 
            filtered.push_back(t);
    }
    results.swap(filtered);

    return results;
}