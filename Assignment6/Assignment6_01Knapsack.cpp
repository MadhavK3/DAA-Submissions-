
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>
using namespace std;

struct Item {
    string name;
    int weight, value;
};

struct State {
    int weight, value;
    vector<string> items;
};

int main() {
    int n, W;

    cout << "============================================================\n";
    cout << "    RELIEF SUPPLY KNAPSACK (DP with Merging & Pruning)\n";
    cout << "============================================================\n";

    cout << "Enter number of items: ";
    cin >> n;

    cout << "Enter truck capacity (kg): ";
    cin >> W;

    if (n <= 0 || W < 0) {
        cout << "Invalid number of items or truck capacity.\n";
        return 1;
    }

    vector<Item> a(n);

    cout << "Enter each item (name weight value):\n";

    for (int i = 0; i < n; i++) {
        cin >> a[i].name >> a[i].weight >> a[i].value;

        if (a[i].weight <= 0 || a[i].value < 0) {
            cout << "Invalid weight or utility value.\n";
            return 1;
        }
    }

    vector<State> dp = {{0, 0, {}}};
    int totalMerged = 0;
    int totalPruned = 0;

    for (int i = 0; i < n; i++) {
        const Item& item = a[i];

        cout << "\nProcessing Item " << i + 1 << ": "
             << item.name << " (" << item.weight
             << " kg, " << item.value << ")\n";

        vector<State> next = dp;
        int merged = 0;
        int pruned = 0;

        for (const State& s : dp) {
            if (s.weight + item.weight <= W) {
                State candidate = s;
                candidate.weight += item.weight;
                candidate.value += item.value;
                candidate.items.push_back(item.name);
                next.push_back(candidate);
            } else {
                pruned++;
            }
        }

        map<int, State> uniqueStates;

        for (const State& s : next) {
            auto it = uniqueStates.find(s.weight);

            if (it == uniqueStates.end()) {
                uniqueStates[s.weight] = s;
            } else {
                merged++;

                if (s.value > it->second.value) {
                    uniqueStates[s.weight] = s;
                }
            }
        }

        dp.clear();
        int bestValue = -1;

        for (const auto& entry : uniqueStates) {
            const State& s = entry.second;

            if (s.value > bestValue) {
                dp.push_back(s);
                bestValue = s.value;
            } else {
                pruned++;
            }
        }

        totalMerged += merged;
        totalPruned += pruned;

        cout << "  - States after merging: " << uniqueStates.size()
             << "\n";
        cout << "  - Merged states: " << merged
             << " (same weight, kept higher value)\n";
        cout << "  - Pruned states: " << pruned
             << " (over capacity or dominated)\n";
        cout << "  - States remaining: " << dp.size() << "\n";
    }

    State best = dp[0];

    for (const State& s : dp) {
        if (s.value > best.value)
            best = s;
    }

    cout << "\n============================================================\n";
    cout << "                       FINAL REPORT\n";
    cout << "============================================================\n";

    cout << "Selected Items:\n";

    if (best.items.empty()) {
        cout << "No items selected.\n";
    } else {
        for (int i = 0; i < (int)best.items.size(); i++) {
            cout << i + 1 << ". " << best.items[i] << "\n";
        }
    }

    cout << "------------------------------------------------------------\n";
    cout << "Total Weight Carried : " << best.weight << " kg\n";
    cout << "Maximum Utility      : " << best.value << "\n";
    cout << "Total Merged States  : " << totalMerged << "\n";
    cout << "Total Pruned States  : " << totalPruned << "\n";
    cout << "============================================================\n";

    cout << "\nTime Complexity: Depends on the number of states generated.\n";
    cout << "Space Complexity: Depends on the number of states retained.\n";

    return 0;
}
`