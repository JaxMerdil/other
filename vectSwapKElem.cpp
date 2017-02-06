#include <iostream>
#include <vector>

//Перемещение части вектора от К-го элемента с конца

using namespace std;

vector<int> solution(vector<int> &A, int K)
{
    vector<int> B;

    if(A.size() < 2)
    {
        return A;
    }
    else if(A.size()==2)
    {
        std::swap(A[0],A[1]);
        return A;
    }
    else if(K > A.size())
    {
        K-=A.size();
    }
    for(size_t j = (A.size()-K); j < A.size(); ++j)
    {
        B.push_back(A[j]);
    }
    for(size_t i = 0; i < (A.size()-K); ++i)
    {
        B.push_back(A[i]);
    }
    return B;
}

template<typename T>
ostream& operator<< (ostream& os, const vector<T>& rhs)
{
    os << rhs.size() << ", " << rhs.capacity() << ": { ";
    for (const T& elem : rhs)
    {
        os << (&elem!=&rhs[0] ? ", " : "");
        os << elem;
    }
    os << " }";
    return os;
}

int main()
{
    vector<int> v = {1,3};
    cout << solution(v,2) << endl;
    return 0;
}
