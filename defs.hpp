#include<string>
#include<vector>
using namespace std;

extern string hupai;
extern string zifeng;
extern string changfeng;
extern vector<string> dora;
extern vector<string> ldora;
extern bool fulu;
extern bool wlizhi;
extern bool lizhi;
extern bool yifa;
extern bool zimo;
extern bool qianggang;
extern bool lingshang;
extern bool haidi;
extern bool hedi;

struct Tiles {
    vector<string> kezi;
    vector<string> shunzi;
    vector<string> gangzi;
    vector<string> duizi;
    vector<string> dazi;
    vector<string> danpai;
    vector<string> daixuan;
    vector<pair<string, int>> yizhong;
    int fushu = 0;
    int fanshu = 20;
    int dianshu = 0;
};

vector<string> parse_tiles(const string& input);
bool cmptile(const string& a, const string& b);
vector<string> sort_tiles(vector<string> tiles);
string next1(const string &tile);
string next2(const string &tile);
string prev1(const string &tile);
vector<Tiles> split_tiles(const vector<string>& tiles);
void count_tiles(vector<Tiles>& all);

bool duanyaojiu(const Tiles& tile);
bool pinghu(const Tiles& tile);
bool zifengpai(const Tiles& tile);
bool changfengpai(const Tiles& tile);
bool bai(const Tiles& tile);
bool fa(const Tiles& tile);
bool zhong(const Tiles& tile);
bool yibeikou(const Tiles& tile);