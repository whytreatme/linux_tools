#ifndef MYLS_H
#define MYLS_H

#include <dirent.h>
#include <string>
#define OPTION_VOID 0
#define OPTION_A (1 << 0)
#define OPTION_a (1 << 1)



class myls_dir{
    private:
        const char *argument = "aA";
        int option;
        std::string path;
        void my_option(int);
        void handle_A(DIR*, struct dirent*);
        void handle_a(DIR*, struct dirent*);
        void handle_non(DIR*, struct dirent*);
    
    public:
        int list_opertation(const char *my_path = nullptr);
        myls_dir(int agrc, char *argv[]);
        myls_dir(const myls_dir&) = delete;
        myls_dir& operator=(const myls_dir& ) = delete;
        ~myls_dir();
};


#endif