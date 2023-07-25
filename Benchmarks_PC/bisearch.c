#include <stdio.h>
int binarySearch(int arr[], int l, int r, int x)
{
    if (r >= l)
    {
        int mid = l + (r - l)/2;
        if (arr[mid] == x) {printf("hit1\n");return mid;}
        if (arr[mid] > x) {printf("hit2\n");return binarySearch(arr, l, mid-1, x);}
        printf("hit3\n");
        return binarySearch(arr, mid+1, r, x);
    }
    return -1;
}

int main(){
    int arr[] = {3,7,9,15,23,41,55,99,156};
    int n = sizeof(arr)/ sizeof(arr[0]);
    int x = 9;
    for(int i=0;i<1;i++){
        int result = binarySearch(arr, 0, n-1, x); //total 15 times in measurement
    }
    return 0;
}