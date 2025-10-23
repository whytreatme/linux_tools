#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
 
//getpid(),每个进程都有自己独立的pid,不要和fork()的返回值搞混
using namespace std;

void FathEXIT(int sig);
void child_handler(int sig);
void childEXIT(int sig);

int main(){
    pid_t pid;
    for(int i = 0; i < 64; i++) signal(i, SIG_IGN); //忽略掉其它信号的干扰
    
    signal(SIGINT, FathEXIT);
    signal(SIGTERM, FathEXIT);
    signal(SIGCHLD,child_handler);  //注册函数处理子进程退出的信号

    for(int j = 0; j < 3; j++){
        //fork()是立即返回的 - 父进程不会等待子进程完成
        //子进程并行执行 - 所有子进程同时运行
        //执行顺序不确定 - 完成顺序取决于任务耗时
        pid = fork(); //父进程返回子进程pid，子进程返回0
        if(pid < 0) return -1;
        else if(pid == 0) {
            signal(SIGINT, SIG_IGN); //覆盖继承
            signal(SIGCHLD, SIG_DFL);//恢复SIGCHLD默认行为
            signal(SIGTERM, childEXIT);
            cout << "产生子进程，PID=" << getpid() << endl;
            switch(j){
                case 0: system("echo '任务0'"); sleep(10);break;
                case 1: system("echo '任务1'"); sleep(20);break;
                case 2: system("echo '任务2'"); sleep(30);break;
            }
            cout << "子进程 PID=" <<getpid() << " 完成任务，现在退出" << endl;
            exit(0); //退出循环很重要，不然会导致fork()炸弹
        }
    }
    while(true){
        //父进程处理其它任务

        sleep(1);
    }
    return 0;
}

void FathEXIT(int sig){
    signal(SIGTERM, SIG_IGN);

    cout << "父进程退出，sig = " << sig << endl;
    kill(0, SIGTERM); //通知进程组里的所有进程退出

    sleep(10); //给子进程一点时间退出

    exit(0);
}

//处理子进程退出，防止出现僵尸进程
void child_handler(int sig){
    
    pid_t pid;
    int sts;

    while((pid = waitpid(-1, &sts, WNOHANG)) > 0){
        cout << "已终止子进程编号：" << pid << endl;
        if(WIFEXITED(sts)){
            cout << "子进程正常退出，退出状态：" << WEXITSTATUS(sts) << endl;
        }
        else{
            cout << "子进程异常退出，终止信号是：" << WTERMSIG(sts) << endl;
        }
    }
}

void childEXIT(int sig){
    signal(SIGTERM, SIG_IGN);
    signal(SIGINT, SIG_IGN);

    cout << "子进程 " << getpid() << " 退出，sig = " << sig << endl;
    exit(0); 
}    