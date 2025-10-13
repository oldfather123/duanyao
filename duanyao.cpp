#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
using namespace std;

vector<string> parse_tiles(const string& input) {
    vector<string> tiles;
    for (size_t i = 0; i < input.size(); i += 2) {
        tiles.push_back(input.substr(i, 2));
    }
    return tiles;
}

bool cmptile(const string& a, const string& b) {
    if (a[1] != b[1]) return a[1] < b[1];
    return a[0] < b[0];
}

vector<string> sort_tiles(vector<string> tiles) {
    sort(tiles.begin(), tiles.end(), cmptile);
    return tiles;
}

struct Tiles {
    vector<string> kezi;
    vector<string> shunzi;
    vector<string> gangzi;
    vector<string> duizi;
    vector<string> dazi;
    vector<string> danpai;
};

vector<Tiles> split_tiles(const vector<string>& tiles) {
    // 统计每种牌的数量
    unordered_map<string,int> counts;
    for (const auto &t : tiles) counts[t]++;

    // 枚举用到的牌位顺序（按花色/数字排序）
    vector<string> keys;
    for (const auto &kv : counts) keys.push_back(kv.first);
    sort(keys.begin(), keys.end(), cmptile);

    vector<Tiles> results;
    Tiles cur;

    // 找到第一个还有牌的 key，返回 "" 表示全部用完
    auto first_nonzero = [&]() -> string {
        for (const auto &k : keys) if (counts[k] > 0) return k;
        return string();
    };

    // 生成 r+1, r+2 的辅助（当 tile 为 '9?' 时会返回空串）
    auto next1 = [](const string &tile)->string {
        if (tile.empty() || tile[0] == '9') return string();
        return string{ char(tile[0] + 1), tile[1] };
    };
    auto next2 = [&](const string &tile)->string {
        string t1 = next1(tile);
        if (t1.empty() || t1[0] == '9') return string();
        return string{ char(t1[0] + 1), t1[1] };
    };

    function<void()> dfs = [&]() {
        string k = first_nonzero();
        if (k.empty()) {
            // 所有牌都用完，记录一个分割（拷贝当前的 cur）
            results.push_back(cur);
            return;
        }

        int c = counts[k];
        // 尝试杠（kan）优先
        if (c >= 4) {
            counts[k] -= 4;
            cur.gangzi.push_back(k + k + k + k);
            dfs();
            cur.gangzi.pop_back();
            counts[k] += 4;
        }

        // 尝试刻子（pon）
        if (c >= 3) {
            counts[k] -= 3;
            cur.kezi.push_back(k + k + k);
            dfs();
            cur.kezi.pop_back();
            counts[k] += 3;
        }

        // 尝试顺子（仅 m/p/s）
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

        // 尝试搭子（两张连续的单牌），定义为 dazi：k 与 k+1 各减1
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
            // k 与 k+2（例如 1s 和 3s 也算搭子）
            if (!k2.empty() && counts[k] > 0 && counts[k2] > 0) {
                counts[k]--; counts[k2]--;
                cur.dazi.push_back(k + k2);
                dfs();
                cur.dazi.pop_back();
                counts[k]++; counts[k2]++;
            }
        }

        // 尝试对子（pair）
        if (c >= 2 && cur.duizi.size() < 2) {
            counts[k] -= 2;
            cur.duizi.push_back(k + k);
            dfs();
            cur.duizi.pop_back();
            counts[k] += 2;
        }

        // 尝试把它作为单张（danpai）
        if (c >= 1 && cur.danpai.size() < 1) {
            counts[k] -= 1;
            cur.danpai.push_back(k);
            dfs();
            cur.danpai.pop_back();
            counts[k] += 1;
        }
    };

    dfs();

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

    return results;
}

int main() {
    string input;
    cin >> input;
    if (input.length() < 26) {
        cout << "Error: tiles length fail, current length " << input.length() / 2 << endl;
        return 0;
    }
    vector<string> tiles = parse_tiles(input);
    tiles = sort_tiles(tiles);
    for (int i = 0; i < tiles.size(); i++) {
        cout << tiles[i];
    }
    cout << endl;
    vector<Tiles> results = split_tiles(tiles);
    cout << "Total combinations: " << results.size() << endl;
    if (results.empty()) {
        cout << "无法识别的牌型" << endl;
        return 0;
    }
    for (int i = 0; i < results.size(); i++) {
        const Tiles &t = results[i];
        cout << "牌型 " << (i + 1) << ":" << endl;
        if (!t.gangzi.empty()) {
            cout << "  杠子: ";
            for (const auto &g : t.gangzi) cout << g << " ";
            cout << endl;
        }
        if (!t.kezi.empty()) {
            cout << "  刻子: ";
            for (const auto &k : t.kezi) cout << k << " ";
            cout << endl;
        }
        if (!t.shunzi.empty()) {
            cout << "  顺子: ";
            for (const auto &s : t.shunzi) cout << s << " ";
            cout << endl;
        }
        if (!t.duizi.empty()) {
            cout << "  对子: ";
            for (const auto &d : t.duizi) cout << d << " ";
            cout << endl;
        }
        if (!t.dazi.empty()) {
            cout << "  搭子: ";
            for (const auto &d : t.dazi) cout << d << " ";
            cout << endl;
        }
        if (!t.danpai.empty()) {
            cout << "  单张: ";
            for (const auto &d : t.danpai) cout << d << " ";
            cout << endl;
        }
    }
    return 0;
}