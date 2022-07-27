#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

void test_is_digit()
{
    char s[] = "1357924680, a b c 123";

    for(char& c: s)
    {
        if(isdigit(c))
            cout<<c<<" "<<"is digit"<<endl;
        else
            cout<<c<<" "<<"not digit"<<endl;
    }
}

void test_foreach_in_vector()
{
    vector<string> str;
    str.push_back("123");
    str.push_back("abc");
    str.push_back("456");
    str.push_back("def");
    str.push_back("789");
    str.push_back("ghi");
    for(string s: str)
    {
        cout<<s<<endl;
    }
}

void test_vector_row_column()
{
    vector< vector<int> > table;
    vector<int> row;
    for(int i=0; i<10; i++)
    {
        row.clear();
        for(int j=0;j<5; j++)
        {
            row.push_back(j);
        }
        table.push_back(row);
    }
    /*
    vector< vector<int> >::iterator iter;
    for (iter=table.begin(); iter!=table.end(); ++iter)
    {
        for (int i=0; i<(*iter).size(); i++)
            cout<<(*iter).size()<<endl;
            //cout<<(*iter).size()<<"\t"<<(*iter)[i]<<" ";
        cout<<endl;
    }*/
    for(vector<int> v: table)
    {
        for(int i: v)
            cout<<i<<" ";
        cout<<endl;
    }
}

int main(void)
{
    // test_is_digit();
    // test_foreach_in_vector();
    test_vector_row_column();

    return 0;
}
