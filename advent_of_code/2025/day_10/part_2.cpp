#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define ll   long long
#define newl '\n'

using namespace std;

using light = bool;
using button = vector<int>;

struct machine {
    int size;
    vector<light> lights;
    vector<button> buttons;
    vector<int> joltage;
};

int to_int(string num) {
    int r = 0;

    for (char c : num) {
        r = r * 10 + (c - '0');
    }

    return r;
}

void parse_lights(machine &m, string str) {
    for (auto c : str) {
        switch (c) {
            case '.': {
                m.size += 1;
                m.lights.push_back(false);
                break;
            }
            case '#': {
                m.size += 1;
                m.lights.push_back(true);
                break;
            }
        }
    }
}

void parse_button(machine &m, string str) {
    button b;
    string p;
    stringstream ss(str);

    while (getline(ss, p, ',')) {
        int n = p.size();
        char first = p[0];
        char last = p[n - 1];

        if (first == '(' && last == ')') {
            b.push_back(to_int(p.substr(1, n - 1 - 1)));
            continue;
        }

        if (first == '(') {
            b.push_back(to_int(p.substr(1, n - 1)));
            continue;
        }

        if (last == ')') {
            b.push_back(to_int(p.substr(0, n - 1)));
            continue;
        }

        b.push_back(to_int(p));
    }

    m.buttons.push_back(b);
}

void parse_joltage(machine &m, string str) {
    string part;
    stringstream ss(str);

    while (getline(ss, part, ',')) {
        int n = part.size();
        char first = part[0];
        char last = part[n - 1];

        if (first == '{' && last == '}') {
            m.joltage.push_back(to_int(part.substr(1, n - 1 - 1)));
            continue;
        }

        if (first == '{') {
            m.joltage.push_back(to_int(part.substr(1, n - 1)));
            continue;
        }

        if (last == '}') {
            m.joltage.push_back(to_int(part.substr(0, n - 1)));
            continue;
        }

        m.joltage.push_back(to_int(part));
    }
}

void parse_machine(machine &m, string line) {
    string part;
    stringstream ss(line);

    while (getline(ss, part, ' ')) {
        switch (part[0]) {
            case '[': {
                parse_lights(m, part);
                break;
            }
            case '(': {
                parse_button(m, part);
                break;
            }
            case '{': {
                parse_joltage(m, part);
                break;
            }
        }
    }
}

struct state {
    int step;
    vector<int> joltage;
};

void increase_joltage(vector<int> &source, vector<int> &target, button &b) {
    for (auto i : b) {
        target[i] = source[i] + 1;
    }
}

void copy_joltage(vector<int> &source, vector<int> &target) {
    for (auto j : source) {
        target.push_back(j);
    }
}

bool compare_joltage(vector<int> &source, vector<int> &target) {
    for (int i = 0; i < source.size(); i++) {
        if (source[i] != target[i]) {
            return false;
        }
    }

    return true;
}

bool is_over_joltage(vector<int> &source, vector<int> &target) {
    for (int i = 0; i < source.size(); i++) {
        if (source[i] < target[i]) {
            return true;
        }
    }

    return false;
}

// Breath-first search
int find_fewest_steps(machine &m) {
    queue<state> q;

    q.push({ .step = 0, .joltage = vector<int>(m.joltage.size(), 0) });

    while (!q.empty()) {
        state s = q.front();
        q.pop();

        for (auto b : m.buttons) {
            vector<int> j;

            copy_joltage(s.joltage, j);
            increase_joltage(s.joltage, j, b);
            if (is_over_joltage(m.joltage, j)) {
                continue;
            }
            if (compare_joltage(m.joltage, j)) {
                return s.step + 1;
            }

            q.push({ .step = s.step + 1, .joltage = j });
        };
    }

    return 0;
}

void clear_machine(machine &m) {
    m.size = 0;
    m.lights.clear();
    m.buttons.clear();
    m.joltage.clear();
}

int main() {
    string filename = "input.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "error: " << filename << " does not exist" << newl;
        return 1;
    }

    string line;
    int answer = 0;
    machine m;

    while (getline(file, line)) {
        parse_machine(m, line);
        answer += find_fewest_steps(m);
        cout << answer << newl;
        clear_machine(m);
    }

    cout << answer << newl;

    file.close();
}
