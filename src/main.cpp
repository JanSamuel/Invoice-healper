#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;
int main();

void help()
{
    cout<<"\n\nWhat this program do?\nIt corrects invoices list recalculate and save to file with name.corrected.txt also if file need to be processed again it is saved as name.not_processed.txt\nHow to run a file?\nYou need to put this program in the same folder as file you need to process,\nthen you past press enter\nREC number later called lines must be separated by comas ',' when finish close input be typing double coma ',,'\nIf You don't want any lines to be operated on you need to put comma','\neg."<<endl;
    cout<<"Lines that need to be processed by client in ****** :"<<endl;
    cout<<"put numbers of lines separated by coma ','"<<endl;
    cout<<","<<endl;
    cout<<"Lines to delete :"<<endl;
    cout<<"put numbers of lines separated by coma ','"<<endl;
    cout<<"2,5,14,,"<<endl;
    cout<<"in this case invoices in lines 2 5 14 will be deleted"<<endl;
    cout<<"file .not_processed.txt will not be created"<<endl;
    cout<<"How to enable copy/past option?\nyou need to right click on panel above program with 'C:/User/...' choose option and enable fast editing\nNow you can past thing with right mouse click"<<endl;

    cout<<"\n\n"<<endl;

}


void exit()
{
    cout<<"To terminate press any key"<<endl;
    cin.get();
    cin.ignore();
    exit(0);
}

vector<int> reader()//reads input in to vector
{
    vector<int>vect;
    cout<<"put numbers of lines separated by coma ','"<<endl;
    int i=0;

    while (cin)
    {
        char x;
        cin>>i>>x;
        if(i<0)
        {
            cout<<"error negative line"<<endl;
        }
        else if(i==0)
        {

        }
        else
            vect.push_back(i);
    }
    cin.clear();
    cin.ignore();
    return vect;
}

string check_header(string x)//check if file is invoice list and returns header of file
{
    istringstream str {x};
    char a1,a2,a3;
    str>>a1>>a2>>a3;
    string header_1,header_2,header_3,header_4;
    if(a1!=','||a2!='"'||a3!='C')
    {
        cout<<"Error report:Detected wrong header of file, program terminated"<<endl;
        cout<<"To terminate press any key"<<endl;
        exit();
    }
    else
    {
        header_1 = x.substr(0, x.find(",", 0));
        x = x.substr(x.find(",", 0)+1, x.length());
        header_2 = x.substr(0, x.find(",", 0));
        x = x.substr(x.find(",", 0)+1, x.length());
        header_3 = x.substr(0, x.find(",", 0));
        x = x.substr(x.find(",", 0)+1, x.length());
        header_4 = x.substr(0, x.find(",", 0));
        x = x.substr(x.find(",", 0)+1, x.length());

        return header_1+","+header_2+","+header_3+","+header_4;
    }
}
bool check_H(string x)//checks if line is beginning of invoice
{
    string H="H,";
    if(!(x.compare(0, H.length(), H)==0))
    {
        return 0;
    }
    else
        return 1;
}
void check_H_correct(string x)//check if line starts with H, if not terminates aplication
{
    if(!check_H(x))
    {
        exit();
    }
}
bool chec_isA(string s)//checks if first char of string is letter
{
    char check;
    istringstream str {s};
    str>>check;
    if(isalpha(check))
    {
        return 1;
    }
    return 0;
}
string find_inv(string str) //looks for invoice number
{
    string inv;
    str = str.substr(str.find("INV", 0)+3, str.length()); // invoice number starts with INV and country latter ex. INVUK
    if(chec_isA(str))
    {
        inv=str.substr(0, str.find(",", 0));
        return "INV"+inv+"\n";
    }
    else
        return find_inv(str);
}
vector<int> sort_v(vector <int> vec)//sorts and check for duplicates
{
    if(vec.size()>0)
    {
        std::sort (vec.begin(), vec.end());
        for(int i=0; i<(vec.size()-1); i++)
        {
            if(vec[i]!=0&&vec[i]==vec[i+1])
            {
                cout<<"Error report:Line "<<vec[i]<<" duplicated"<<endl;
                vec.erase(vec.begin()+i);
            }
        }
    }
    return vec;
}
double StringToNumber (string text )
{
    stringstream ss(text);
    double result;
    return ss >> result?result:0;
}
double value(string text)
{
    string value;
    for(int i=0; i<15; ++i)
    {
        text = text.substr(text.find(",", 0)+1, text.length());
    }
    value= text.substr(0, text.find(",", 0));
    return StringToNumber(value.c_str());
}
template <typename T>
string NumberToString ( T Number )//return string with double inside
{
    stringstream ss;
    ss << fixed << setprecision(2) << Number;
    return ss.str();
}
vector<string> load(string name)//loads file into vector
{
    fstream ist;
    string file_name=name+".txt";
    ist.open(file_name.c_str(),ios_base::in);
    if(ist.good())
    {
        string line;
        vector <string> tmp;
        while(getline(ist,line))
        {
            tmp.push_back(line);
        }
        return tmp;
    }
    else
    {
        cout<<"File error"<<endl;
        exit();
    }
}
void delete_lines(vector<int> n_lines, vector<string>const &tmp, string name, string header)//delete lines indicated by user
{
    for(int i=0; i<n_lines.size(); ++i) //if lines inputed by user aren't beginning of invoice terminates application
    {
        check_H_correct(tmp[(n_lines[i]-1)]);
    }
    fstream ost;
    string file_name_o=name+".corrected.txt";
    ost.open(file_name_o.c_str(),ios_base::out);
    int num_inv=0;
    double sum=0;
    vector<string>new_file;
    bool add_line=0;
    for(int i=0; i<tmp.size(); ++i)//lines between beginning of indicated invoice and beginning of next invoice are not copied
    {
        if(check_H(tmp[i]))
        {
            add_line=1;
            for(int j=0; j<n_lines.size(); ++j)
            {
                if(i==(n_lines[j]-1))
                {
                    add_line=0;
                }
            }
        }
        if(add_line==1)
        {
            new_file.push_back(tmp[i]);
            if(check_H(tmp[i]))//recalculate number of invoices and their sum for new header
            {
                num_inv++;
                sum+=value(tmp[i]);
            }
        }
    }
    header+=","+NumberToString(num_inv)+","+NumberToString(sum)+"\n";
    ost<<header;
    for(int i=0; i<new_file.size(); ++i)
    {
        ost<<new_file[i]<<"\n";
    }
}
void not_processed(vector<int> n_lines, vector<string>const &tmp, string name)//copies number of invoice to be check and recalculated by hand
{
    fstream ost;
    string file_name_o=name+".not_processed.txt";
    ost.open(file_name_o.c_str(),ios_base::out);
    for(int i=0; i<n_lines.size(); ++i)
    {
        string str;
        check_H_correct(tmp[(n_lines[i]-1)]);
        str=find_inv(tmp[(n_lines[i]-1)]);
        ost<<str;
    }
}



int main()
{
    cout<<"Invoice Helper 2018 licensed under EPL license"<<endl;
    string answer;

    cout<<"Hello\nWelcome to Invoice Helper\nIf it is you're first time using I encourage you to use help by writing 'help'"<<endl;
    do
    {
        cout<<"If you want open file write/past it name without .txt at end :"<<endl;
        string name, header;
        cin>>name;
        if(name=="help")
        {
            help();
            cout<<"If you want open file write/past it name without .txt at end :"<<endl;
            cin>>name;
        }
        vector<int>num_lines;
        vector<int>num_lines_2;
        vector <string>file;
        file=load(name);
        header=check_header(file[0]);
        cout<<"Lines that need to be processed by client in ***** :"<<endl;
        num_lines=reader();
        if(num_lines.size()>0)
        {
            not_processed(num_lines,file,name);
        }
        cout<<"Lines to delete :"<<endl;
        num_lines_2=reader();
        num_lines.insert(num_lines.end(), num_lines_2.begin(), num_lines_2.end());
        num_lines=sort_v(num_lines);
        delete_lines(num_lines,file,name,header);
        cout<<"Process completed"<<endl;
        cout<<"Do yo want to continue with next file?"<<endl;
        cout<<"if yes press y, if no press any another key"<<endl;
        cin>>answer;
    }
    while(answer=="y"||answer=="Y");
    exit();
    return 0;
}
