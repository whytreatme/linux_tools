#include <sys/types.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(){
    if(fork() > 0 ){
        cout << "Hello from parent" << enndl;
    }
    else{
        cout << "Hello from child" << endl;
    }
    return 0;
}