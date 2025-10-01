#include  "myls.h"


using namespace std;

int main(int argc, char *argv[]){
    myls_dir op1(argc,argv);
    return op1.list_opertation();
}