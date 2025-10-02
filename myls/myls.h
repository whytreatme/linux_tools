#ifndef MYLS_H
#define MYLS_H

#include <dirent.h>
#include <string>
#include <sys/stat.h>


enum ShowHiddenMode{
    HIDE_DOT,
    SHOW_ALL,
    SHOW_EXCEPT_DOTDOT
};

class myls_dir{
    private:
        const char *argument = "aAl";
        std::string path;
        ShowHiddenMode my_mode;  //用来解决遍历问题
        bool long_format;        // 用来解决长格式输出问题
        void my_option(int);
        bool filter(struct dirent*);
        void display_entries_info(const char* file_name);
        void long_format_show(struct stat&);
    
    public:
        int list_opertation(const char *my_path = nullptr);
        myls_dir(int argc, char *argv[]);
        myls_dir(const myls_dir&) = delete;
        myls_dir& operator=(const myls_dir& ) = delete;
        ~myls_dir();
};


#endif