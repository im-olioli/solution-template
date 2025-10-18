#include <bits/stdc++.h>

using namespace std;
namespace fs = filesystem;

static string read_all(const fs::path &p) {
    ifstream f(p, ios::binary);
    if (!f) return {};
    ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

vector<vector<string>> split(const string& input) {
    vector<vector<string>> result;
    istringstream in(input);
    string line;

    while (getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        istringstream ls(line);

        vector<string> tokens{
            istream_iterator<string>{ls},
            istream_iterator<string>{}
        };

        if (!tokens.empty()) result.push_back(tokens);
    }
    return result;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cerr << "Usage: testrunner <program> <input_file> <expected_file>\n";
        return 2;
    }

    const fs::path program = argv[1];
    const fs::path input = argv[2];
    const fs::path expected = argv[3];

    if (!fs::exists(program)) {
        cerr << "Program not found: " << program << "\n";
        return 2;
    }
    if (!fs::exists(input)) {
        cerr << "Input not found: " << input << "\n";
        return 2;
    }

    const auto out_actual = fs::temp_directory_path() / ("actual_" + input.filename().string());

    const string cmd = "\"" + program.string() + "\" < \"" + input.string() + "\" > \"" + out_actual.string() + "\"";

    const auto t0 = chrono::steady_clock::now();
    const auto rc = system(cmd.c_str());
    const auto t1 = chrono::steady_clock::now();
    const auto ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();

    if (rc != 0) {
        cerr << "\033[31m[FAIL]\033[0m " << input.filename().string()
                  << " — program exit code: " << rc << " — time: " << ms << " ms\n";
        return 1;
    }

    const auto got = read_all(out_actual);

    if (!fs::exists(expected)) {
        cout << "\033[33m[INFO]\033[0m No expected output for " << input.filename().string()
                  << " — showing program output below\n";
        cout << "---------------------------------------------\n";
        cout << got;
        cout << "\n---------------------------------------------\n";
        cout << "[DONE] Time: " << ms << " ms\n";
        return 0;
    }

    const auto exp = read_all(expected);

    auto got_no_space = got;
    erase_if(got_no_space, ::isspace);
    auto exp_no_space = exp;
    erase_if(exp_no_space, ::isspace);

    if (got_no_space == exp_no_space) {
        cout << "\033[32m[PASS]\033[0m " << input.filename().string()
                  << " — time: " << ms << " ms\n";
        error_code ec;
        fs::remove(out_actual, ec);
        return 0;
    }

    cerr << "\033[31m[FAIL]\033[0m " << input.filename().string()
              << " — output differs — time: " << ms << " ms\n\n";

    const auto got_split = split(got);
    const auto exp_split = split(exp);

    size_t got_tokens = 0;
    size_t exp_tokens = 0;

    cerr << "Got:\n";
    for (size_t i = 0; i < got_split.size(); ++i) {
        if (exp_split.size() <= i) {
            cerr << "\033[31m- ";
            for (const auto& token : got_split[i]) {
                cerr << token << " ";
                ++got_tokens;
            }
            cerr << "\033[0m\n";
            continue;
        }

        cerr << "  ";
        const size_t m = max(got_split[i].size(), exp_split[i].size());
        for (size_t j = 0; j < m; ++j) {
            if (j >= exp_split[i].size()) {
                cerr << "\033[31m" << got_split[i][j] << " \033[0m";
                ++got_tokens;
                continue;
            }
            if (j >= got_split[i].size()) {
                for (size_t x = 0; x < exp_split[i][j].size(); ++x) cerr << ' ';
                cerr << ' ';
                continue;
            }

            const string& gtok = got_split[i][j];
            const string& etok = exp_split[i][j];

            const size_t k = max(gtok.size(), etok.size());
            bool colored = false;
            for (size_t x = 0; x < k; ++x) {
                if (x >= gtok.size()) { cerr << ' '; continue; }
                if (x >= etok.size()) {
                    if (!colored) { cerr << "\033[31m"; colored = true; }
                    cerr << gtok[x];
                    continue;
                }
                if (gtok[x] != etok[x]) {
                    if (!colored) { cerr << "\033[31m"; colored = true; }
                    cerr << gtok[x];
                } else {
                    if (colored) { cerr << "\033[0m"; colored = false; }
                    cerr << gtok[x];
                }
            }
            if (colored) { cerr << "\033[0m"; }
            cerr << ' ';
            ++got_tokens;
        }
        cerr << "\033[0m\n";
    }
    cerr << "Expected:\n";
    for (size_t i = 0; i < exp_split.size(); ++i) {
        if (got_split.size() <= i) {
            cerr << "\033[32m+ ";
            for (const auto& token : exp_split[i]) {
                cerr << token << " ";
                ++exp_tokens;
            }
            cerr << "\033[0m\n";
            continue;
        }

        cerr << "  ";
        const size_t m = max(got_split[i].size(), exp_split[i].size());
        for (size_t j = 0; j < m; ++j) {
            if (j >= got_split[i].size()) {
                cerr << "\033[32m" << exp_split[i][j] << "\033[0m ";
                ++exp_tokens;
                continue;
            }
            if (j >= exp_split[i].size()) {
                for (size_t x = 0; x < got_split[i][j].size(); ++x) cerr << ' ';
                cerr << ' ';
                continue;
            }

            const string& gtok = got_split[i][j];
            const string& etok = exp_split[i][j];

            const size_t k = max(gtok.size(), etok.size());
            bool colored = false;
            for (size_t x = 0; x < k; ++x) {
                if (x >= etok.size()) { cerr << ' '; continue; }
                if (x >= gtok.size()) {
                    if (!colored) { cerr << "\033[32m"; colored = true; }
                    cerr << etok[x];
                    continue;
                }
                if (etok[x] != gtok[x]) {
                    if (!colored) { cerr << "\033[32m"; colored = true; }
                    cerr << etok[x];
                } else {
                    if (colored) { cerr << "\033[0m"; colored = false; }
                    cerr << etok[x];
                }
            }
            if (colored) { cerr << "\033[0m"; }
            cerr << ' ';
            ++exp_tokens;
        }
        cerr << "\033[0m\n";
    }
    cerr << "---------------------------------------------\n";
    cerr << "Got tokens: " << got_tokens << " | Expected tokens: " << exp_tokens << "\n";
    return 1;
}