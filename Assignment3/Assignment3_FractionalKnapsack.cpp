#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Item
{
    string name;
    double weight;
    double utility;
    bool divisible;
    double ratio;
};

// Function to calculate utility/weight ratio
void calculateRatio(vector<Item>& items)
{
    for (int i = 0; i < items.size(); i++)
    {
        items[i].ratio = items[i].utility / items[i].weight;
    }
}

// Display all items
void displayItems(const vector<Item>& items)
{
    cout << "\n";
    cout << "-------------------------------------------------------------\n";
    cout << left << setw(15) << "Item"
         << setw(12) << "Weight"
         << setw(12) << "Utility"
         << setw(12) << "Divisible"
         << setw(12) << "Ratio" << endl;
    cout << "-------------------------------------------------------------\n";

    for (const auto& item : items)
    {
        cout << left << setw(15) << item.name
             << setw(12) << item.weight
             << setw(12) << item.utility
             << setw(12) << (item.divisible ? "Yes" : "No")
             << setw(12) << fixed << setprecision(2) << item.ratio
             << endl;
    }

    cout << "-------------------------------------------------------------\n";
}

// Fractional Knapsack using Greedy method
double fractionalKnapsack(vector<Item>& items, double capacity)
{
    // Sort items according to utility/weight ratio
    sort(items.begin(), items.end(),
         [](const Item& a, const Item& b)
         {
             return a.ratio > b.ratio;
         });

    double remainingCapacity = capacity;
    double totalUtility = 0;

    cout << "\nItems selected for the boat:\n";
    cout << "-------------------------------------------------------------\n";

    for (auto& item : items)
    {
        if (remainingCapacity <= 0)
            break;

        // Take complete item if it fits
        if (item.weight <= remainingCapacity)
        {
            remainingCapacity -= item.weight;
            totalUtility += item.utility;

            cout << item.name << " -> Full"
                 << " | Weight: " << item.weight << " kg"
                 << " | Utility: " << item.utility << endl;
        }

        // Take fraction if item is divisible
        else if (item.divisible)
        {
            double fraction = remainingCapacity / item.weight;
            double obtainedUtility = fraction * item.utility;

            cout << item.name << " -> Partial"
                 << " | Fraction: " << fixed << setprecision(2)
                 << fraction
                 << " | Weight: " << remainingCapacity << " kg"
                 << " | Utility: " << obtainedUtility << endl;

            totalUtility += obtainedUtility;
            remainingCapacity = 0;
        }

        // Skip item if it is indivisible
        else
        {
            cout << item.name << " -> Skipped (Indivisible)"
                 << endl;
        }
    }

    cout << "-------------------------------------------------------------\n";

    return totalUtility;
}

int main()
{
    cout << "===============================================\n";
    cout << "     EMERGENCY RELIEF SUPPLY DISTRIBUTION\n";
    cout << "          FRACTIONAL KNAPSACK\n";
    cout << "===============================================\n";

    // Relief supply dataset
    vector<Item> items =
    {
        {"Medicine", 5, 50, false, 0},
        {"Food", 10, 30, true, 0},
        {"Water", 20, 20, true, 0}
    };

    // Calculate utility/weight ratio
    calculateRatio(items);

    cout << "\nAvailable Relief Supplies:";
    displayItems(items);

    double capacity;

    cout << "\nEnter boat capacity (kg): ";
    cin >> capacity;

    if (capacity <= 0)
    {
        cout << "Invalid capacity." << endl;
        return 0;
    }

    // Sort a copy to display preference order
    vector<Item> sortedItems = items;

    sort(sortedItems.begin(), sortedItems.end(),
         [](const Item& a, const Item& b)
         {
             return a.ratio > b.ratio;
         });

    cout << "\nGreedy Preference Order (Highest Utility/Weight Ratio):\n";

    for (int i = 0; i < sortedItems.size(); i++)
    {
        cout << i + 1 << ". "
             << sortedItems[i].name
             << " -> Ratio = "
             << fixed << setprecision(2)
             << sortedItems[i].ratio << endl;
    }

    double totalUtility = fractionalKnapsack(items, capacity);

    cout << "\nBoat Capacity: " << capacity << " kg";
    cout << "\nMaximum Utility Obtained: "
         << fixed << setprecision(2)
         << totalUtility << endl;

    cout << "\n===============================================\n";
    cout << "        RELIEF DISTRIBUTION COMPLETED\n";
    cout << "===============================================\n";

    return 0;
}