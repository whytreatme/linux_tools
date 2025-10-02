#include <unistd.h> //get_current_dir_name()
#include <dirent.h> //dir , struct dirent, readdir
#include <getopt.h> //getopt()
#include <cstring>  //strcmp()
#include <string>  //string类
#include <sys/stat.h> // struct stat结构体
#include <sys/types.h>
#include <pwd.h> //getpwuid()
#include <grp.h> //getgrgid()
#include <time.h> //localtime()
#include <iostream>
#include <iomanip>
#include "myls.h"
using namespace std;


    myls_dir::myls_dir(int argc, char *argv[]) : 
        my_mode(HIDE_DOT),long_format(false){
        //cout << "我在这里，在构造函数这里" <<endl;
        //cout << "my_mode: " << this->my_mode <<endl;
        //cout << "long_format: " << this->long_format <<endl;

        int opt = 0;
        //cout << "opt: " << opt << endl;
        while( (opt = getopt(argc, argv, this->argument)) != -1){
          //  cout << "我有在执行哦！" << endl;
            this->my_option(opt);
        }
        this->path = (optind < argc) ? argv[optind] : ".";

    }

    void myls_dir::my_option(int opt){
        //cout << "我在这里，在my_option这里" <<endl;
        switch(opt){
            case 'a': this->my_mode = SHOW_ALL; break;
            case 'A': this->my_mode = SHOW_EXCEPT_DOTDOT; break;
            case 'l': this->long_format = true; break;

        }
    }

    int myls_dir::list_opertation(const char *my_path){
        //cout << "我在这里，在list_opertation" <<endl;
        DIR *dir;
        struct dirent *stdinfo = nullptr;

        if( (dir = opendir(this->path.c_str())) == nullptr){
            cerr << "Error opening directory!" << endl;
            return -1;
        }
        // if(this->option & OPTION_A) handle_A(dir, nullptr);
        // if(this->option & OPTION_a) handle_a(dir, nullptr);
        // if(this->option == OPTION_VOID) handle_non(dir, nullptr);

        while(true){
            //cout << "我进到文件遍历的循环了哦" << endl;
            if( (stdinfo = readdir(dir)) == nullptr) break;
            if(!this->filter(stdinfo)) continue;
            display_entries_info(stdinfo->d_name);
            cout << stdinfo->d_name << endl;
        }

        closedir(dir);
        return 0;
    }

    //====================输出文件信息的逻辑=========================
    void myls_dir::display_entries_info(const char* file_name){
        struct stat st;
        string file_path = this->path;
        file_path += "/" ; file_path += file_name;
        
        if (stat(file_path.c_str(),&st) != 0){
            cout << "Can't find " << file_path  << endl; return;
        }

        if(this->long_format){
            long_format_show(st);
            cout << "  ";
        }
        return;
    }

//   长格式自身的处理逻辑
  void myls_dir::long_format_show(struct stat& st){
            if(S_ISDIR(st.st_mode)){
                cout <<"d";
            }
            else if(S_ISREG(st.st_mode)){
                cout <<"-";
            }
            else if(S_ISLNK(st.st_mode)){
                cout <<"l";
            }
            else if(S_ISSOCK(st.st_mode)){
                cout <<"s";
            }
            else if(S_ISBLK(st.st_mode)){
                cout <<"b";
            }
            else if(S_ISFIFO(st.st_mode)){
                cout <<"p";
            }
            else{
                cout <<"c";
            }

        cout << ((st.st_mode & S_IRUSR) ? 'r' : '-');
        cout << ((st.st_mode & S_IWUSR) ? 'w' : '-');
        cout << ((st.st_mode & S_IXUSR) ? 'x' : '-');
        cout << ((st.st_mode & S_IRGRP) ? 'r' : '-');
        cout << ((st.st_mode & S_IWGRP) ? 'w' : '-');
        cout << ((st.st_mode & S_IXGRP) ? 'x' : '-');
        cout << ((st.st_mode & S_IROTH) ? 'r' : '-');
        cout << ((st.st_mode & S_IWOTH) ? 'w' : '-');
        cout << ((st.st_mode & S_IXOTH) ? 'x' : '-');

        //硬链接数
        cout << " " << setw(2) << st.st_nlink;

        //组名、用户名
        struct passwd *pw = getpwuid(st.st_uid);
        cout << " " << (pw ? pw->pw_name : to_string(st.st_uid));
        struct group *gr = getgrgid(st.st_gid);
        cout << " " << (gr ? gr->gr_name : to_string(st.st_gid));
        
         // 文件大小
        cout << " " << setw(6) << st.st_size;

        //最后一次的修改日期
        struct tm *ctm = localtime(&st.st_mtime);
        cout << " " << setw(4) << ctm->tm_mon <<"月";
        cout << " " << setw(4) << ctm->tm_mday;
        cout << " " << setw(6) << ctm->tm_year;
    }




    //======================遍历选取的逻辑============================================
    bool myls_dir::filter(struct dirent *stdinfo){
            // cout << "我在这里，在filter里面" << endl;
            // cout << "my_mode: " <<my_mode << endl;
            // cout << "stdinfo->d_name: " << stdinfo->d_name << endl;
             if(this->my_mode == SHOW_EXCEPT_DOTDOT){
                if(strcmp(stdinfo->d_name, ".") == 0 || strcmp(stdinfo->d_name,"..") == 0)
                    return false;           
            }
            else if(this->my_mode == HIDE_DOT){
                if(stdinfo->d_name[0] == '.') return false;
            }
            return true;
    }

    

    myls_dir::~myls_dir(){}
