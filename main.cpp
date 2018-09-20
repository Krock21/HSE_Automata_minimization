#include <bits/stdc++.h>

using namespace std;

string alphabet;
int n, q0, termcnt;
vector<bool> isterm;
vector<vector<int>> g;
vector<vector<vector<int>>> pg;
vector<vector<bool>> used;
vector<bool> isreachable;

int q2, classcnt, termcnt2;
vector<bool> isterm2;
vector<int> classnum;
vector<vector<int>> g2;

void reachdfs(int v) {
    if (isreachable[v]) return;
    isreachable[v] = true;
    for (int i = 0; i < alphabet.size(); ++i) {
        reachdfs(g[v][i]);
    }
}

int main(int argc, char **argv) {
    cin >> alphabet;
    cin >> n >> q0 >> termcnt;
    q0--;
    isterm.resize(n);
    isreachable.resize(n);
    classnum.resize(n, -1);
    used.resize(n, vector<bool>(n));
    g.resize(n, vector<int>(alphabet.size()));
    pg.resize(n, vector<vector<int>>(alphabet.size()));
    for (int i = 0; i < termcnt; ++i) {
        int cur;
        cin >> cur;
        cur--;
        isterm[cur] = true;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < alphabet.size(); ++j) {
            cin >> g[i][j];
            g[i][j]--;
            pg[g[i][j]][j].push_back(i);
        }
    }
    reachdfs(q0);
    queue<pair<int, int>> q;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (!isreachable[i] || !isreachable[j]) continue;
            if (isterm[i] != isterm[j]) {
                used[i][j] = used[j][i] = true;
                q.push({i, j});
            }
        }
    }
    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();
        for (int i = 0; i < alphabet.size(); ++i) {
            for (auto k1 : pg[cur.first][i]) {
                for (auto k2: pg[cur.second][i]) {
                    if (!isreachable[k1] || !isreachable[k2] || k1 == k2 || used[k1][k2]) continue;
                    used[k1][k2] = used[k2][k1] = true;
                    q.push({k1, k2});
                }
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        if (!isreachable[i]) continue;
        for (int j = 0; j < i; ++j) {
            if (!isreachable[j]) continue;
            if (!used[i][j]) {
                classnum[i] = classnum[j];
                if (isterm[i]) {
                    if (!isterm2[classnum[i]]) {
                        termcnt2++;
                        isterm2[classnum[i]] = true;
                    }
                }
                if (i == q0) q2 = classnum[i];
                break;
            }
        }
        if (classnum[i] == -1) {
            classnum[i] = classcnt;
            if (isterm[i]) termcnt2++;
            if (i == q0) q2 = classnum[i];
            isterm2.push_back(isterm[i]);
            classcnt++;
        }
    }
    g2.resize(classcnt, vector<int>(alphabet.size()));
    for (int i = 0; i < n; ++i) {
        if (!isreachable[i]) continue;
        for (int j = 0; j < alphabet.size(); ++j) {
            g2[classnum[i]][j] = classnum[g[i][j]];
        }
    }
    if (argc > 1 && strcmp(argv[1], "-raw") == 0) {
        cout << classcnt << "\n";
        cout << q2 + 1 << " " << termcnt2 << " ";
        for (int i = 0; i < classcnt; ++i) {
            if (isterm2[i]) cout << i + 1 << " ";
        }
        cout << "\n";
        for (int i = 0; i < classcnt; ++i) {
            for (int j = 0; j < alphabet.size(); ++j) {
                cout << g2[i][j] + 1 << " ";
            }
            cout << "\n";
        }
    } else {
        cout << "digraph automata {\n";
        cout << "node [ shape = doublecircle ]; ";
        bool is = false;
        for (int i = 0; i < classcnt; ++i) {
            if (isterm2[i]) {
                is = true;
                cout << "" << i + 1 << " ";
            }
        }
        if (is) cout << ";";
        cout << "\n";
        is = false;
        cout << "node [ shape = box ]; ";
        cout << "" << q2 + 1 << ";\n";
        cout << "node [ shape = circle ]; ";
        for (int i = 0; i < classcnt; ++i) {
            if (!isterm2[i] && i != q2) {
                is = true;
                cout << "" << i + 1 << " ";
            }
        }
        if (is) cout << ";";
        cout << "\n";
        for (int i = 0; i < classcnt; ++i) {
            for (int j = 0; j < alphabet.size(); ++j) {
                cout << "" << i + 1 << " -> " << g2[i][j] + 1 << " [ label = \"" << alphabet[j] << "\" ];\n";
            }
        }
        cout << "}\n";
    }

}