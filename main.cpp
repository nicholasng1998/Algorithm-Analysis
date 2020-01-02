#include <iostream> // Input and Output : cin and cout
#include <fstream>  //  
#include <vector>
#include <sstream> // to split string
#include "HashTable.cpp"
#include <string> // char at string
#include <chrono> // to count the process time
#include <locale> // std::locale, std::toupper
using namespace std;

// declare hash table with size 11, prime number;
// declare hash table with size 10, which is not prime number;
HashTable <vector<string>> ht(11); 

// declare a new vector to store duplicates keys
vector<vector<string> > duplicates;

// the declaration of the binary tree node
struct BstNode
{
    vector<string> data;
    BstNode* left;
    BstNode* right;
    BstNode* duplicates; // implementation for AVL with duplicates keys
    int height; // implementation for AVL
};

// declare binary search root
BstNode* root = NULL;

// function to get the height of the tree AVL
int height(BstNode *node){
    if(node==NULL)
        return 0;
    return node->height;
}

// function to get maximum of two height
int max(int heightA, int heightB){
    return (heightA > heightB)? heightA : heightB; // true:heightA, false:heightB 
}

// function to allocates a new node with the given data
BstNode* getNewNode(vector<string> data)
{
    BstNode* newNode = new BstNode();
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // new node is initially added at leaf
    return newNode;
}

// function to right rotate subtree rooted with z
/*
       z                                y
      / \                             /   \
     y   zRightLeaf,T4    =>         x     z
    / \                             / \   /  \
   x   yRightLeaf,T3," "           T1 T2 T3   T4
  / \
T1   xRightLeaf,T2
*/
BstNode* rightRotate(BstNode *z) 
{
    BstNode *y = z->left; // y is on the left side of z
    BstNode *T3 = y->right; // T3 is right leaf of y which is " "  

    // perform rotation
    y->right = z; // right leaf of y is now z
    z->left = T3; // put T3 as the left leaf of z

    // update the height
    z->height = max(height(z->left),height(z->right))+1;
    y->height = max(height(y->left),height(y->right))+1;

    // return new root
    return y;
}

// function to left rotate subtree rooted with z
/*
   z                    y
  / \                 /   \     
T1   y               z     x
    /  \      =>    / \   / \
   T2   x          T1 T2 T3 T4
       / \
      T3  T4
*/
BstNode* leftRotate(BstNode *z) 
{
    BstNode *y = z->right; // y is on the right side of z
    BstNode *T2 = y->left; // T2 is left leaf of y which is " "  

    // perform rotation
    y->left = z; // left leaf of y is now z
    z->right = T2; // put T2 as the right leaf of z

    // update the height
    z->height = max(height(z->left),height(z->right))+1;
    y->height = max(height(y->left),height(y->right))+1;

    // return new root
    return y;
}

// function to get balance factor of node N
int getBalance(BstNode* node)
{
    if(node==NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// insert node into a binary tree function
BstNode* insert(BstNode* root, vector<string> data)
{
    if(root == NULL)
    {
        return(getNewNode(data));
    }
    
    // if key already exists in BST, increment count and return
    
    if(data.at(1).length() == root->data.at(1).length()){
        duplicates.push_back(data);
    }
    else if(data.at(1).length() < root->data.at(1).length()){
        root->left = insert(root->left,data);
    }
    else{
        root->right = insert(root->right,data);
    }
    

    // update height of this ancestor node
    root->height = 1 + max(height(root->left),height(root->right));

    // get balance factor of this ancestor node to check whether this node became unbalanced
    int balance = getBalance(root);
    // right single rotation
    if (balance > 1 && data.at(1).length() < root->left->data.at(1).length())
    {
        return rightRotate(root); 
    }
    // left single rotation
    if (balance < -1 && data.at(1).length() > root->right->data.at(1).length())
    {    
        return leftRotate(root); 
    }
    // left right double rotation 
    if (balance > 1 && data.at(1).length() > root->left->data.at(1).length()) 
    { 
        root->left =  leftRotate(root->left); 
        return rightRotate(root); 
    } 
  
    // right left double rotation
    if (balance < -1 && data.at(1).length() < root->right->data.at(1).length()) 
    { 
        root->right = rightRotate(root->right); 
        return leftRotate(root); 
    } 
  
    /* return the (unchanged) node pointer */
    return root; 

} 

// a class to read data from a csv file
class CSVReader
{
    string fileName;
    string delimeter;

public:
    CSVReader(string filename, string delm = ",") :
            fileName(filename), delimeter(delm)
    { }

    // function to fetch data from a CSV File
    vector<vector<string> > getData();
};

// split function into a vector
vector<string> split(string strToSplit, char delm)
{
    stringstream ss(strToSplit);
    string item;
    vector<string> splittedStrings;
    while (getline(ss, item, delm))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}

// parses through csv file line by line and returns the data in vector of vector of strings
vector<vector<string> > CSVReader::getData()
{
    ifstream file(fileName.c_str());

    vector<vector<string> > dataList;
    string line = "";
    getline(file,line); // skip the header.
    // iterate through each line and split the content using delimeter
    while (getline(file, line))
    {
        vector<string> vec;
        vec = split(line,',');
        dataList.push_back(vec);
    }
    // close the File
    file.close();

    return dataList;
}

string searchByVector(string data){

    for(int i = 0; i < duplicates.size();i++)
    {
        if(data == duplicates[i][1]){
            return duplicates[i][0];
        }
        
        if(i==duplicates.size())
            break;
    }
    return "notfound";
}

// search a specific url's ip address
string Search(BstNode* root, string data){
    if(root == NULL){
        return "notfound";
    }
    else if(root->data.at(1) == data) 
    {
        return root->data.at(0);  // return the ip address if the url match
    }
    else if(data.length()<= root->data.at(1).length()) 
    {
        return Search(root->left,data);
    }
    else
    {
        return Search(root->right,data);
    }
}

// use binary search tree to search
void bstree(string newtarget){
    string result;
    cout << "Target to retrieve: " << newtarget;
    std::chrono::time_point<std::chrono::system_clock> start, end; //declare start & end
    start = std::chrono::system_clock::now(); //start now
    if(Search(root,newtarget)!="" && Search(root,newtarget)!="notfound")
    {
        cout<<endl;
        cout << "Its ip address is: " << Search(root,newtarget)<<endl;;
    }
    else if(Search(root,newtarget) == "notfound" && searchByVector(newtarget) != "notfound"){
        cout<<endl;
        cout<<"Its ip address is: "<< searchByVector(newtarget)<<endl;
    }else if(searchByVector(newtarget) == "notfound"){
        result = "notfound";
    }else{
        cout << "Something wrong";
    }
    end = std::chrono::system_clock::now(); //end now
    std::chrono::duration<long double> elapsed_seconds = end-start; //declare elapsed time
    cout << "Search time for " << newtarget << " is " << elapsed_seconds.count()*100 <<" seconds" << endl;
    cout<<endl;
    if(result=="notfound")
    {
        string target;
        string input;
        locale loc;
        cout << "DNS not found. Error 404." <<endl;;
        cout << "Do you want to search again? Y/N"<<endl;
        cin >> input;
        cout<< endl;
        for(int i = 0; i < input.length();i++){
            input[i] = toupper(input[i],loc);
        }
        
        if(input == "Y" or input == "N"){
            if(input == "Y")
                cout << "Please type in the domain name you want to search: ";
                cin >> target;
                bstree(target);
        }

        while(input.at(0)!='Y' && input.at(0)!='N') {
            cout<<"Wrong input. Try Again."<<endl;
            cin>>input;
            for(int i = 0; i < input.length();i++){
                input[i] = toupper(input[i],loc);
            }
            if(input == "Y"){
                cout << "Please type in the domain name you want to search: ";
                cin >> target;
                bstree(target);
            }
        }
    }
}
// Experiment Purpose
void printLeafNodes(BstNode* root) 
{ 
    // if node is null, return 
    if (!root) 
        return; 

    // if node is leaf node, print its data     
    if (!root->left && !root->right) 
    { 
        cout << "ip: \t"<<root->data.at(0)<<"\t url:\t"<<root->data.at(1) <<endl;  
        return; 
    }

    // if left child exists, check for leaf  
    // recursively 
    if (root->left) 
       printLeafNodes(root->left); 
          
    // if right child exists, check for leaf  
    // recursively 
    if (root->right) 
       printLeafNodes(root->right); 
} 

// Experiment Purpose
void printNodes(BstNode* root){
    if(!root)
        return;

    if(root)
        cout << "ip: \t"<<root->data.at(0)<<"\t url:\t"<<root->data.at(1).length() <<endl;  

    printNodes(root->left);
    printNodes(root->right);
}

// A utility function to print preorder traversal 
// of the tree. 
// The function also prints height of every node 
void preOrder(BstNode *root) 
{ 
    if(root != NULL) 
    {
        printf("%d \n",root->data.at(1).length());  
        preOrder(root->left);
        preOrder(root->right); 
    } 
} 

// use hash table to search
void htable(string target){
    cout << "Target to retrieve: " << target <<endl;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    if (ht.retrieve (target)!="" && ht.retrieve(target)!="notfound")
    {
        cout << "Its ip address is: " << ht.retrieve(target)<<endl;;
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<long double> elapsed_seconds = end-start;
    cout << "Search time for " << target << " is " << elapsed_seconds.count()*100 <<" seconds" << endl;
    
    //Exception handling for hash table
    if(ht.retrieve(target)=="notfound")
    {
        string input;
        locale loc;
        string target;
        cout << "DNS not found. Error 404." <<endl;;
        cout << "Do you want to search again? Y/N"<<endl;
        cin >> input;
        cout<< endl;
        for(int i = 0; i < input.length();i++){
            input[i] = toupper(input[i],loc);
        }
        if(input == "Y" or input == "N"){
            if(input == "Y")
                cout << "Please type in the domain name you want to search: ";
                cin >> target;
                htable(target);
        }

        while(input.at(0)!='Y' && input.at(0)!='N') {
            cout<<"Wrong input. Try Again."<<endl;
            cin>>input;
            for(int i = 0; i < input.length();i++){
                input[i] = toupper(input[i],loc);
            }
            if(input == "Y"){
                cout << "Please type in the domain name you want to search: ";
                cin >> target;
                htable(target);
            }
        }
    }
    cout<<endl;
}

int main()
{

    // Creating an object of CSVWriter
    CSVReader reader("C:/Users/Nicholas Ng/Desktop/Year 2 Sem 2/Analysis of Algorithm/assignment/nameservers.csv");
        
    // Get the data from CSV File
    vector<vector<string> > dataList = reader.getData();

    // declare the full set number of dataset 
    int numberofdataset = dataList.size();

    // number of data for testing
    int testnumber = 50000;

    // Insert into Hash Table
    for(int i = 0; i < testnumber; i++){
        string url = dataList[i][1];
        int urlLength = url.length();
        ht.insert(dataList[i],urlLength); // store the vector based on its url length
    }
    
    // Insert into BST
    for (int i = 0; i < testnumber; i++)
    {
        root = insert(root,dataList[i]);
    }

    // Main
    string input; // declare the variable of input of user
    int quit = 0; // declare the flag of quitting the program
    string target; // declare the variable of domain name target
    int choice;
    std::chrono::time_point<std::chrono::system_clock> start, end; // declare the variable of clock
    cout << "********************************" << endl;
    cout << "*WELCOME TO SEARCHING ALGORITHM*" << endl;
    cout << "********************************" << endl;
    cout << "1. Searching ip address of the domain name in AVL tree" << endl;
    cout << "2. Searching ip address of the domain name in Hash Table" << endl;
    cout << "3. Compare the searching time of prime number Hash Table and\n   normal Hash Table" << endl;
    cout << "4. Compare the searching time of Hash Table with different\n   load factor (0.5 and 0.9)" <<endl;
    cout << "5. Compare the searching time of AVL tree and Hash Table \n   searching all ip address of the domain name from AVL tree\n   and Hash Table" << endl;
    cout << "6. Quit the program." << endl;
    cout << "Please make your choice: ";
    cin >> input;
    std::istringstream iss(input);
    iss >> choice;
    while(choice == 0 or choice > 6){
        cout << "You have entered wrong choice. Try again"<<endl;
        cin >> input;
        std::istringstream iss(input);
        iss >> choice;
    }

    while(choice && choice < 7 && quit == 0)
    {
        if(choice == 1)
        {
            cout << "Binary Search Tree (AVL)"<< endl;
            cout << "Please type in the domain name you want to search: ";
            cin >> target;
            bstree(target);
            break;
        }
        else if(choice == 2)
        {
            cout << "Hash Table"<< endl;
            cout << "Please type in the domain name you want to search: ";
            cin >> target;
            htable(target);
            break;
        }
        else if(choice == 3)
        {
            ht.setSize(8); // normal hash table
            for(int i = 0; i < testnumber; i++){
                string url = dataList[i][1];
                int urlLength = url.length();
                ht.insert(dataList[i],urlLength); // store the vector based on its url length
            }
            start = std::chrono::system_clock::now();
            for(int i = 0; i < (0.5*testnumber); i++){
                htable(dataList[i][1]);
            }
            end = std::chrono::system_clock::now();
            std::chrono::duration<long double> htnormal_elapsed_seconds = end-start;

            ht.setSize(7); // prime number hash table
            for(int i = 0; i < testnumber; i++){
                string url = dataList[i][1];
                int urlLength = url.length();
                ht.insert(dataList[i],urlLength); // store the vector based on its url length
            }
            start = std::chrono::system_clock::now();
            for(int i = 0; i < (0.5*testnumber); i++){
                htable(dataList[i][1]);
            }
            end = std::chrono::system_clock::now();
            std::chrono::duration<long double> htprime_elapsed_seconds = end-start;

            cout << "Total Normal Hash Table searching time : " << htnormal_elapsed_seconds.count()*100 << " seconds"<<endl;
            cout << "Total Prime Number Hash Table searching time : " << htprime_elapsed_seconds.count()*100 << " seconds"<<endl;
            break;
        }
        else if(choice == 4)
        {
            start = std::chrono::system_clock::now();
            for(int i = 0; i < (0.5*testnumber); i++){
                htable(dataList[i][1]);
            }
            end = std::chrono::system_clock::now();
            std::chrono::duration<long double> ht05_elapsed_seconds = end-start;

            start = std::chrono::system_clock::now();
            for(int i = 0; i < (0.9*testnumber); i++){
                htable(dataList[i][1]);
            }
            end = std::chrono::system_clock::now();
            std::chrono::duration<long double> ht09_elapsed_seconds = end-start;

            cout << "Total Hash Table with load factor 0.5 searching time : " << ht05_elapsed_seconds.count()*100 << " seconds"<<endl;
            cout << "Total Hash Table with load factor 0.9 searching time : " << ht09_elapsed_seconds.count()*100 << " seconds"<<endl;
            break;
        }
        else if(choice == 5){
            start = std::chrono::system_clock::now();
            for(int i = 0; i < testnumber; i++){
                bstree(dataList[i][1]);
            }
            end = std::chrono::system_clock::now();
            std::chrono::duration<long double> bst_elapsed_seconds = end-start;

            start = std::chrono::system_clock::now();
            for(int i = 0; i < testnumber; i++){
                htable(dataList[i][1]);
            }
            end = std::chrono::system_clock::now();
            std::chrono::duration<long double> ht_elapsed_seconds = end-start;
            cout << "Total AVL Tree searching time : " << bst_elapsed_seconds.count()*100 << " seconds"<<endl;
            cout << "Total Hash Table searching time : " << ht_elapsed_seconds.count()*100 << " seconds"<<endl;
            break;
        }else{
            cout << "Thank you for using the program." << endl;
            quit = 1;
        }
    }
    return 0;
}