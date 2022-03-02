#include<iostream>          // input output operations
#include<iomanip>           // output manipulators on console
#include<fstream>           // to perform file handling operations
#include<stdlib.h>          // to use CLS screen function to clear screen

#include<vector>  
#include<sstream>
#include<ctime>
#include <unistd.h>       //loding animation before printing bill

using namespace std;
 
class User
{  
    //  User class which cannot be accessed from outside while declaring its objects
    private:
        ofstream write;              
        ifstream read;
        string manager,cashier,customer;
        int order_no;
        float gst;
        string manager_pass,cashier_pass;
        vector<pair <string,int> > itm_q;           // itm_s for item and it's quantity
        vector<pair <int,float> > pr_t;              // price and total
        float Grand_total;
    
    protected:
        void initially()
        {
            Grand_total=0;
            gst=0;
        }
        void set_id_pass()              // the default id and password for owner manager and cashier
        {        
            read.open("password.txt");    
            read>>manager;
            read>>manager_pass;
            read>>cashier;
            read>>cashier_pass;
            read.close();
        } 
        void set_manager(string manager,string manager_pass)    // setter for setting the values for Manager
        {
            write.open("password.txt");
            this->manager=manager;
            write<<manager<<" ";
            this->manager_pass=manager_pass;
            write<<manager_pass<<endl;
            write<<this->cashier<<" ";
            write<<this->cashier_pass;
            write.close();
        }
        void set_cashier(string cashier,string cashier_pass)    // setter for setting the values for Cashier
        {
            write.open("password.txt");
            this->cashier=cashier;
            write<<this->manager<<" ";
            write<<this->manager_pass<<endl;
            write<<cashier<<" ";
            this->cashier_pass=cashier_pass;
            write<<cashier_pass;
            write.close();
        }
        void display()                                          // to display the details of Manager & Cashier
        {
            cout<<"Details of Manager\n"<<manager<<"  "<<manager_pass<<endl;
            cout<<"Details of Cashier\n"<<cashier<<"  "<<cashier_pass<<endl;
        }
        bool check_manager(string id,string pass)       // to check if the user already exist and if you want to overwrite the user
        {
            if(id==manager && pass==manager_pass)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        int check_pass(string id,string pass)       // check if the user already exist and if you want to overwrite the user
        {
            if(id==manager && pass==manager_pass )
            {
                return 1;
            }
            else if(id==cashier && pass==cashier_pass)
            {
                return 2;
            }
            else
            {
                return 0;
            }
        }

    public :
        void items(string search)
        {
            string item,item1;
            int stock,stock1,price,price1,n,num,count=0;
            
            read.open("items.txt");
            while(read>>item)
            {
                read>>stock;
                read>>price;
    
                if(item==search)
                {   
                    count=1;
                    do
                    {
                        system("CLS");
                        cout<<"Item : "<<item<<endl;                
                        cout<<"Enter"<<endl; 
                        cout<<"1- Alter the stock."<<endl;
                        cout<<"2- Alter price of item."<<endl;
                        cout<<"3- Exit."<<endl;
                        cout<<"Please, Enter your choice : ";
                        cin>>n;
                        if(n==1)
                        {                            
                            cout<<"Enter number of recently arrived items : ";
                            cin>>num;
                            stock+=num;
                            system("CLS");
                        }
                        else if(n==2)
                        {
                            cout<<"Enter new price of item : ";
                            cin>>price;
                            system("CLS");
                        }
                        else if(n==3)
                        {
                            break;
                        }
                        else
                        {
                            cout<<"Wrong choice"<<endl;                            
                        }
                    }while(n!=3);
                    break;
                }  
            }
            read.close();
            read.open("items.txt");                             
                
            write.open("new.txt");                              // to write the updated items into file                          
            while(read>>item1)
            {
                read>>stock1;
                read>>price1;
                if(item1==item)
                {
                    write<<item<<" ";
                    write<<stock<<" ";
                    write<<price<<endl;
                }
                else
                {
                    write<<item1<<" ";
                    write<<stock1<<" ";
                    write<<price1<<endl;
                }   
            }
            read.close();
            write.close();
            remove("items.txt");
            rename("new.txt","items.txt");
            if(count==0)
            {
                cout<<"Item not found in the stock.."<<endl;
                cout<<"Press Any Key to continue. ";
                fflush(stdin);
                getchar();
            }
        }
        float items_of_cashier(string search)
        {
            string item,item1;
            int stock,stock1,price,price1,n,num,count=0;
            float total;
            read.open("items.txt");
            while(read>>item)
            {
                read>>stock;
                read>>price;
                if(item==search)
                {   
                    count=1;                     
                    cout<<"Enter quantity of "<<item<<" you want to buy :"; 
                    cin>>stock1;
                    itm_q.push_back(make_pair(item,stock1));  
                    total=stock1*price;
                    pr_t.push_back(make_pair(price,total));
                    stock-=stock1;
                    break;                  
                }
            }
            read.close();
            if(count==0)
            {
                cout<<"Not found in our stock"<<endl;
            }
            read.open("items.txt");                             // to write the updated items into file      
                
            write.open("new.txt");                              // to write the updated items into file                          
            while(read>>item1)
            {
                read>>stock1;
                read>>price1;
                if(item1==item)
                {
                    write<<item<<" ";
                    write<<stock<<" ";
                    write<<price<<endl;
                }
                else
                {
                    write<<item1<<" ";
                    write<<stock1<<" ";
                    write<<price1<<endl;
                }   
            }
            read.close();
            write.close();
            remove("items.txt");
            rename("new.txt","items.txt");
            return total;
        }
        void stock_of_items()
        {
            system("CLS");
            string search;
            cout<<"Enter item name that you want to update in stocks : ";
            cin>>search;
            items(search);                
        }

        void add_new_item()
        {
            write.open("items.txt",ios::app);
            string item,item1;
            int stock,stock1,price,price1,count,n=0;
            do
            {
                count=0;
                cout<<"Enter item name that you want to add in stock : ";
                cin>>item;
            
                read.open("items.txt");
                while(read>>item1)                                              // if the item already exist in the file
                {
                    read>>stock1;
                    read>>price1;
                    if(item1==item)
                    {
                        count=1;
                        cout<<"Item already present in stocks."<<endl;
                        cout<<"Please select correct option to update existing item "<<endl;
                        fflush(stdin);
                        cout<<"Enter any key to proceed.";
                        getchar();
                        system("CLS");
                        count=1;
                        read.close();
                        break;
                    }
                }
                if(count==0)                                        // if the item is not in the file
                {
                    cout<<"Enter stock of "<<item<<" :";
                    cin>>stock;
                    cout<<"Enter price of "<<item<<" :";
                    cin>>price;
                    write<<item<<" ";
                    write<<stock<<" ";
                    write<<price<<endl; 
                }
                system("CLS");
                cout<<"Press"<<endl<<"0-Add more items to  stock "<<endl;
                cout<<"1-Exit."<<endl;
                cout<<"Please, Enter your choice : ";
                cin>>n;
                read.close();
            }while(n!=1);
        }
        
        void generate_bill()
        {
            system("CLS");
            float total;
            int i=1;
            cout<<"Enter 0 to stop ordering ."<<endl;
            cout<<"Enter item name :"<<endl; 
            string s;
            do
            {
                fflush(stdin);
                cout<<i<<" ";
                i++;
                getline(cin,s);                
                total=items_of_cashier(s);
                Grand_total+=total;
            }while (s!="0");
            printing_bill();
        }
        void load_animation()
        {
            int i = 0; 
 	        char load[9]; 
 	        while(i < 8) 
 	        { 
 		        system("CLS"); 
 		        load[i++] = '.'; 
 		        load[i] = '\0'; 
		        printf("\n\nLOADING, Please wait %-8s", load); 
 		        usleep(199900); 
 	        } 
            system("CLS"); 
            printf("\n"); 
        }
        void printing_bill()
        {
            unsigned long long int receipt_number,order_number;
            read.open("order.txt");
            read>>receipt_number>>order_no;
            receipt_number++;
            order_no++;
            read.close();
            write.open("order.txt");
            write<<receipt_number<<endl<<order_no;
            write.close();

            gst=(.18*Grand_total);
            time_t now = time(NULL);            
            string t=ctime(&now);
            system("CLS");
            cout<<"Please, Enter customer's name :";
            getline(cin,customer);
            system("CLS");
            load_animation();
            
            system("CLS"); 
            cout<<"--------------------------- TRENDS SUPERMARKET ----------------------------"<<endl<<endl;
            cout<<"                           ISBT Road , DEHRADUN                   "<<endl;
            cout<<"                               PIN :- 248001                          "<<endl<<endl;
            cout<<"                      FSSAI REG. NO. :- 998656890328                   "<<endl;
            cout<<"                        TAX NO. :- BGL5489057WQLN4                "<<endl<<endl;
            cout<<"  Reciept Number :- "<<receipt_number<<endl;
            cout<<"  "<<t<<endl;
            cout<<"  Manager Name  :-  "<<manager<<endl;
            cout<<"  Cashier Name  :-  "<<cashier<<endl;
            cout<<"  Customer Name :- "<<customer<<endl;
            cout<<"  Order Number  :- "<<order_no<<endl;
            cout<<"  ------------------------------------------------------------------ "<<endl;
            cout<<"  No Item \tQuantity\t\tRate\t\tNet Amt"<<endl;
            for(int i=0;i<itm_q.size();i++)
            {
                cout<<"  "<<i+1<<". "<<itm_q[i].first<<"\t   "<<itm_q[i].second<<"\t\tX\tRs."<<pr_t[i].first<<"\t=\tRs."<<pr_t[i].second<<endl; 
            }
            cout<<"  ------------------------------------------------------------------ "<<endl<<endl;
            cout<<"  Item's Count :- "<<itm_q.size()<<endl<<endl;
            cout<<"  ------------------------------------------------------------------ "<<endl<<endl;
            cout<<"  Subtotal :-                                                Rs. "<<Grand_total<<endl;
            cout<<"  GST (18%)  :-                                              Rs. "<<gst<<endl<<endl;
            cout<<"  ------------------------------------------------------------------ "<<endl<<endl;
            cout<<"  \e[1mTOTAL\e[1m                                                    Rs. "<<Grand_total+gst<<endl<<endl;
            cout<<"  \e[0m\e[0m------------------------------------------------------------------ "<<endl<<endl;
            cout<<"  Thank You! "<<customer<<" for shopping with us."<<endl<<endl;
            cout<<"  ------------------------------------------------------------------ "<<endl;
            cout<<"                NOTE : NO RETURN, NO REFUND POLICIES"<<endl;
            cout<<"  ------------------------------------------------------------------ "<<endl;

            stringstream ss;
            ss<<"bills/"<<customer<<"_"<<receipt_number<<".txt";
            string filename;
            ss>>filename;
            write.open(filename);
            write<<"------------------------ TRENDS SUPERMARKET -------------------------"<<endl<<endl;
            write<<"                        ISBT Road , DEHRADUN                   "<<endl;
            write<<"                            PIN :- 248001                          "<<endl<<endl;
            write<<"                      FSSAI REG. NO. :- 998656890328                   "<<endl;
            write<<"                        TAX NO. - BGL5489057WQLN4                 "<<endl<<endl;
            write<<"  Reciept Number :- "<<receipt_number<<endl;
            write<<"  "<<t<<endl;
            write<<"  Manager Name  :-  "<<manager<<endl;
            write<<"  Cashier Name  :-  "<<cashier<<endl;
            write<<"  Customer Name :-  "<<customer<<endl;
            write<<"  Order Number  :- "<<order_no<<endl;
            write<<"  ------------------------------------------------------------------ "<<endl;
            write<<"  No Item \tQuantity\t\tRate\t\tNet Amt"<<endl;
            for(int i=0;i<itm_q.size();i++)
            {
                write<<"  "<<i+1<<". "<<itm_q[i].first<<"\t   "<<itm_q[i].second<<"\t\tX\tRs."<<pr_t[i].first<<"\t=\tRs."<<pr_t[i].second<<endl; 
            }
            write<<"  ------------------------------------------------------------------ "<<endl<<endl;
            write<<"  Item's Count :- "<<itm_q.size()<<endl<<endl;
            write<<"  ------------------------------------------------------------------ "<<endl<<endl;
            write<<"  Subtotal :-                                                 Rs."<<Grand_total<<endl;
            write<<"  GST (18%)  :-                                               Rs."<<gst<<endl<<endl;
            write<<"  ------------------------------------------------------------------ "<<endl<<endl;
            write<<"  TOTAL                                                     Rs."<<Grand_total+gst<<endl<<endl;
            write<<"  ------------------------------------------------------------------ "<<endl<<endl;
            write<<"  Thank You! "<<customer<<" for shopping with us."<<endl<<endl;
            write<<"  ------------------------------------------------------------------ "<<endl;
            write<<"                NOTE : NO RETURN, NO REFUND POLICIES"<<endl;
            write<<"  ------------------------------------------------------------------ "<<endl;
            getchar();
            system("CLS");
        }
};
// Login using inherited properties of the Parent class User
class login:public User
{  
    public:
        login()
        {
            initially();
        }
        bool manager_check()
        {
            string id,pass;
            fflush(stdin);
            cout<<"Enter Manager's id : ";
            getline(cin,id);
            fflush(stdin);
            cout<<"Enter Manager's password : ";
            getline(cin,pass);
            return check_manager(id,pass);
        }
        int pass_check()
        {
            system("CLS");
            string id,pass;
            fflush(stdin);
            cout<<"Enter User-Id : ";
            getline(cin,id);
            fflush(stdin);
            cout<<"Enter User-Password : ";
            getline(cin,pass);
            return check_pass(id,pass);
        }
        void set_id_password()  // to set the id - password & call setter from the parent class
        {
            set_id_pass();
        }
        void sign_up()        //to set the id - password for Manager & Cashier
        {   
            int n;
            string id,pass;
            do
            {
                cout<<"Enter :"<<endl;
                cout<<"1-Sign up as Manager."<<endl;
                cout<<"2-Sign up as Cashier."<<endl; 
                cout<<"3-Exit."<<endl;
                cout<<"Please, Enter your choice : ";    
                cin>>n; 
                if(n==1)
                {                  
                    cout<<"Enter Manager's Username :";
                    fflush(stdin);
                    getline(cin,id);
                    cout<<endl<<"Enter Manager's Password :";
                    fflush(stdin);
                    getline(cin,pass);
                    set_manager(id,pass);             
                }
                else if(n==2)        
                {
                    cout<<"Enter Cashier's Username :";
                    fflush(stdin);
                    getline(cin,id);
                    cout<<endl<<"Enter Cashier's Password :";
                    fflush(stdin);
                    getline(cin,pass);
                    set_cashier(id,pass);                  
                }
                else if(n>3)
                {
                    cout<<" You have entered wrong choice! Please Retry."<<endl;
                }
            }while(n!=3);
        }
        void manager_id()
        {
            int n;
            do
            {
                system("CLS");
                cout<<"Welcome Manager : "<<endl<<endl;
                cout<<"Enter your command : "<<endl;
                cout<<"1- Manage items stock or change item price."<<endl;
                cout<<"2- Add new items in list ."<<endl;
                cout<<"3- Exit ."<<endl;
                cout<<"Please, Enter your choice : ";
                cin>>n;
                switch(n)
                {
                    case 1:
                        stock_of_items();
                        break;
                    case 2:
                        add_new_item();
                        break;
                    default:
                        cout<<"Wrong choice"<<endl;                        
                }
            } while (n!=3);
        }
        void cashier_id()
        {
            int n;
            do
            {
                system("CLS");
                cout<<"Welcome Cashier : "<<endl<<endl;
                cout<<"Enter your command : "<<endl;
                cout<<"1- Create Bill."<<endl;
                cout<<"2- Exit ."<<endl;
                cout<<"Please, Enter your choice : ";
                cin>>n;
                switch(n)
                {
                    case 1:
                        generate_bill();
                        break;
                    case 2:
                        break;
                    default:cout<<"Wrong choice."<<endl;
                }
            } while(n!=2);
            
        }
};
int main()
{

    login a;       //Object for accessing the data members of the class User
    a.set_id_password();    
    int choice;
    int c;
    // Menu for displaying the login page
    do
    {
        system("CLS");
        cout<<setfill('*')<<setw(70)<<"*"<<endl;        
        cout<<"-----------------------TRENDS SUPERMARKET------------------------"<<endl<<endl;
        cout<<"                     Welcome to login page                               "<<endl<<endl;
        cout<<"*******************        USER-MENU      ***********************"<<endl<<endl;
        cout<<"1.Login"<<endl;
        cout<<"2.Register"<<endl;
        cout<<"3.Exit"<<endl;
        cout<<"\nPlease, Enter your choice :";
        cin>>choice;
        cout<<endl;
        switch(choice)
        {
            case 1:
                c=a.pass_check();
                if(c==1)//for manager
                {                    
                    system("CLS");
                    cout<<endl<<"Permission Granted"<<endl<<endl;
                    
                    
                    a.manager_id();    // Access the signup member function of class login
                }
                else if(c==2)//for cashier
                {
                    system("CLS");
                    cout<<endl<<"Permission Granted"<<endl<<endl;
                    
                    a.cashier_id();
                }
                else
                {
                    system("CLS");
                    cout<<endl<<"Permission Denied"<<endl<<endl;
                             
                }
                break;
            case 2:
                if(a.manager_check())
                {
                    system("CLS");
                    cout<<endl<<"Permission Granted"<<endl<<endl;
                    
                    a.sign_up();    // Access the signup member function of class login
                }
                else
                {
                    system("CLS");
                    cout<<endl<<"Permission Denied"<<endl<<endl;
                    
                }
                break;
            case 3:
                break;
            default:
                system("CLS");
                cout<<"You entered wrong credentials, try again\n"<<endl; 
                main();
        }
    } while(choice!=3);
    return 0;
}
