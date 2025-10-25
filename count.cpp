#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
#include"defs.hpp"

bool ph = false;
bool qd = false;

string getdora(const string &dora) {
    char next;
    if (dora[1] == 'z') {
        if (dora[0] == '4')
            next = '1';
        else if (dora[0] == '7')
            next = '5';
        else
            next = dora[0] + 1;
    }
    else {
        if (dora[0] == '9')
            next = '1';
        else
            next = dora[0] + 1;
    }
    return string{next, dora[1]};
}

vector<string> getdoras(const string &doras) {
    vector<string> res;
    for (int i = 0; i < doras.length(); i += 2) {
        res.push_back(getdora(doras.substr(i, 2)));
    }
    return res;
}

void count_fanshu(Tiles &cur) {
    if (wlizhi) {
        lizhi = true;
        fulu = false;
        cur.yizhong.push_back({"两立直", 2});
        cur.fanshu += 2;
    }
    if (lizhi && !wlizhi) {
        fulu = false;
        cur.yizhong.push_back({"立直", 1});
        cur.fanshu += 1;
    }
    if (qingyise(cur)) {
        if (fulu) {
            cur.yizhong.push_back({"清一色", 5});
            cur.fanshu += 5;
        }
        else {
            cur.yizhong.push_back({"清一色", 6});
            cur.fanshu += 6;
        }
    }
    bool ebk = false;
    if (!fulu && erbeikou(cur)) {
        cur.yizhong.push_back({"二杯口", 3});
        cur.fanshu += 3;
        ebk = true;
    }
    if (chunquan(cur)) {
        if (fulu) {
            cur.yizhong.push_back({"纯全带幺九", 2});
            cur.fanshu += 2;
        }
        else {
            cur.yizhong.push_back({"纯全带幺九", 3});
            cur.fanshu += 3;
        }
    }
    if (hunyise(cur)) {
        if (fulu) {
            cur.yizhong.push_back({"混一色", 2});
            cur.fanshu += 2;
        }
        else {
            cur.yizhong.push_back({"混一色", 3});
            cur.fanshu += 3;
        }
    }
    if (sansetongke(cur)) {
        cur.yizhong.push_back({"三色同刻", 2});
        cur.fanshu += 2;
    }
    if (sangangzi(cur)) {
        cur.yizhong.push_back({"三杠子", 2});
        cur.fanshu += 2;
    }
    if (duiduihu(cur)) {
        cur.yizhong.push_back({"对对和", 2});
        cur.fanshu += 2;
    }
    if (sananke(cur, ftile)) {
        cur.yizhong.push_back({"三暗刻", 2});
        cur.fanshu += 2;
    }
    bool xsy = false;
    if (xiaosanyuan(cur)) {
        cur.yizhong.push_back({"小三元", 2});
        cur.fanshu += 2;
        xsy = true;
    }
    if (hunlaotou(cur)) {
        cur.yizhong.push_back({"混老头", 2});
        cur.fanshu += 2;
    }
    if (!fulu && qiduizi(cur)) {
        cur.yizhong.push_back({"七对子", 2});
        cur.fanshu += 2;
        qd = true;
    }
    if (hunquan(cur)) {
        if (fulu) {
            cur.yizhong.push_back({"混全带幺九", 1});
            cur.fanshu += 1;
        }
        else {
            cur.yizhong.push_back({"混全带幺九", 2});
            cur.fanshu += 2;
        }
    }
    if (yiqi(cur)) {
        if (fulu) {
            cur.yizhong.push_back({"一气通贯", 1});
            cur.fanshu += 1;
        }
        else {
            cur.yizhong.push_back({"一气通贯", 2});
            cur.fanshu += 2;
        }
    }
    if (sansetongshun(cur)) {
        if (fulu) {
            cur.yizhong.push_back({"三色同顺", 1});
            cur.fanshu += 1;
        }
        else {
            cur.yizhong.push_back({"三色同顺", 2});
            cur.fanshu += 2;
        }
    }
    if (duanyaojiu(cur)) {
        cur.yizhong.push_back({"断幺九", 1});
        cur.fanshu += 1;
    }
    if (!fulu && pinghu(cur)) {
        cur.yizhong.push_back({"平和", 1});
        cur.fanshu += 1;
        ph = true;
    }
    if (!fulu && !ebk && yibeikou(cur)) {
        cur.yizhong.push_back({"一杯口", 1});
        cur.fanshu += 1;
    }
    if (changfengpai(cur)) {
        cur.yizhong.push_back({"役牌：场风牌", 1});
        cur.fanshu += 1;
    }
    if (zifengpai(cur)) {
        cur.yizhong.push_back({"役牌：自风牌", 1});
        cur.fanshu += 1;
    }
    if (!xsy && bai(cur)) {
        cur.yizhong.push_back({"役牌：白", 1});
        cur.fanshu += 1;
    }
    if (!xsy && fa(cur)) {
        cur.yizhong.push_back({"役牌：发", 1});
        cur.fanshu += 1;
    }
    if (!xsy && zhong(cur)) {
        cur.yizhong.push_back({"役牌：中", 1});
        cur.fanshu += 1;
    }
    if (zimo && !fulu) {
        cur.yizhong.push_back({"门前清自摸和", 1});
        cur.fanshu += 1;
    }
    if (yifa && lizhi) {
        cur.yizhong.push_back({"一发", 1});
        cur.fanshu += 1;
    }
    if (qianggang) {
        cur.yizhong.push_back({"枪杠", 1});
        cur.fanshu += 1;
    }
    if (lingshang) {
        cur.yizhong.push_back({"岭上开花", 1});
        cur.fanshu += 1;
    }
    if (haidi) {
        cur.yizhong.push_back({"海底捞月", 1});
        cur.fanshu += 1;
    }
    if (hedi) {
        cur.yizhong.push_back({"河底摸鱼", 1});
        cur.fanshu += 1;
    }
    if (doracnt != 0) {
        cur.yizhong.push_back({"宝牌", doracnt});
        cur.fanshu += doracnt;
    }
    if (cdoracnt != 0) {
        cur.yizhong.push_back({"赤宝牌", cdoracnt});
        cur.fanshu += cdoracnt;
    }
    if (wlizhi || lizhi) {
        cur.yizhong.push_back({"里宝牌", ldoracnt});
        cur.fanshu += ldoracnt;
    }
}

void count_fushu(Tiles &cur) {
    if (ph && zimo && !fulu) {
        cur.fushu = 20;
        return;
    }
    if (qd) {
        cur.fushu = 25;
        return;
    }
    for (const auto &k : cur.kezi) {
        bool m = false, yj = false;
        int f = 0;
        if (count(ftile.kezi.begin(), ftile.kezi.end(), k) > 0)
            m = true;
        if (is_yaojiu(k.substr(0, 2)))
            yj = true;
        if (m) {
            if (yj)
                f = 4;
            else
                f = 2;
        }
        else {
            if (yj)
                f = 8;
            else
                f = 4;
        }
        cur.fushu += f;
    }
    for (const auto &g : cur.gangzi) {
        bool m = false, yj = false;
        int f = 0;
        if (count(ftile.gangzi.begin(), ftile.gangzi.end(), g) > 0)
            m = true;
        if (is_yaojiu(g.substr(0, 2)))
            yj = true;
        if (m) {
            if (yj)
                f = 16;
            else
                f = 8;
        }
        else {
            if (yj)
                f = 32;
            else
                f = 16;
        }
        cur.fushu += f;
    }
    for (const auto &d : cur.duizi) {
        if (d.substr(0, 2) == zifeng && d.substr(0, 2) == changfeng)
            cur.fushu += 4;
        if (d.substr(0, 2) == zifeng && d.substr(0, 2) != changfeng)
            cur.fushu += 2;
        if (d.substr(0, 2) != zifeng && d.substr(0, 2) == changfeng)
            cur.fushu += 2;
        if (d.substr(0, 2) == "5z" || d.substr(0, 2) == "6z" || d.substr(0, 2) == "7z")
            cur.fushu += 2;
    }
    if (cur.daixuan.size() == 1) 
        cur.fushu += 2;
    if (!ph && zimo)
        cur.fushu += 2;
    if (!fulu && !zimo)
        cur.fushu += 10;
    if (fulu && cur.fushu < 30)
        cur.fushu = 30;
    cur.fushu = (cur.fushu + 9) / 10 * 10;
}