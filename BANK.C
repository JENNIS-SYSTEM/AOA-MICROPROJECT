#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

// ---------------- TRANSACTION STRUCTURE ----------------

struct Transaction
{
    int tid;
    int cid;
    char date[11];
    char type;
    float amount;
};

struct Transaction t[MAX];
int n = 0;

// ---------------- DISPLAY ----------------

void display()
{
    int i;

    if (n == 0)
    {
        printf("\nNo transactions available.\n");
        return;
    }

    printf("\n-------------------------------------------------------------\n");
    printf("ID\tCustomer\tDate\t\tType\tAmount\n");
    printf("-------------------------------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        printf("%d\t%d\t\t%s\t%c\t%.2f\n",
               t[i].tid,
               t[i].cid,
               t[i].date,
               t[i].type,
               t[i].amount);
    }
}

// ---------------- ADD TRANSACTION ----------------

void addTransaction()
{
    int i;

    if (n >= MAX)
    {
        printf("\nTransaction limit reached.\n");
        return;
    }

    printf("\nEnter Transaction ID: ");
    scanf("%d", &t[n].tid);

    // Check duplicate Transaction ID
    for (i = 0; i < n; i++)
    {
        if (t[i].tid == t[n].tid)
        {
            printf("Transaction ID already exists!\n");
            return;
        }
    }

    printf("Enter Customer ID: ");
    scanf("%d", &t[n].cid);

    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%s", t[n].date);

    printf("Enter Type (C = Credit, D = Debit): ");
    scanf(" %c", &t[n].type);

    printf("Enter Amount: ");
    scanf("%f", &t[n].amount);

    n++;

    printf("\nTransaction added successfully!\n");
}

// ---------------- COMPARE FUNCTION ----------------

int compare(struct Transaction a, struct Transaction b, int field)
{
    if (field == 1)
        return a.tid - b.tid;

    if (field == 2)
        return a.cid - b.cid;

    if (field == 3)
        return strcmp(a.date, b.date);

    if (field == 4)
    {
        if (a.amount > b.amount)
            return 1;

        if (a.amount < b.amount)
            return -1;

        return 0;
    }

    return 0;
}

// ---------------- MERGE SORT ----------------

void merge(struct Transaction a[], int low, int mid, int high, int field)
{
    struct Transaction temp[MAX];

    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= high)
    {
        if (compare(a[i], a[j], field) <= 0)
        {
            temp[k] = a[i];
            i++;
        }
        else
        {
            temp[k] = a[j];
            j++;
        }

        k++;
    }

    while (i <= mid)
    {
        temp[k] = a[i];
        i++;
        k++;
    }

    while (j <= high)
    {
        temp[k] = a[j];
        j++;
        k++;
    }

    for (i = low; i <= high; i++)
    {
        a[i] = temp[i];
    }
}

void mergeSort(struct Transaction a[], int low, int high, int field)
{
    int mid;

    if (low < high)
    {
        mid = (low + high) / 2;

        // Divide
        mergeSort(a, low, mid, field);
        mergeSort(a, mid + 1, high, field);

        // Combine
        merge(a, low, mid, high, field);
    }
}

// ---------------- QUICK SORT ----------------

int partition(struct Transaction a[], int low, int high, int field)
{
    struct Transaction pivot;
    struct Transaction temp;

    int i;
    int j;

    pivot = a[high];

    i = low - 1;

    for (j = low; j < high; j++)
    {
        if (compare(a[j], pivot, field) <= 0)
        {
            i++;

            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    temp = a[i + 1];
    a[i + 1] = a[high];
    a[high] = temp;

    return i + 1;
}

void quickSort(struct Transaction a[], int low, int high, int field)
{
    int p;

    if (low < high)
    {
        p = partition(a, low, high, field);

        // Divide
        quickSort(a, low, p - 1, field);
        quickSort(a, p + 1, high, field);
    }
}

// ---------------- CHOOSE FIELD ----------------

int chooseField()
{
    int field;

    printf("\nSort/Search By:\n");
    printf("1. Transaction ID\n");
    printf("2. Customer ID\n");
    printf("3. Date\n");
    printf("4. Amount\n");

    printf("Enter choice: ");
    scanf("%d", &field);

    return field;
}

// ---------------- BINARY SEARCH ----------------

int binarySearch(struct Transaction a[],
                 int low,
                 int high,
                 int field,
                 struct Transaction key)
{
    int mid;
    int result;

    if (low > high)
        return -1;

    mid = (low + high) / 2;

    result = compare(a[mid], key, field);

    if (result == 0)
        return mid;

    if (result > 0)
        return binarySearch(a, low, mid - 1, field, key);

    return binarySearch(a, mid + 1, high, field, key);
}

// ---------------- SEARCH ----------------

void searchTransaction()
{
    int field;
    int position;

    struct Transaction key;

    if (n == 0)
    {
        printf("\nNo transactions available.\n");
        return;
    }

    field = chooseField();

    // Sort before binary search
    mergeSort(t, 0, n - 1, field);

    printf("\nRecords sorted for Binary Search.\n");

    if (field == 1)
    {
        printf("Enter Transaction ID: ");
        scanf("%d", &key.tid);
    }
    else if (field == 2)
    {
        printf("Enter Customer ID: ");
        scanf("%d", &key.cid);
    }
    else if (field == 3)
    {
        printf("Enter Date: ");
        scanf("%s", key.date);
    }
    else if (field == 4)
    {
        printf("Enter Amount: ");
        scanf("%f", &key.amount);
    }

    position = binarySearch(t, 0, n - 1, field, key);

    if (position == -1)
    {
        printf("\nTransaction not found.\n");
    }
    else
    {
        printf("\nTransaction Found!\n");

        printf("Transaction ID : %d\n", t[position].tid);
        printf("Customer ID    : %d\n", t[position].cid);
        printf("Date           : %s\n", t[position].date);
        printf("Type           : %c\n", t[position].type);
        printf("Amount         : %.2f\n", t[position].amount);
    }
}

// ---------------- HIGHEST / LOWEST ----------------

void findMinMax(int low, int high, int *min, int *max)
{
    int mid;

    int min1, max1;
    int min2, max2;

    if (low == high)
    {
        *min = low;
        *max = low;
        return;
    }

    if (high == low + 1)
    {
        if (t[low].amount < t[high].amount)
        {
            *min = low;
            *max = high;
        }
        else
        {
            *min = high;
            *max = low;
        }

        return;
    }

    mid = (low + high) / 2;

    // Divide
    findMinMax(low, mid, &min1, &max1);
    findMinMax(mid + 1, high, &min2, &max2);

    // Combine
    if (t[min1].amount < t[min2].amount)
        *min = min1;
    else
        *min = min2;

    if (t[max1].amount > t[max2].amount)
        *max = max1;
    else
        *max = max2;
}

void highestLowest()
{
    int min, max;

    if (n == 0)
    {
        printf("\nNo transactions available.\n");
        return;
    }

    findMinMax(0, n - 1, &min, &max);

    printf("\nHighest Transaction\n");
    printf("ID: %d\n", t[max].tid);
    printf("Amount: %.2f\n", t[max].amount);

    printf("\nLowest Transaction\n");
    printf("ID: %d\n", t[min].tid);
    printf("Amount: %.2f\n", t[min].amount);
}

// ---------------- CUSTOMER REPORT ----------------

void customerReport()
{
    int customerID;
    int i;
    int found = 0;

    float credit = 0;
    float debit = 0;

    printf("\nEnter Customer ID: ");
    scanf("%d", &customerID);

    for (i = 0; i < n; i++)
    {
        if (t[i].cid == customerID)
        {
            printf("\nTransaction ID: %d", t[i].tid);
            printf("\nDate: %s", t[i].date);
            printf("\nType: %c", t[i].type);
            printf("\nAmount: %.2f\n", t[i].amount);

            found++;

            if (t[i].type == 'C')
                credit = credit + t[i].amount;
            else
                debit = debit + t[i].amount;
        }
    }

    if (found == 0)
    {
        printf("\nNo transactions found.\n");
    }
    else
    {
        printf("\nTotal Transactions: %d\n", found);
        printf("Total Credit: %.2f\n", credit);
        printf("Total Debit: %.2f\n", debit);
        printf("Net Amount: %.2f\n", credit - debit);
    }
}

// ---------------- PERFORMANCE ANALYSIS ----------------

void performance()
{
    int sizes[] = {100, 500, 1000, 5000};

    int i;

    printf("\nPerformance Analysis\n");
    printf("----------------------------\n");

    for (i = 0; i < 4; i++)
    {
        printf("Number of records: %d\n", sizes[i]);

        printf("Merge Sort      : O(n log n)\n");
        printf("Quick Sort Avg  : O(n log n)\n");
        printf("Quick Sort Worst: O(n^2)\n");
        printf("Binary Search   : O(log n)\n\n");
    }
}

// ---------------- SORT MENU ----------------

void sortTransactions()
{
    int field;
    int choice;

    if (n == 0)
    {
        printf("\nNo transactions available.\n");
        return;
    }

    field = chooseField();

    printf("\nChoose Sorting Algorithm:\n");
    printf("1. Merge Sort\n");
    printf("2. Quick Sort\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        mergeSort(t, 0, n - 1, field);

        printf("\nTransactions sorted using Merge Sort.\n");
    }
    else if (choice == 2)
    {
        quickSort(t, 0, n - 1, field);

        printf("\nTransactions sorted using Quick Sort.\n");
    }
    else
    {
        printf("\nInvalid choice.\n");
    }

    display();
}

// ---------------- REPORT MENU ----------------

void reports()
{
    int choice;

    if (n == 0)
    {
        printf("\nNo transactions available.\n");
        return;
    }

    printf("\n========== REPORTS ==========\n");

    printf("1. Highest and Lowest Transaction\n");
    printf("2. Customer Report\n");
    printf("3. Performance Analysis\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
        highestLowest();

    else if (choice == 2)
        customerReport();

    else if (choice == 3)
        performance();

    else
        printf("\nInvalid choice.\n");
}

// ---------------- MAIN ----------------

int main()
{
    int choice;

    while (1)
    {
        printf("\n\n====================================\n");
        printf(" BANKING TRANSACTION ANALYSIS SYSTEM\n");
        printf("====================================\n");

        printf("1. Add Transaction\n");
        printf("2. Display Transactions\n");
        printf("3. Sort Transactions\n");
        printf("4. Search Transaction\n");
        printf("5. Reports\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addTransaction();
                break;

            case 2:
                display();
                break;

            case 3:
                sortTransactions();
                break;

            case 4:
                searchTransaction();
                break;

            case 5:
                reports();
                break;

            case 6:
                printf("\nThank you!\n");
                exit(0);

            default:
                printf("\nInvalid choice.\n");
        }
    }

    return 0;
}
