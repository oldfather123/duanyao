#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <cstdlib>
using namespace std;

void initialize_users_table(MYSQL *conn) {
    const char *create_table_query = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INT AUTO_INCREMENT PRIMARY KEY,
            username VARCHAR(255) NOT NULL UNIQUE,
            password VARCHAR(255) NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
    )";

    if (mysql_query(conn, create_table_query) != 0) {
        cerr << "Failed to initialize users table: " << mysql_error(conn) << endl;
        exit(EXIT_FAILURE); // 如果初始化失败，直接退出程序
    }
}

static string extract_json_field(const string &s, const string &key) {
    string pat = "\"" + key + "\"";
    auto p = s.find(pat);
    if (p == string::npos) return "";
    p = s.find(':', p + pat.size());
    if (p == string::npos) return "";
    // Skip spaces
    p++;
    while (p < s.size() && isspace((unsigned char)s[p])) p++;
    if (p >= s.size()) return "";
    if (s[p] == '"') {
        p++;
        size_t q = p;
        string out;
        while (q < s.size()) {
            if (s[q] == '\\' && q + 1 < s.size()) { out += s[q+1]; q += 2; continue; }
            if (s[q] == '"') break;
            out += s[q++];
        }
        return out;
    } else {
        // bare token
        size_t q = p;
        while (q < s.size() && !isspace((unsigned char)s[q]) && s[q] != ',' && s[q] != '}') q++;
        return s.substr(p, q - p);
    }
}

void handle_auth_if_present() {
    // 读一行看看是不是 JSON auth 请求
    string line;
    // 使用 peek 判断是否有数据可读（stdin may block）——尽量用 getline 非阻塞场景由 server 控制
    if (!std::getline(cin, line)) return;
    // 如果不是 JSON，则把该行放回到 cin 流的后续处理 — 但简单实现：如果不是 JSON，存入 global buffer for later parsing
    if (line.empty() || line[0] != '{') {
        // 把读取的首行放入一个 global variable consumed_input，以便后续 main 使用（简化处理）
        // 这里做简单处理：store in a file /tmp/duanyao_input.tmp（或在程序协议中由 server 先处理 auth）
        // 为简洁：如果不是 JSON，设置 cin 恢复读取用一个 istringstream
        static string leftover = "";
        leftover = line + "\n";
        // 替换 cin buffer by stringstream: not trivial; 这里直接 push leftover into a global input stream for later use is omitted.
        // 如果你使用 server 来区分 auth 路径，请确保 server 在 auth 模式下发送 JSON 并关闭 stdin。
        return;
    }

    string action = extract_json_field(line, "action");
    string username = extract_json_field(line, "username");
    string password = extract_json_field(line, "password");

    // DB 参数从环境变量读取
    const char *db_host = getenv("DB_HOST") ? getenv("DB_HOST") : "127.0.0.1";
    const char *db_port_s = getenv("DB_PORT");
    unsigned int db_port = db_port_s ? (unsigned int)atoi(db_port_s) : 3306;
    const char *db_user = getenv("DB_USER") ? getenv("DB_USER") : "root";
    const char *db_pass = getenv("DB_PASS") ? getenv("DB_PASS") : "lzh20050902";
    const char *db_name = getenv("DB_NAME") ? getenv("DB_NAME") : "duanyao";

    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        cout << "{\"success\":false,\"error\":\"mysql_init failed\"}" << endl;
        return;
    }
    if (!mysql_real_connect(conn, db_host, db_user, db_pass, db_name, db_port, NULL, 0)) {
        cout << "{\"success\":false,\"error\":\"db connect failed\"}" << endl;
        mysql_close(conn);
        return;
    }

    // 简单实现：register / login
    if (action == "register") {
        // 检查是否存在
        string q = "SELECT id FROM users WHERE username = '" + username + "' LIMIT 1";
        if (mysql_query(conn, q.c_str()) != 0) {
            cout << "{\"success\":false,\"error\":\"db query failed\"}" << endl;
            mysql_close(conn);
            return;
        }
        MYSQL_RES *res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            if (res) mysql_free_result(res);
            cout << "{\"success\":false,\"error\":\"username exists\"}" << endl;
            mysql_close(conn);
            return;
        }
        if (res) mysql_free_result(res);
        // 插入（注意：示例直接存明文，生产请用哈希）
        string ins = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
        if (mysql_query(conn, ins.c_str()) != 0) {
            cout << "{\"success\":false,\"error\":\"db insert failed\"}" << endl;
            mysql_close(conn);
            return;
        }
        cout << "{\"success\":true}" << endl;
        mysql_close(conn);
        return;
    } else if (action == "login") {
        string q = "SELECT password FROM users WHERE username = '" + username + "' LIMIT 1";
        if (mysql_query(conn, q.c_str()) != 0) {
            cout << "{\"success\":false,\"error\":\"db query failed\"}" << endl;
            mysql_close(conn);
            return;
        }
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res || mysql_num_rows(res) == 0) {
            if (res) mysql_free_result(res);
            cout << "{\"success\":false,\"error\":\"user not found\"}" << endl;
            mysql_close(conn);
            return;
        }
        MYSQL_ROW row = mysql_fetch_row(res);
        string stored = row[0] ? row[0] : "";
        mysql_free_result(res);
        if (stored == password) {
            cout << "{\"success\":true}" << endl;
        } else {
            cout << "{\"success\":false,\"error\":\"invalid password\"}" << endl;
        }
        mysql_close(conn);
        return;
    } else {
        cout << "{\"success\":false,\"error\":\"unknown action\"}" << endl;
        mysql_close(conn);
        return;
    }
}

int main() {
    // DB 参数从环境变量读取
    const char *db_host = getenv("DB_HOST") ? getenv("DB_HOST") : "127.0.0.1";
    const char *db_port_s = getenv("DB_PORT");
    unsigned int db_port = db_port_s ? (unsigned int)atoi(db_port_s) : 3306;
    const char *db_user = getenv("DB_USER") ? getenv("DB_USER") : "root";
    const char *db_pass = getenv("DB_PASS") ? getenv("DB_PASS") : "lzh20050902";
    const char *db_name = getenv("DB_NAME") ? getenv("DB_NAME") : "duanyao";

    // 初始化 MySQL 连接
    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        cerr << "Failed to initialize MySQL connection" << endl;
        return EXIT_FAILURE;
    }
    if (!mysql_real_connect(conn, db_host, db_user, db_pass, db_name, db_port, NULL, 0)) {
        cerr << "Failed to connect to MySQL: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // 初始化 users 表
    initialize_users_table(conn);

    // 关闭连接
    mysql_close(conn);
    handle_auth_if_present();
    return 0;
}