#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
#include<sstream>
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

int doracnt;
int ldoracnt;
int bdoracnt;
int cdoracnt;

FTiles ftile;

static string escape_json(const string &s) {
    string o;
    o.reserve(s.size());
    for (unsigned char c : s) {
        switch (c) {
            case '\"': o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\b': o += "\\b";  break;
            case '\f': o += "\\f";  break;
            case '\n': o += "\\n";  break;
            case '\r': o += "\\r";  break;
            case '\t': o += "\\t";  break;
            default:
                if (c < 0x20) {
                    char buf[8];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    o += buf;
                } else {
                    o += c;
                }
        }
    }
    return o;
}

int main() {
    initzhuangjia();
    initzijia();
    string hand;
    // cout << "输入手牌" << endl;
    cin >> hand;
    if (hand.length() < 26) {
        cerr << "Error: tiles length fail, current length " << hand.length() / 2 << endl;
        return 0;
    }
    // cout << "输入和牌" << endl;
    cin >> hupai;
    cdoracnt = 0;
    for (int i = 0; i < hand.length(); i += 2) {
        if (hand[i] == '0') {
            cdoracnt++;
            hand[i] = '5';
        }
    }
    if (hupai[0] == '0') {
        cdoracnt++;
        hupai[0] = '5';
    }
    // cout << "输入场风" << endl;
    cin >> changfeng;
    // cout << "输入自风" << endl;
    cin >> zifeng;
    // cout << "输入宝牌指示牌" << endl;
    string doras;
    cin >> doras;
    vector<string> ds = getdoras(doras);
    for (const auto &d : ds) {
        for (int i = 0; i < hand.length(); i += 2) {
            if (hand.substr(i, 2) == d) 
                doracnt++;
        }
        if (hupai == d) 
            doracnt++;
    }
    // cout << "两立直|立直|一发|自摸|副露|枪杠|岭上|海底|河底" << endl;
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
            else if (i == 7) hedi = true;
            else if (i == 8) haidi = true;
        }
    }
    if (wlizhi || lizhi) {
        // cout << "输入里宝指示牌" << endl;
        string ldoras;
        cin >> ldoras;
        vector<string> lds = getdoras(ldoras);
        for (const auto &ld : lds) {
        for (int i = 0; i < hand.length(); i += 2) {
            if (hand.substr(i, 2) == ld) 
                ldoracnt++;
        }
        if (hupai == ld) 
            ldoracnt++;
    }
    }
    vector<string> tiles = parse_tiles(hand);
    tiles = sort_tiles(tiles);
    // for (int i = 0; i < tiles.size(); i++) {
    //     cout << tiles[i];
    // }
    // cout << endl;
    vector<Tiles> results = split_tiles(tiles);
    // cout << "Total combinations: " << results.size() << endl;
    if (results.empty()) {
        cout << "无法识别的牌型" << endl;
        return 0;
    }
    // for (int i = 0; i < results.size(); i++) {
    //     const Tiles &t = results[i];
    //     cout << "牌型 " << (i + 1) << ":" << endl;
    //     if (!t.gangzi.empty()) {
    //         cout << "  杠子: ";
    //         for (const auto &g : t.gangzi) 
    //             cout << g << " ";
    //         cout << endl;
    //     }
    //     if (!t.kezi.empty()) {
    //         cout << "  刻子: ";
    //         for (const auto &k : t.kezi) 
    //             cout << k << " ";
    //         cout << endl;
    //     }
    //     if (!t.shunzi.empty()) {
    //         cout << "  顺子: ";
    //         for (const auto &s : t.shunzi) 
    //             cout << s << " ";
    //         cout << endl;
    //     }
    //     if (!t.duizi.empty()) {
    //         cout << "  对子: ";
    //         for (const auto &d : t.duizi) 
    //             cout << d << " ";
    //         cout << endl;
    //     }
    //     if (!t.dazi.empty()) {
    //         cout << "  搭子: ";
    //         for (const auto &d : t.dazi) 
    //             cout << d << " ";
    //         cout << endl;
    //     }
    //     if (!t.danpai.empty()) {
    //         cout << "  单张: ";
    //         for (const auto &d : t.danpai) 
    //             cout << d << " ";
    //         cout << endl;
    //     }
    // }
    int choice = 1;
    if (results.size() > 1) {
        // cout << "选择牌型" << endl;
        cin >> choice;
    }
    Tiles tile = results[choice - 1];
    if (fulu) {
        cout << "选择副露牌" << endl;
        if (!tile.gangzi.empty()) {
            cout << "  杠子: ";
            for (const auto &g : tile.gangzi) 
                cout << g << " ";
            cout << endl;
            string tags;
            cin >> tags;
            if (tags[0] != '0'){
                for (int i = 0; i < tags.length(); i++) {
                    ftile.gangzi.push_back(tile.gangzi[tags[i] - '1']);
                }
            }
        }
        if (!tile.kezi.empty()) {
            cout << "  刻子: ";
            for (const auto &k : tile.kezi) 
                cout << k << " ";
            cout << endl;
            string tags;
            cin >> tags;
            if (tags[0] != '0'){
                for (int i = 0; i < tags.length(); i++) {
                    ftile.kezi.push_back(tile.kezi[tags[i] - '1']);
                }
            }
        }
        if (!tile.shunzi.empty()) {
            cout << "  顺子: ";
            for (const auto &s : tile.shunzi) 
                cout << s << " ";
            cout << endl;
            string tags;
            cin >> tags;
            if (tags[0] != '0'){
                for (int i = 0; i < tags.length(); i++) {
                    ftile.shunzi.push_back(tile.shunzi[tags[i] - '1']);
                }
            }
        }
    }
    count_fanshu(tile);
    count_fushu(tile);
    // if (!tile.yizhong.empty()) {
    //     cout << "役种: " << endl;
    //     for (const auto &d : tile.yizhong) {
    //         cout << "\t" << d.first << " " << d.second << "番" << endl;      
    //     }
    //     cout << tile.fanshu << "番 " << tile.fushu << "符" << endl;
    //     if (tile.fanshu == 3 && tile.fushu >= 70)
    //         cout << "满贯 ";
    //     if (tile.fanshu == 4 && tile.fushu >= 40)
    //         cout << "满贯 ";
    //     if (tile.fanshu == 5)
    //         cout << "满贯 ";
    //     if (tile.fanshu >= 6 && tile.fanshu <= 7)
    //         cout << "跳满 ";
    //     if (tile.fanshu >= 8 && tile.fanshu <= 10)
    //         cout << "倍满 ";    
    //     if (tile.fanshu >= 11 && tile.fanshu <= 12)
    //         cout << "三倍满 ";
    //     if (tile.fanshu >= 13)
    //         cout << "累计役满 ";
    //     int f = tile.fanshu > 13 ? 13 : tile.fanshu;
    //     if (zifeng == "1z")
    //         cout << zhuangjia[f][tile.fushu].total << "点" << endl;
    //     else
    //         cout << zijia[f][tile.fushu].total << "点" << endl;
    // }
    // if (tile.yizhong.empty()) {
    //     cout << "役种: 无" << endl;
    // }
    ostringstream js;
    js << "{";

    js << "\"yizhong\":[";
    bool first = true;
    for (const auto &d : tile.yizhong) {
        if (!first) 
            js << ",";
        first = false;
        js << "{";
        js << "\"name\":\"" << escape_json(d.first) << "\"";
        js << ",\"fans\":" << d.second;
        js << "}";
    }
    js << "],";

    js << "\"fanshu\":" << tile.fanshu << ",";
    js << "\"fushu\":" << tile.fushu << ",";

    vector<string> titles;
    if (tile.fanshu == 3 && tile.fushu >= 70) titles.push_back("满贯");
    if (tile.fanshu == 4 && tile.fushu >= 40) titles.push_back("满贯");
    if (tile.fanshu == 5) titles.push_back("满贯");
    if (tile.fanshu >= 6 && tile.fanshu <= 7) titles.push_back("跳满");
    if (tile.fanshu >= 8 && tile.fanshu <= 10) titles.push_back("倍满");
    if (tile.fanshu >= 11 && tile.fanshu <= 12) titles.push_back("三倍满");
    if (tile.fanshu >= 13) titles.push_back("累计役满");

    js << "\"titles\":[";
    for (size_t i = 0; i < titles.size(); ++i) {
        if (i) js << ",";
        js << "\"" << escape_json(titles[i]) << "\"";
    }
    js << "],";

    int fidx = tile.fanshu > 13 ? 13 : tile.fanshu;
    int totalPoints = 0;
    string owner = "zijia";
    if (zifeng == "1z") {
        owner = "zhuangjia";
        totalPoints = zhuangjia[fidx][tile.fushu].total;
    } 
    else {
        owner = "zijia";
        totalPoints = zijia[fidx][tile.fushu].total;
    }
    js << "\"score_owner\":\"" << escape_json(owner) << "\",";
    js << "\"score\":" << totalPoints;

    js << "}";

    cout << js.str() << endl;
    return 0;
}