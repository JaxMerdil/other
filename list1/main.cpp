#include <iostream>
using namespace std;

template <class T>
struct Node
{
    Node* next;
    T data;
};

template <class T>
class LinkedList{
public:
    LinkedList():head(0), tail(0){}
    ~LinkedList()
    {
        Node<T> * tmp;
        while(head)
        {
            tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void push_back(T val)
    {
        Node<T> * newNode = new Node<T>;

        newNode->data = val;
        newNode->next = 0;

        if(tail==0)
        {
            head = tail = newNode;
            return;
        }

        tail->next = newNode;
        tail = newNode;
    }
    void push_front(T val)
    {
        Node<T> * newNode = new Node<T>;

        newNode->data = val;
        newNode->next = head;

        head = newNode;

        if(tail==0)
            tail = head;
    }
    void reverse()
    {
        if (head == 0)
            return;

        tail = head;

        Node<T> * current = head->next;
        Node<T> * right;
        tail->next = 0;


        while(current)
        {
            right = current->next;

            current->next = head;
            head = current;
            current = right;
        }

//        tail->next = 0;
    }
    void display() const
    {
        Node<T> * current = head;

        while(current)
        {
            cout<<current->data<<' ';
            current = current->next;
        }
    }
private:
    Node<T> * head;
    Node<T> * tail;
};


int main()
{
    LinkedList<int> intList;

    intList.push_front(6);
    intList.push_front(2);
    intList.push_front(1);

    intList.push_back(4);
    intList.push_back(5);
    intList.push_back(6);

    cout << "Normal: ";
    intList.display();

    intList.reverse();

    cout << "\nReversed: ";
    intList.display();

    cout << endl;
    return 0;
}
