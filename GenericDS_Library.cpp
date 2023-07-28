#include<iostream>
using namespace std;

template <class T>
struct NodeS    //singly
{
    T data;
    struct NodeS *next;
};


template <class T>
struct NodeD    //Doubly
{
    T data;
    struct NodeD *next;
    struct NodeD *prev;
};



template <class T>
class SinglyLL
{
    public:
        struct NodeS<T> *first;  
        int iCount;
    
    SinglyLL();

    void Display();
    int Count();

    void insertFirst(T);
    void Insertlast(T);
    void InsertAtPos(T, int);

    void DeleteFirst();
    void DeleteLast();
    void DeleteAtPos(int);
};

template <class T>
class SinglyCL
{
    public:
        struct NodeS<T> *first; 
        struct NodeS<T> *last; 
        int iCount; 

    SinglyCL();
    
    void Display();
    int Count();

    void insertFirst(T);
    void Insertlast(T);
    void InsertAtPos(T, int);

    void DeleteFirst();
    void DeleteLast();
    void DeleteAtPos(int);         
};

template <class T>
class DoublyLL
{
    public:
        struct NodeD<T> *first; 
        int iCount; 

    DoublyLL();
    
    void Display();
    int Count();

    void insertFirst(T);
    void Insertlast(T);
    void InsertAtPos(T, int);

    void DeleteFirst();
    void DeleteLast();
    void DeleteAtPos(int);       
};

template <class T>
class DoublyCL
{
    public:
        struct NodeD<T> *first; 
        struct NodeD<T> *last; 
        int iCount;     

    DoublyCL();
    
    void Display();
    int Count();

    void insertFirst(T);
    void Insertlast(T);
    void InsertAtPos(T, int);

    void DeleteFirst();
    void DeleteLast();
    void DeleteAtPos(int);   
};

//logic of 32 functions================>>>>>>>

///////////////////////////////////////////////////////////////////

template <class T>
SinglyLL<T>::SinglyLL()
{
    first = NULL;
    iCount = 0;
}

template <class T>
void SinglyLL<T>:: Display()
{
    cout<<"Elementrs of singly linear LL are :"<<"\n";

    NodeS<T> * temp = first;
    while(temp != NULL)
    {
        cout<<"|"<<temp->data<"|->";
        temp = temp -> next;
    }
    cout<<"NULL"<<"\n";
}

template <class T>
int SinglyLL<T>::Count()
{
    return iCount;
}

template <class T>
void SinglyLL<T>::insertFirst(T no)
{
    NodeS<T> *newn = new NodeS<T>;

    newn->data = no;
    newn->next = NULL;

    if(first == NULL)
    {
        first  = newn;
    }
    else
    {
        newn->next = first;
        first = newn;
    }
    iCount++;
}

template <class T>
void SinglyLL<T>::Insertlast(T no)
{
        NodeS<T> *newn = new NodeS<T>;

    newn->data = no;
    newn->next = NULL;

    if(first == NULL)
    {
        first  = newn;
    }
    else
    {
        NodeS<T> temp = first;

        while(temp->next != NULL)
        {
            temp = temp-> next;

        }
        temp->next = first;

    }
    iCount++;
}

template <class T>
void SinglyLL<T>::InsertAtPos(T no, int iPos)
{
    
}

template <class T>
void SinglyLL<T>::DeleteFirst()
{}

template <class T>
void SinglyLL<T>::DeleteLast()
{}

template <class T>
void SinglyLL<T>::DeleteAtPos(int iPos)
{}


/////////////////////////////////////////////////////////////

template <class T>
SinglyCL<T>::SinglyCL()
{}

template <class T>
void SinglyCL<T>:: Display()
{}

template <class T>
int SinglyCL<T>::Count()
{
    return 0;
}

template <class T>
void SinglyCL<T>::insertFirst(T no)
{}

template <class T>
void SinglyCL<T>::Insertlast(T no)
{}

template <class T>
void SinglyCL<T>::InsertAtPos(T no, int iPos)
{}

template <class T>
void SinglyCL<T>::DeleteFirst()
{}

template <class T>
void SinglyCL<T>::DeleteLast()
{}

template <class T>
void SinglyCL<T>::DeleteAtPos(int iPos)
{}
//////////////////////////////////////////////////////////////

template <class T>
DoublyLL<T>::DoublyLL()
{}

template <class T>
void DoublyLL<T>:: Display()
{}

template <class T>
int DoublyLL<T>::Count()
{
    return 0;
}

template <class T>
void DoublyLL<T>::insertFirst(T no)
{}

template <class T>
void DoublyLL<T>::Insertlast(T no)
{}

template <class T>
void DoublyLL<T>::InsertAtPos(T no, int iPos)
{}

template <class T>
void DoublyLL<T>::DeleteFirst()
{}

template <class T>
void DoublyLL<T>::DeleteLast()
{}

template <class T>
void DoublyLL<T>::DeleteAtPos(int iPos)
{}
//////////////////////////////////////////////////////////////////////

template <class T>
DoublyCL<T>::DoublyCL()
{}

template <class T>
void DoublyCL<T>:: Display()
{}

template <class T>
int DoublyCL<T>::Count()
{
    return 0;
}

template <class T>
void DoublyCL<T>::insertFirst(T no)
{}

template <class T>
void DoublyCL<T>::Insertlast(T no)
{}

template <class T>
void DoublyCL<T>::InsertAtPos(T no, int iPos)
{}

template <class T>
void DoublyCL<T>::DeleteFirst()
{}

template <class T>
void DoublyCL<T>::DeleteLast()
{}

template <class T>
void DoublyCL<T>::DeleteAtPos(int iPos)
{}

int main()
{
    SinglyLL <int>obj1;

    obj1.insertFirst(51);
    obj1.insertFirst(21);
    obj1.insertFirst(11);

    obj1.Insertlast(101);
    obj1.Insertlast(111);

    obj1.Display();
    cout<<"Number of elements are:"<<obj1.Count()<<"\n";

    SinglyLL <char>obj2;

    obj2.insertFirst('C');
    obj2.insertFirst('B');
    obj2.insertFirst('A');

    obj2.Insertlast('D');
    obj2.Insertlast('E');

    obj2.Display();
    cout<<"Number of elements are:"<<obj1.Count()<<"\n";
    

    return 0;
}