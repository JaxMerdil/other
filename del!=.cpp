#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

//Равновесие. Вычитание ненужного значения для равновесия

using namespace std;

int solution(vector<int> &A)
{
    int size=A.size();
    int dif=0;
    int index=-1;
    int size_p;

    if(!(size%2==0))
    {
        A.push_back(0);
        size_p=A.size()/2;
    }
    else
    {
        size_p=A.size()/2;
    }
    int sum1 = std::accumulate(A.begin(), A.begin()+size_p , 0);
    int sum2 = std::accumulate(A.rbegin(), A.rbegin()+size_p , 0);
    if(sum1!=sum2)
    {
        if(sum1>sum2)
        {
            dif=sum1-sum2;
            vector<int>::iterator it=find(A.begin(),  A.begin()+size_p , dif);
            index=it-A.begin();
            return index;
        }
        else
        {
            dif=sum2-sum1;
            vector<int>::iterator it=find(A.begin(),  A.begin()+size_p , dif);
            index=it-A.begin();
            return index;
        }
    }
    else if (sum1==sum2)
    {
        return index;
    }
}

int main()
{
    vector<int> A={1082132608, 0, 1082132608};
    cout<< solution(A);
    return 0;
}
