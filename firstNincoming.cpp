#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int solution (vector<int>& A)
{
    unordered_map<int, bool> B;
    for(size_t i=0; i!=A.size();i++)
    {
        if(B[A[i]] == false)
            B[A[i]] = true;
        else B[A[i]] = false;
    }
    for(auto it=B.begin(); it!=B.end(); ++it)
    {
             if(it->second==true)
                 return it -> first;
    }
    return 0;
}

int main()
{
    vector<int> V={1,2,3,4,4,3,2,1,8};
    cout << solution(V) << endl;
    return 0;
}
