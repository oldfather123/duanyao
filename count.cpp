#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
#include"defs.hpp"

void count_fanshu(Tiles &cur){
    if (wlizhi) {
        lizhi = true;
        cur.yizhong.push_back({"两立直", 2});
        cur.fanshu += 2;
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
    if (qiduizi(cur) && !fulu) {
        cur.yizhong.push_back({"七对子", 2});
        cur.fanshu += 2;
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
    if (pinghu(cur) && !fulu) {
        cur.yizhong.push_back({"平和", 1});
        cur.fanshu += 1;
    }
    if (yibeikou(cur) && !fulu) {
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
    if (bai(cur) && !xsy) {
        cur.yizhong.push_back({"役牌：白", 1});
        cur.fanshu += 1;
    }
    if (fa(cur) && !xsy) {
        cur.yizhong.push_back({"役牌：发", 1});
        cur.fanshu += 1;
    }
    if (zhong(cur) && !xsy) {
        cur.yizhong.push_back({"役牌：中", 1});
        cur.fanshu += 1;
    }
    if (lizhi && !wlizhi) {
        cur.yizhong.push_back({"立直", 1});
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
}