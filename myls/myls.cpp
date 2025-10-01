#include <unistd.h> //get_current_dir_name()
#include <dirent.h> //dir , struct dirent, readdir
#include <getopt.h> //getopt()
#include <cstring>  //strcmp()
#include <string>
#include <iostream>
#include "myls.h"
using namespace std;


    myls_dir::myls_dir(int argc, char *argv[]) : option(0){
        int opt;
        while( (opt = getopt(argc, argv, this->argument)) != -1){
            this->my_option(opt);
        }

        this->path = (optind < argc) ? argv[optind] : ".";

    }

    void myls_dir::my_option(int opt){
        switch(opt){
            case 'a': this->option |= OPTION_a; break;
            case 'A': this->option |= OPTION_A; break;

        }
    }

    int myls_dir::list_opertation(const char *my_path){
        DIR *dir;
        if( (dir = opendir(this->path.c_str())) == nullptr){
            cerr << "Error opening directory!" << endl;
            return -1;
        }
        if(this->option & OPTION_A) handle_A(dir, nullptr);
        if(this->option & OPTION_a) handle_a(dir, nullptr);
        if(this->option == OPTION_VOID) handle_non(dir, nullptr);

        closedir(dir);
        return 0;
    }

    void myls_dir::handle_A(DIR* dir, struct dirent *stdinfo){
        while(true){
            if( (stdinfo = readdir(dir)) == nullptr) break;
            if(strcmp(stdinfo->d_name, ".") == 0) continue;
            if(strcmp(stdinfo->d_name, "..") == 0) continue;
            cout << stdinfo->d_name << endl;
        }
    }

    void myls_dir::handle_a(DIR* dir, struct dirent *stdinfo){
        
        while(true){
            if( (stdinfo = readdir(dir)) == nullptr) break;
            cout << stdinfo->d_name << endl;
        }
    }

    void myls_dir::handle_non(DIR* dir, struct dirent *stdinfo){
        while(true){
            if( (stdinfo = readdir(dir)) == nullptr) break;
            if(stdinfo->d_name[0] == '.') continue;
            cout << stdinfo->d_name << endl;
        }
    }


    myls_dir::~myls_dir(){}
