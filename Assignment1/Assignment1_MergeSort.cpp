#include <iostream>
#include <vector>
#include <string>
using namespace std;

void merge(vector<string>& orders, int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;

    vector<string> temp;

    while (i <= mid && j <= right)
    {
        if (orders[i] <= orders[j])
        {
            temp.push_back(orders[i]);
            i++;
        }
        else
        {
            temp.push_back(orders[j]);
            j++;
        }
    }

    while (i <= mid)
    {
        temp.push_back(orders[i]);
        i++;
    }

    while (j <= right)
    {
        temp.push_back(orders[j]);
        j++;
    }

    for (int k = 0; k < temp.size(); k++)
    {
        orders[left + k] = temp[k];
    }
}

void mergeSort(vector<string>& orders, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(orders, left, mid);
        mergeSort(orders, mid + 1, right);

        merge(orders, left, mid, right);
    }
}

int main()
{
    int n;

    cout << "Enter number of customer orders: ";
    cin >> n;

    vector<string> orders(n);

    cout << "Enter order timestamps (HH:MM):" << endl;

    for (int i = 0; i < n; i++)
    {
        cin >> orders[i];
    }

    cout << "\nOriginal order timestamps: ";
    for (int i = 0; i < n; i++)
    {
        cout << orders[i] << " ";
    }

    mergeSort(orders, 0, n - 1);

    cout << "\nSorted order timestamps: ";
    for (int i = 0; i < n; i++)
    {
        cout << orders[i] << " ";
    }

    cout << endl;

    return 0;
}