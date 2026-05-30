#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>

using namespace std;

long long comparisons = 0;
long long swapsCount = 0;

void printFormattedTime(long long microseconds)
{
    long long total_ms = microseconds / 1000;
    long long seconds = total_ms / 1000;
    long long ms = total_ms % 1000;

    cout << (seconds < 10 ? "0" : "") << seconds << " s "
        << (ms < 100 ? (ms < 10 ? "00" : "0") : "") << ms << " ms";
}

void skaitymas(const string& filename, int*& arr, int& n)
{
    ifstream f(filename);
    f >> n;
    arr = new int[n];

    for (int i = 0; i < n; i++)
        f >> arr[i];

    f.close();
}

void insertionSort(int* arr, int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0)
        {
            comparisons++;

            if (arr[j] > key)
            {
                arr[j + 1] = arr[j];
                swapsCount++;
                j--;
            }
            else break;
        }

        arr[j + 1] = key;
        swapsCount++;
    }
}

void merge(int* arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        comparisons++;

        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            swapsCount++;
            i++;
        }
        else
        {
            arr[k] = R[j];
            swapsCount++;
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        swapsCount++;
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        swapsCount++;
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSort(int* arr, int left, int right)
{
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}

long long measureInsertion(int* arr, int n)
{
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    insertionSort(arr, n);
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

long long measureMerge(int* arr, int n)
{
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    mergeSort(arr, 0, n - 1);
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

void runTest(const string& name, int* data, int n, bool isInsertion)
{
    long long totalTime = 0;
    long long lastComparisons = 0;
    long long lastSwaps = 0;

    for (int i = 0; i < 5; i++)
    {
        int* arr = new int[n];
        for (int j = 0; j < n; j++) arr[j] = data[j];

        comparisons = 0;
        swapsCount = 0;

        long long t = isInsertion ? measureInsertion(arr, n)
            : measureMerge(arr, n);

        totalTime += t;
        lastComparisons = comparisons;
        lastSwaps = swapsCount;

        delete[] arr;
    }

    cout << "--------------------------------------------\n";
    cout << name << endl;

    cout << "Vidutinis laikas: ";
    printFormattedTime(totalTime / 5);
    cout << endl;

    cout << "Palyginimai: " << lastComparisons << endl;
    cout << "Sukeitimai: " << lastSwaps << endl;
}

int main()
{
    string files[3] = {
        "5000_elementai.txt",
        "10000_elementai.txt",
        "50000_elementai.txt"
    };

    for (string filename : files)
    {
        int* original = nullptr;
        int n = 0;

        skaitymas(filename, original, n);

        int* unsorted = new int[n];
        int* sorted = new int[n];
        int* reversed = new int[n];

        for (int i = 0; i < n; i++)
            unsorted[i] = original[i];

        for (int i = 0; i < n; i++)
            sorted[i] = original[i];
        sort(sorted, sorted + n);

        for (int i = 0; i < n; i++)
            reversed[i] = sorted[n - 1 - i];

        cout << "\n===== FAILAS: " << filename << " =====\n";

        runTest("Insertion Sort | Nesurikiuoti", unsorted, n, true);
        runTest("Insertion Sort | Surikiuoti", sorted, n, true);
        runTest("Insertion Sort | Atvirksciai surikiuoti", reversed, n, true);

        runTest("Merge Sort | Nesurikiuoti", unsorted, n, false);
        runTest("Merge Sort | Surikiuoti", sorted, n, false);
        runTest("Merge Sort | Atvirksciai surikiuoti", reversed, n, false);

        delete[] original;
        delete[] unsorted;
        delete[] sorted;
        delete[] reversed;
    }

    return 0;
}
 