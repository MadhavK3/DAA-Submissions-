#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Item {
    string name;
    int weight;
    int utility;
    bool perishable;
};

int main() {
    int n, W;

    cout << "===== Disaster Relief Resource Allocation =====\n";
    cout << "Enter number of relief items: ";
    cin >> n;

    if (n <= 0) {
        cout << "Number of items must be positive.\n";
        return 0;
    }

    vector<Item> items(n);

    for (int i = 0; i < n; i++) {
        cout << "\nItem " << i + 1 << ":\n";
        cout << "Enter item name: ";
        cin >> ws;
        getline(cin, items[i].name);

        cout << "Enter weight (kg): ";
        cin >> items[i].weight;

        cout << "Enter utility value: ";
        cin >> items[i].utility;

        cout << "Is it perishable/life-saving? (1 = Yes, 0 = No): ";
        cin >> items[i].perishable;

        if (items[i].weight <= 0 || items[i].utility < 0) {
            cout << "Invalid weight or utility value.\n";
            return 0;
        }
    }

    cout << "\nEnter truck capacity (kg): ";
    cin >> W;

    if (W < 0) {
        cout << "Capacity cannot be negative.\n";
        return 0;
    }

    const int PRIORITY_BONUS = 20;
    vector<int> adjustedUtility(n);

    for (int i = 0; i < n; i++) {
        adjustedUtility[i] = items[i].utility;

        if (items[i].perishable) {
            adjustedUtility[i] += PRIORITY_BONUS;
        }
    }

    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];

            if (items[i - 1].weight <= w) {
                dp[i][w] = max(
                    dp[i][w],
                    adjustedUtility[i - 1] +
                    dp[i - 1][w - items[i - 1].weight]
                );
            }
        }
    }

    vector<int> selected;
    int remainingCapacity = W;

    for (int i = n; i >= 1; i--) {
        if (dp[i][remainingCapacity] !=
            dp[i - 1][remainingCapacity]) {
            selected.push_back(i - 1);
            remainingCapacity -= items[i - 1].weight;
        }
    }

    reverse(selected.begin(), selected.end());

    int totalWeight = 0;
    int totalOriginalUtility = 0;
    int totalAdjustedUtility = 0;

    cout << "\n===== Selected Relief Items =====\n";
    cout << left << setw(25) << "Item Name"
         << setw(12) << "Weight"
         << setw(12) << "Utility"
         << setw(15) << "Priority"
         << setw(18) << "Adjusted Utility" << '\n';

    for (int index : selected) {
        cout << left << setw(25) << items[index].name
             << setw(12) << items[index].weight
             << setw(12) << items[index].utility
             << setw(15)
             << (items[index].perishable ? "High" : "Normal")
             << setw(18) << adjustedUtility[index] << '\n';

        totalWeight += items[index].weight;
        totalOriginalUtility += items[index].utility;
        totalAdjustedUtility += adjustedUtility[index];
    }

    cout << "\n===== Final Report =====\n";
    cout << "Truck Capacity: " << W << " kg\n";
    cout << "Total Weight Carried: " << totalWeight << " kg\n";
    cout << "Total Original Utility: "
         << totalOriginalUtility << " units\n";
    cout << "Total Adjusted Utility: "
         << totalAdjustedUtility << " units\n";

    cout << "\nTime Complexity: O(N * W)\n";
    cout << "Space Complexity: O(N * W)\n";

    return 0;
}
