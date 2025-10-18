#include <bits/stdc++.h>

using namespace std;

mt19937 rng(std::chrono::duration_cast<std::chrono::seconds>
    (std::chrono::system_clock::now().time_since_epoch()).count());

namespace fs = std::filesystem;

int randi(int a, int b) {
    return uniform_int_distribution<int>(a, b)(rng);
}

int main() {
    fs::path folder = "tests/in";
    fs::create_directories(folder);

    for (int i = 1; i <= 5; ++i)
    {
        fs::path filepath = folder / (std::to_string(i) + ".txt");
        ofstream test(filepath);
        if (!test) {
            std::cerr << "Error: could not open file for writing\n";
            return 1;
        }

        test << to_string(randi(1, 10)) << "\n";

        test.close();
    }
}
