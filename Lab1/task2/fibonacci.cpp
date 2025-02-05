#include <iostream>
#include <sys/time.h>

using namespace std;

int main() {
    struct timeval start, end;
    

    int n, firstTerm = 1, secondTerm = 1, nextTerm;
    cout << "Enter number of terms: ";
    cin >> n;

    cout << "Fibonacci Series: " << firstTerm << " + " << secondTerm << " + ";
    //measure the execution time of the following loop

    gettimeofday(&start, NULL);


    for (int i = 1; i <= n-2; ++i) {
        nextTerm = firstTerm + secondTerm;
        cout << nextTerm << " + ";
        firstTerm = secondTerm;
        secondTerm = nextTerm;
    }

    gettimeofday(&end, NULL);
    //output the time in microseconds
    cout << "\nExecution time: " << (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec << " microseconds" << endl;

    return 0;
}
