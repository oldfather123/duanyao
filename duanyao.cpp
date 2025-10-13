#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
#include"defs.hpp"
using namespace std;

string hupai;
string zifeng;
string changfeng;
vector<string> dora;
vector<string> ldora;
bool fulu;
bool lizhi;
bool wlizhi;
bool yifa;
bool zimo;
bool qianggang;
bool lingshang;
bool haidi;
bool hedi;

int main() {
    string input;
    cout << "输入手牌" << endl;
    cin >> input;
    if (input.length() < 26) {
        cout << "Error: tiles length fail, current length " << input.length() / 2 << endl;
        return 0;
    }
    cout << "输入和牌" << endl;
    cin >> hupai;
    cout << "输入场风" << endl;
    cin >> changfeng;
    cout << "输入自风" << endl;
    cin >> zifeng;
    cout << "两立直|立直|一发|自摸|副露|枪杠|岭上|海底|河底" << endl;
    string flags;
    cin >> flags;
    for (int i = 0; i < flags.length(); i++) {
        if (flags[i] == '1') {
            if (i == 0) wlizhi = true;
            else if (i == 1) lizhi = true;
            else if (i == 2) yifa = true;
            else if (i == 3) zimo = true;
            else if (i == 4) fulu = true;
            else if (i == 5) qianggang = true;
            else if (i == 6) lingshang = true;
            else if (i == 7) haidi = true;
            else if (i == 8) hedi = true;
        }
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
    count_tiles(results);
    for (int i = 0; i < results.size(); i++) {
        const Tiles &t = results[i];
        cout << "牌型 " << (i + 1) << ":" << endl;
        if (!t.gangzi.empty()) {
            cout << "  杠子: ";
            for (const auto &g : t.gangzi) 
                cout << g << " ";
            cout << endl;
        }
        if (!t.kezi.empty()) {
            cout << "  刻子: ";
            for (const auto &k : t.kezi) 
                cout << k << " ";
            cout << endl;
        }
        if (!t.shunzi.empty()) {
            cout << "  顺子: ";
            for (const auto &s : t.shunzi) 
                cout << s << " ";
            cout << endl;
        }
        if (!t.duizi.empty()) {
            cout << "  对子: ";
            for (const auto &d : t.duizi) 
                cout << d << " ";
            cout << endl;
        }
        if (!t.dazi.empty()) {
            cout << "  搭子: ";
            for (const auto &d : t.dazi) 
                cout << d << " ";
            cout << endl;
        }
        if (!t.danpai.empty()) {
            cout << "  单张: ";
            for (const auto &d : t.danpai) 
                cout << d << " ";
            cout << endl;
        }
        if (!t.yizhong.empty()) {
            cout << "  役种: " << endl;
            for (const auto &d : t.yizhong) {
                cout << "\t" << d.first << " " << d.second << "番" << endl;
            }
        }
        if (t.yizhong.empty()) {
            cout << "  役种: 无" << endl;
        }
    }
    return 0;
}