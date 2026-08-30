#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Movie
{
    string name;
    float imdbRating;
    int releaseYear;
    int watchTime;
};

// Swap two movies
void swapMovies(Movie &a, Movie &b)
{
    Movie temp = a;
    a = b;
    b = temp;
}

// Partition function
int partition(Movie movies[], int low, int high, int choice)
{
    Movie pivot = movies[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        bool condition = false;

        if (choice == 1)
            condition = movies[j].imdbRating <= pivot.imdbRating;
        else if (choice == 2)
            condition = movies[j].releaseYear <= pivot.releaseYear;
        else if (choice == 3)
            condition = movies[j].watchTime <= pivot.watchTime;

        if (condition)
        {
            i++;
            swapMovies(movies[i], movies[j]);
        }
    }

    swapMovies(movies[i + 1], movies[high]);

    return i + 1;
}

// Quick Sort function
void quickSort(Movie movies[], int low, int high, int choice)
{
    if (low < high)
    {
        int pivotIndex = partition(movies, low, high, choice);

        quickSort(movies, low, pivotIndex - 1, choice);
        quickSort(movies, pivotIndex + 1, high, choice);
    }
}

// Generate movie dataset
void generateMovies(Movie movies[], int n)
{
    for (int i = 0; i < n; i++)
    {
        movies[i].name = "Movie" + to_string(i + 1);

        // Generate IMDB rating between 5.0 and 9.9
        movies[i].imdbRating = 5.0 + (rand() % 50) / 10.0;

        // Generate release year between 2000 and 2025
        movies[i].releaseYear = 2000 + rand() % 26;

        // Generate watch time between 80 and 200 minutes
        movies[i].watchTime = 80 + rand() % 121;
    }
}

// Display movie dataset
void displayMovies(Movie movies[], int n)
{
    cout << "\n";
    cout << "------------------------------------------------------------\n";
    cout << "Movie\t\tIMDB Rating\tYear\tWatch Time\n";
    cout << "------------------------------------------------------------\n";

    for (int i = 0; i < n; i++)
    {
        cout << movies[i].name << "\t\t"
             << movies[i].imdbRating << "\t\t"
             << movies[i].releaseYear << "\t"
             << movies[i].watchTime << " mins\n";
    }

    cout << "------------------------------------------------------------\n";
}

int main()
{
    srand(time(0));

    int n;

    cout << "============================================\n";
    cout << "     STREAMFLIX MOVIE RECOMMENDATION\n";
    cout << "          QUICK SORT SYSTEM\n";
    cout << "============================================\n";

    cout << "\nEnter number of movies to generate: ";
    cin >> n;

    if (n <= 0)
    {
        cout << "Invalid number of movies." << endl;
        return 0;
    }

    Movie *movies = new Movie[n];

    // Generate dataset
    generateMovies(movies, n);

    cout << "\nGenerated Movie Dataset:";
    displayMovies(movies, n);

    // Ask user for sorting parameter
    int choice;

    cout << "\nSelect sorting parameter:\n";
    cout << "1. IMDB Rating\n";
    cout << "2. Release Year\n";
    cout << "3. Watch Time Popularity\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice < 1 || choice > 3)
    {
        cout << "\nInvalid choice." << endl;
        delete[] movies;
        return 0;
    }

    // Apply Quick Sort
    quickSort(movies, 0, n - 1, choice);

    cout << "\n============================================\n";

    if (choice == 1)
        cout << "Movies Sorted by IMDB Rating";
    else if (choice == 2)
        cout << "Movies Sorted by Release Year";
    else
        cout << "Movies Sorted by Watch Time Popularity";

    cout << "\n============================================";

    // Display sorted dataset
    cout << "\n\nSorted Movie Dataset:";
    displayMovies(movies, n);

    delete[] movies;

    return 0;
}