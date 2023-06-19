#include <iostream>




int bin_search(int ar[], int n, int key)
{
    int low = 0, high = n -1, mid;
    while(low <= high)
    {
        mid = (low + high)/2;
        if(ar[mid] == key)
        {
            return mid;
        }

        if(key > ar[mid])
        {
            low = mid + 1;
        }
        else
        {
            high = mid -1;
        }
    }

    return -1;

}

int main(int argc, char const *argv[])
{
    int ar[10];
    for(int i = 0; i < 10; i++)
    {
        ar[i] = 100 + i * 1000;
    }

    std::cout <<"find the value 100 at index :"<< bin_search(ar, 10, 1100) << std::endl;
    return 0;
}
