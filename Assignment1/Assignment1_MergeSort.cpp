#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <iomanip>
#include <cctype>

using namespace std;

struct Order {
    string orderId;
    string date;
    string originalRow;
    long long sortKey;
};

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string lowerString(string s) {
    for (char &ch : s)
        ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    return s;
}

vector<string> splitCSV(const string &line) {
    vector<string> fields;
    string field;
    bool quoted = false;

    for (size_t i = 0; i < line.size(); i++) {
        char ch = line[i];

        if (ch == '"') {
            if (quoted && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                i++;
            } else {
                quoted = !quoted;
            }
        } else if (ch == ',' && !quoted) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += ch;
        }
    }

    fields.push_back(trim(field));
    return fields;
}

bool parseDate(string s, long long &key) {
    s = trim(s);
    if (s.empty()) return false;

    string datePart = s;
    string timePart = "00:00:00";

    size_t space = s.find(' ');
    if (space != string::npos) {
        datePart = trim(s.substr(0, space));
        timePart = trim(s.substr(space + 1));

        if (timePart.size() == 5)
            timePart += ":00";
    }

    char separator = datePart.find('/') != string::npos ? '/' : '-';
    vector<string> parts;
    string part;
    stringstream ss(datePart);

    while (getline(ss, part, separator))
        parts.push_back(part);

    if (parts.size() != 3) return false;

    int year = 0, month = 0, day = 0;

    try {
        if (parts[0].size() == 4) {
            year = stoi(parts[0]);
            month = stoi(parts[1]);
            day = stoi(parts[2]);
        } else if (separator == '/') {
            month = stoi(parts[0]);
            day = stoi(parts[1]);
            year = stoi(parts[2]);
        } else {
            day = stoi(parts[0]);
            month = stoi(parts[1]);
            year = stoi(parts[2]);
        }
    } catch (...) {
        return false;
    }

    if (year < 100)
        year += (year < 70 ? 2000 : 1900);

    int hour = 0, minute = 0, second = 0;
    char colon;

    stringstream ts(timePart);
    if (!(ts >> hour >> colon >> minute))
        hour = minute = 0;
    else if (ts >> colon >> second) {
    }

    if (year < 1900 || month < 1 || month > 12 ||
        day < 1 || day > 31 || hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 || second < 0 || second > 59)
        return false;

    key = (((((long long)year * 100 + month) * 100 + day) * 100
             + hour) * 100 + minute) * 100 + second;

    return true;
}

void merge(vector<Order> &orders, int left, int mid, int right) {
    vector<Order> temp;
    int i = left, j = mid + 1;

    while (i <= mid && j <= right) {
        if (orders[i].sortKey <= orders[j].sortKey)
            temp.push_back(orders[i++]);
        else
            temp.push_back(orders[j++]);
    }

    while (i <= mid) temp.push_back(orders[i++]);
    while (j <= right) temp.push_back(orders[j++]);

    for (int k = 0; k < (int)temp.size(); k++)
        orders[left + k] = temp[k];
}

void mergeSort(vector<Order> &orders, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    mergeSort(orders, left, mid);
    mergeSort(orders, mid + 1, right);
    merge(orders, left, mid, right);
}

int main() {
    ifstream input("orders_raw.csv");

    if (!input.is_open()) {
        cout << "Error: Could not open orders_raw.csv\n";
        return 1;
    }

    string headerLine;
    if (!getline(input, headerLine)) {
        cout << "Error: Dataset is empty.\n";
        return 1;
    }

    vector<string> headers = splitCSV(headerLine);
    int idIndex = -1, dateIndex = -1;

    for (int i = 0; i < (int)headers.size(); i++) {
        string h = lowerString(trim(headers[i]));

        if (h == "order_id" || h == "orderid" || h == "id")
            idIndex = i;

        if (h == "order_date" || h == "ordered_at" ||
            h == "transactiondate" || h == "order date")
            dateIndex = i;
    }

    if (idIndex == -1 || dateIndex == -1) {
        cout << "Error: Could not find order ID and date columns.\n";
        cout << "Check the dataset's column names.\n";
        return 1;
    }

    vector<Order> orders;
    unordered_set<string> seenIds;
    string line;
    int invalidRows = 0, duplicateRows = 0;

    while (getline(input, line)) {
        if (trim(line).empty()) continue;

        vector<string> fields = splitCSV(line);

        if (idIndex >= (int)fields.size() ||
            dateIndex >= (int)fields.size()) {
            invalidRows++;
            continue;
        }

        string id = trim(fields[idIndex]);
        string date = trim(fields[dateIndex]);
        long long key;

        if (id.empty() || !parseDate(date, key)) {
            invalidRows++;
            continue;
        }

        if (!seenIds.insert(id).second) {
            duplicateRows++;
            continue;
        }

        orders.push_back({id, date, line, key});
    }

    input.close();

    ofstream cleaned("orders_cleaned.csv");
    ofstream sorted("orders_sorted.csv");

    if (!cleaned.is_open() || !sorted.is_open()) {
        cout << "Error: Could not create output files.\n";
        return 1;
    }

    cleaned << headerLine << '\n';
    for (const Order &order : orders)
        cleaned << order.originalRow << '\n';

    mergeSort(orders, 0, (int)orders.size() - 1);

    sorted << headerLine << '\n';
    for (const Order &order : orders)
        sorted << order.originalRow << '\n';

    cleaned.close();
    sorted.close();

    cout << "\nDataset Processing Completed\n";
    cout << "Valid unique rows sorted: " << orders.size() << '\n';
    cout << "Invalid rows skipped: " << invalidRows << '\n';
    cout << "Duplicate order IDs skipped: " << duplicateRows << '\n';
    cout << "Cleaned dataset: orders_cleaned.csv\n";
    cout << "Complete sorted dataset: orders_sorted.csv\n";

    cout << "\nAll sorted orders:\n";
    cout << left << setw(20) << "Order ID"
         << "Order Date\n";
    cout << string(45, '-') << '\n';

    for (const Order &order : orders)
        cout << left << setw(20) << order.orderId
             << order.date << '\n';

    return 0;
}
