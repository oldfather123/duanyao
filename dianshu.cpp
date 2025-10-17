#include<unordered_map>
#include"defs.hpp"
using namespace std;

unordered_map<int, unordered_map<int, ZhuangDian>> zhuangjia;
void initzhuangjia() {
    zhuangjia[1][30]  = ZhuangDian(1500, 500);
    zhuangjia[1][40]  = ZhuangDian(2000, 700);
    zhuangjia[1][50]  = ZhuangDian(2400, 800);
    zhuangjia[1][60]  = ZhuangDian(2900, 1000);
    zhuangjia[1][70]  = ZhuangDian(3400, 1200);
    zhuangjia[1][80]  = ZhuangDian(3900, 1300);
    zhuangjia[1][90]  = ZhuangDian(4400, 1500);
    zhuangjia[1][100] = ZhuangDian(4800, 1600);

    zhuangjia[2][30]  = ZhuangDian(2900, 1000);
    zhuangjia[2][40]  = ZhuangDian(3900, 1300);
    zhuangjia[2][50]  = ZhuangDian(4800, 1600);
    zhuangjia[2][60]  = ZhuangDian(5800, 2000);
    zhuangjia[2][70]  = ZhuangDian(6800, 2300);
    zhuangjia[2][80]  = ZhuangDian(7700, 2600);
    zhuangjia[2][90]  = ZhuangDian(8700, 2900);
    zhuangjia[2][100] = ZhuangDian(9600, 3200);
    zhuangjia[2][110] = ZhuangDian(10600, 3600);

    zhuangjia[3][25] = ZhuangDian(4800, 1600);
    zhuangjia[3][30] = ZhuangDian(5800, 2000);
    zhuangjia[3][40] = ZhuangDian(7700, 2600);
    zhuangjia[3][50] = ZhuangDian(9600, 3200);
    zhuangjia[3][60] = ZhuangDian(11600, 3900);
    for (int i = 70; i <= 110; i += 10)
        zhuangjia[3][i] = ZhuangDian(12000, 4000);

    zhuangjia[4][25] = ZhuangDian(9600, 3200);
    zhuangjia[4][30] = ZhuangDian(11600, 3900);
    for (int i = 40; i <= 110; i += 10)
        zhuangjia[4][i] = ZhuangDian(12000, 4000);

    for (int i = 20; i <= 110; i += 10) {
        zhuangjia[5][i] = ZhuangDian(12000, 4000);
        zhuangjia[6][i] = ZhuangDian(18000, 6000);
        zhuangjia[7][i] = ZhuangDian(18000, 6000);
        zhuangjia[8][i] = ZhuangDian(24000, 8000);
        zhuangjia[9][i] = ZhuangDian(24000, 8000);
        zhuangjia[10][i] = ZhuangDian(24000, 8000);
        zhuangjia[11][i] = ZhuangDian(36000, 12000);
        zhuangjia[12][i] = ZhuangDian(36000, 12000);
        zhuangjia[13][i] = ZhuangDian(48000, 16000);
    }

    zhuangjia[5][25] = ZhuangDian(12000, 4000);
    zhuangjia[6][25] = ZhuangDian(18000, 6000);
    zhuangjia[7][25] = ZhuangDian(18000, 6000);
    zhuangjia[8][25] = ZhuangDian(24000, 8000);
    zhuangjia[9][25] = ZhuangDian(24000, 8000);
    zhuangjia[10][25] = ZhuangDian(24000, 8000);
    zhuangjia[11][25] = ZhuangDian(36000, 12000);
    zhuangjia[12][25] = ZhuangDian(36000, 12000);
    zhuangjia[13][25] = ZhuangDian(48000, 16000);
}

unordered_map<int, unordered_map<int, ZiDian>> zijia;
void initzijia() {
    zijia[1][30] = ZiDian(1000, 500, 300);
    zijia[1][40] = ZiDian(1300, 700, 400);
    zijia[1][50] = ZiDian(1600, 800, 400);
    zijia[1][60] = ZiDian(2000, 1000, 500);
    zijia[1][70] = ZiDian(2300, 1200, 600);
    zijia[1][80] = ZiDian(2600, 1300, 700);
    zijia[1][90] = ZiDian(2900, 1500, 800);
    zijia[1][100] = ZiDian(3200, 1600, 800);

    zijia[2][25] = ZiDian(1600, 800, 400);
    zijia[2][30] = ZiDian(2000, 1000, 500);
    zijia[2][40] = ZiDian(2600, 1300, 700);
    zijia[2][50] = ZiDian(3200, 1600, 800);
    zijia[2][60] = ZiDian(3900, 2000, 1000);
    zijia[2][70] = ZiDian(4500, 2300, 1200);
    zijia[2][80] = ZiDian(5200, 2600, 1300);
    zijia[2][90] = ZiDian(5800, 2900, 1500);
    zijia[2][100] = ZiDian(6400, 3200, 1600);
    zijia[2][110] = ZiDian(7100, 3600, 1800);

    zijia[3][25] = ZiDian(3200, 1600, 800);
    zijia[3][30] = ZiDian(3900, 2000, 1000);
    zijia[3][40] = ZiDian(5200, 2600, 1300);
    zijia[3][50] = ZiDian(6400, 3200, 1600);
    zijia[3][60] = ZiDian(7700, 3900, 2000);
    for (int i = 70; i <= 110; i += 10)
        zijia[3][i] = ZiDian(8000, 4000, 2000);
    
    zijia[4][25] = ZiDian(6400, 3200, 1600);
    zijia[4][30] = ZiDian(7700, 3900, 2000);
    for (int i = 40; i <= 110; i += 10)
        zijia[4][i] = ZiDian(8000, 4000, 2000);

    for (int i = 20; i <= 110; i += 10) {
        zijia[5][i] = ZiDian(8000, 4000, 2000);
        zijia[6][i] = ZiDian(12000, 6000, 3000);
        zijia[7][i] = ZiDian(12000, 6000, 3000);
        zijia[8][i] = ZiDian(16000, 8000, 4000);
        zijia[9][i] = ZiDian(16000, 8000, 4000);
        zijia[10][i] = ZiDian(16000, 8000, 4000);
        zijia[11][i] = ZiDian(24000, 12000, 6000);
        zijia[12][i] = ZiDian(24000, 12000, 6000);
        zijia[13][i] = ZiDian(32000, 16000, 8000);
    }

    zijia[5][25] = ZiDian(8000, 4000, 2000);
    zijia[6][25] = ZiDian(12000, 6000, 3000);
    zijia[7][25] = ZiDian(12000, 6000, 3000);
    zijia[8][25] = ZiDian(16000, 8000, 4000);
    zijia[9][25] = ZiDian(16000, 8000, 4000);
    zijia[10][25] = ZiDian(16000, 8000, 4000);
    zijia[11][25] = ZiDian(24000, 12000, 6000);
    zijia[12][25] = ZiDian(24000, 12000, 6000);
    zijia[13][25] = ZiDian(32000, 16000, 8000);
}