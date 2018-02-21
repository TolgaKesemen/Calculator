#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <cmath>

/*
    This program evaluates given expressions with using custom
    Stack structure. It takes one input file as argument and
    four output file names also as argument. It writes prefix,
    infix and postfix expressions of the given expression and
    the result of that.
*/


using namespace std;

/*
    This class used as Stack structure. It stores the variables in a
    string array 'st'. 'stc' (stack counter) used like stack pointer.
    It has four public functions which are push(), peek(), pop() and
    clear().
*/
class Stack{
    public:
        string st[1024];
        int stc = -1;
        void push(string s){ stc++; st[stc] = s;}
        string peek(){ if(stc > -1){ return st[stc];}}
        void pop(){ if(stc > -1){ st[stc].clear(); stc--;}}
        void clear(){ if(stc > -1){ while(stc!=-1){ st[stc].clear(); stc--;}}}
};

/*
    That functions takes two numbers from valstack, convert them to
    integer from string, takes an operator from opstack then making
    the calculation. The result from this calculation is pushed back
    into valstack. This process continues until opstack is empty.
    Finally result of all is written to the file.
*/
void calc(Stack valst, Stack opst, ofstream& resultWrite){

    int operand1, operand2;
    string op;
    int n = opst.stc;
    while(n > -1){                                                  /*Repeating until there is no operator left*/
        operand1 = atoi(valst.peek().c_str());
        valst.pop();
        operand2 = atoi(valst.peek().c_str());
        valst.pop();
        op = opst.peek();
        opst.pop();
        if(!op.compare("+")){
            operand2 += operand1;
        }
        else if(!op.compare("-")){
            operand2 -= operand1;
        }
        else if(!op.compare("*")){
            operand2 *= operand1;
        }
        else if(!op.compare("/")){
            if(operand1 == 0){                                      /*If 'number/0' then program terminates and return NaN*/
                valst.clear();
                valst.push("NaN");
                break;
            }
            else{
                operand2 /= operand1;
            }
        }
        else if(!op.compare("^")){
            operand2 = pow(operand2, operand1);
        }
        char* temp = new char[20];
        sprintf(temp, "%d", operand2);
        string result = string(temp);
        delete temp;
        valst.push(result);
        n--;
    }
    resultWrite << valst.peek() << endl;
}

/*
    This function creating the prefix form of given expression with
    using Stack structures and writes it to the file.
*/
void PrefixWrite(Stack valstack, Stack opstack, ofstream& prefixWrite){

    string reverseprefix[100];
    string prefix[100];
    reverseprefix[0] = valstack.peek();
    reverseprefix[1] = " ";
    valstack.pop();
    int k = 1;
    while(opstack.stc > -1){
        if(valstack.stc > -1){
            reverseprefix[++k] = valstack.peek();
            reverseprefix[++k] = " ";
            valstack.pop();
        }
        reverseprefix[++k] = opstack.peek();
        reverseprefix[++k] = " ";
        opstack.pop();
    }
    int reverseprefixlength = k--;
    for(int l=0; l < reverseprefixlength; l++){
        prefix[l] = reverseprefix[k];
        k--;
    }
    k++;
    while(k < reverseprefixlength){
        prefixWrite << prefix[k] ;
        k++;
    }
    prefixWrite << endl;
}

/*
    This function creating the infix form of given expression. It add the
    necessary parenthesis then writes it to the file.
*/
void InfixWrite(Stack valstack, Stack opstack, ofstream& infixWrite){

    string infix = "";
    string operand1, operand2, op;
    int i = opstack.stc;
    while(i > -1){
        operand2 = valstack.peek();
        valstack.pop();
        operand1 = valstack.peek();
        valstack.pop();
        op = opstack.peek();
        opstack.pop();
        infix += operand1;
        infix += " ";
        infix += op;
        infix += " ";
        infix += operand2;

        if(((!op.compare("-")) || (!op.compare("+"))) && (i != 0)){
            infix.insert(0, "( ");
            infix.insert(infix.length(), " )");
        }

        valstack.push(infix);
        if(i != 0){ infix.clear();}
        i--;
    }
    infixWrite << infix << endl;
}

/*
    This function creating the prefix form of given expression with
    using Stack structures and writes it to the file.
*/
void PostfixWrite(Stack valstack, Stack opstack, ofstream& postfixWrite){

    string reversepostfix[100];
    string postfix[100];
    int j=0;
    while(valstack.stc > -1){
        reversepostfix[j] = valstack.peek();
        valstack.pop();
        j++;
    }
    while(j > -1){
        postfixWrite << reversepostfix[j] << " " ;
        j--;
    }
    while(opstack.stc > -1){
        postfixWrite << opstack.peek() << " " ;
        opstack.pop();
    }
    postfixWrite << endl;
}

/*
    This function reads the input file line by line. Splitting those
    lines into tokens with respect to white spaces. It pushes tokens
    to stacks with the control of is it value or operator(creating
    stacks). Then calls PrefixWrite(), InfixWrite(), PostfixWrite(),
    calc() respectively.
*/
void ReadWriteFile(char**& argv){

    ifstream readfile;
    readfile.open(argv[1], ios::in);
    ofstream prefixWrite(argv[2], ios::out);
    ofstream infixWrite(argv[3], ios::out);
    ofstream postfixWrite(argv[4], ios::out);
    ofstream resultWrite(argv[5], ios::out);
    if(!readfile){
        cout << "File not opened!!!" << endl;
    }
    else{
        string line;
        Stack valstack;
        Stack opstack;
        int i=0;
        while(!readfile.eof()){
            getline(readfile, line);
            stringstream ss(line);
            string token;
            while(i<line.length()){
                getline(ss, token, ' ');
                if(!token.empty() && ((atoi(token.c_str()) != 0) || (!token.compare("0")))){
                    valstack.push(token);
                }
                if((atoi(token.c_str()) == 0) && (token.compare("0"))){
                    opstack.push(token);
                    break;
                }
                i++;
            }
            i = valstack.stc;
            while(i > 1){
                getline(ss, token, ' ');
                if(!token.empty()){
                    opstack.push(token);
                }
                i--;
            }
            i = 0;
            PrefixWrite(valstack, opstack, prefixWrite);
            InfixWrite(valstack, opstack, infixWrite);
            PostfixWrite(valstack, opstack, postfixWrite);
            calc(valstack, opstack, resultWrite);
            valstack.clear();
            opstack.clear();
        }
        prefixWrite.close();
        infixWrite.close();
        postfixWrite.close();
        resultWrite.close();
        readfile.close();
    }
}

int main(int argc, char** argv)
{
    ReadWriteFile(argv);
    return 0;
}
