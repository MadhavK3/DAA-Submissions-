#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Course {
    string name;
    int students;
    int color;
    int room;
    vector<int> adj;
};

struct Room {
    string name;
    int capacity;
};

int main() {
    int n, s, r;

    cout << "===== University Timetable Scheduling =====\n";
    cout << "Enter number of courses: ";
    cin >> n;

    if (n <= 0) {
        cout << "Number of courses must be positive.\n";
        return 0;
    }

    vector<Course> courses(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter name of course " << i + 1 << ": ";
        cin >> courses[i].name;

        cout << "Enter number of students in " << courses[i].name << ": ";
        cin >> courses[i].students;

        if (courses[i].students < 0) {
            cout << "Invalid number of students.\n";
            return 0;
        }

        courses[i].color = -1;
        courses[i].room = -1;
    }

    cout << "\nEnter number of students with enrolment data: ";
    cin >> s;

    if (s < 0) {
        cout << "Invalid number of students.\n";
        return 0;
    }

    for (int i = 0; i < s; i++) {
        int count;
        cout << "\nEnter number of courses enrolled by student "
             << i + 1 << ": ";
        cin >> count;

        vector<int> enrolled;

        for (int j = 0; j < count; j++) {
            string name;
            cout << "Enter course name: ";
            cin >> name;

            int index = -1;

            for (int k = 0; k < n; k++) {
                if (courses[k].name == name) {
                    index = k;
                    break;
                }
            }

            if (index == -1) {
                cout << "Invalid course name.\n";
                return 0;
            }

            enrolled.push_back(index);
        }

        sort(enrolled.begin(), enrolled.end());
        enrolled.erase(
            unique(enrolled.begin(), enrolled.end()),
            enrolled.end()
        );

        for (int a = 0; a < (int)enrolled.size(); a++) {
            for (int b = a + 1; b < (int)enrolled.size(); b++) {
                int u = enrolled[a];
                int v = enrolled[b];

                courses[u].adj.push_back(v);
                courses[v].adj.push_back(u);
            }
        }
    }

    for (auto &course : courses) {
        sort(course.adj.begin(), course.adj.end());
        course.adj.erase(
            unique(course.adj.begin(), course.adj.end()),
            course.adj.end()
        );
    }

    vector<int> order(n);
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }

    sort(order.begin(), order.end(), [&](int a, int b) {
        if (courses[a].adj.size() != courses[b].adj.size())
            return courses[a].adj.size() > courses[b].adj.size();
        return courses[a].name < courses[b].name;
    });

    int totalSlots = 0;

    for (int u : order) {
        vector<bool> used(n, false);

        for (int v : courses[u].adj) {
            if (courses[v].color != -1) {
                used[courses[v].color] = true;
            }
        }

        int color = 0;
        while (color < n && used[color]) {
            color++;
        }

        courses[u].color = color;
        totalSlots = max(totalSlots, color + 1);
    }

    cout << "\nEnter number of available rooms: ";
    cin >> r;

    if (r <= 0) {
        cout << "At least one room is required.\n";
        return 0;
    }

    vector<Room> rooms(r);

    for (int i = 0; i < r; i++) {
        cout << "Enter room name: ";
        cin >> rooms[i].name;

        cout << "Enter capacity of " << rooms[i].name << ": ";
        cin >> rooms[i].capacity;

        if (rooms[i].capacity < 0) {
            cout << "Invalid room capacity.\n";
            return 0;
        }
    }

    vector<int> roomOrder(r);
    for (int i = 0; i < r; i++) {
        roomOrder[i] = i;
    }

    sort(roomOrder.begin(), roomOrder.end(), [&](int a, int b) {
        return rooms[a].capacity < rooms[b].capacity;
    });

    vector<vector<bool>> occupied(
        totalSlots, vector<bool>(r, false)
    );

    bool allocationFailed = false;

    for (int slot = 0; slot < totalSlots; slot++) {
        vector<int> exams;

        for (int i = 0; i < n; i++) {
            if (courses[i].color == slot) {
                exams.push_back(i);
            }
        }

        sort(exams.begin(), exams.end(), [&](int a, int b) {
            return courses[a].students > courses[b].students;
        });

        for (int exam : exams) {
            bool assigned = false;

            for (int roomIndex : roomOrder) {
                if (!occupied[slot][roomIndex] &&
                    rooms[roomIndex].capacity >= courses[exam].students) {
                    courses[exam].room = roomIndex;
                    occupied[slot][roomIndex] = true;
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                allocationFailed = true;
            }
        }
    }

    cout << "\n===== University Exam Timetable =====\n";
    cout << left << setw(15) << "Course"
         << setw(15) << "Time Slot"
         << setw(15) << "Students"
         << setw(15) << "Room" << '\n';

    for (int i = 0; i < n; i++) {
        cout << left << setw(15) << courses[i].name
             << setw(15) << ("Slot " + to_string(courses[i].color + 1))
             << setw(15) << courses[i].students;

        if (courses[i].room != -1) {
            cout << setw(15) << rooms[courses[i].room].name;
        } else {
            cout << setw(15) << "No room";
        }

        cout << '\n';
    }

    cout << "\nTotal exam slots required: " << totalSlots << '\n';

    if (allocationFailed) {
        cout << "Warning: Some exams could not be allocated a suitable room.\n";
    } else {
        cout << "Room allocation successful for all exams.\n";
    }

    cout << "\nAlgorithm: Welsh-Powell Graph Colouring\n";
    cout << "Graph colouring time complexity: O(V^2 + E)\n";

    return 0;
}
