#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"

volatile pid_t curpid;

void sigint_handler(int sig);

char * compress_spaces(char * str);

char * get_word(char * str);

int proc_simple_com(char * com, int fd[2]);

int proc_conv(char * conv, char * fin_name, char * fout_name, int is_app);

int proc_com(char * com);

int proc_cond_exec_com(char * cecom);

int shell(char * input);

int main()
{
    signal(SIGINT, sigint_handler);
    char input[ARG_MAX];
    char cwd[PATH_MAX];
    int err;
    curpid = -1;
    while (1)
    {
        err = 0;
        getcwd(cwd, PATH_MAX);
        printf("%s%s>> %s", YELLOW, cwd, RESET);
        fgets(input, ARG_MAX, stdin);
        char * cs_input = compress_spaces(input);
        if (!strcmp(cs_input, "exit\0"))
        {
            free(cs_input);
            return 0;
        }
        err = shell(cs_input);
        if (err)
            printf("%sERROR\n%s", RED, RESET);
        free(cs_input);
    }
    return 0;
}

void sigint_handler(int sig)
{
    if (curpid != -1)
        kill(curpid, SIGINT);
    curpid = -1;
}

char * compress_spaces(char * str)
{
    if (str == NULL)
        return NULL;
    char * cs_str = (char *)malloc(strlen(str) + 1);
    size_t cs_ptr = 0, ptr = 0;
    int flag;
    while (str[ptr] != '\0')
    {
        if (str[ptr] != ' ')
        {
            if (str[ptr] != '\n')
            {
                if (str[ptr] != ' ' && str[ptr] != '&' && str[ptr] != '|' && str[ptr] != '<' && str[ptr] != '>' && str[ptr] != ';' && str[ptr] != '(' && str[ptr] != ')' && str[ptr] != '\0' && str[ptr + 1] != '\n')
                    flag = 1;
                else 
                    flag = 0;
                cs_str[cs_ptr] = str[ptr];
                cs_ptr++;
            }
            if (str[ptr] == '&')
                flag = 2;
        }
        else 
        {
            if (str[ptr + 1] != ' ')
            {
                if (flag == 1 && str[ptr + 1] != '&' && str[ptr + 1] != '|' && str[ptr + 1] != '<' && str[ptr + 1] != '>' && str[ptr + 1] != ';' && str[ptr + 1] != '(' && str[ptr + 1] != ')' && str[ptr + 1] != '\0' && str[ptr + 1] != '\n')
                {
                    flag = 0;
                    cs_str[cs_ptr] = str[ptr];
                    cs_ptr++;
                }
                if (flag == 2 && str[ptr + 1] == '&')
                {
                    cs_str[cs_ptr] = str[ptr];
                    cs_ptr++;
                }                    
            }
        }            
        ptr++;
    }
    cs_str[cs_ptr] = '\0';
    return cs_str;
}

char * get_word(char * str)
{
    if (str == NULL)
        return NULL;
    size_t len = 0;
    while (str[len] != ' ' && str[len] != '&' && str[len] != '|' && str[len] != '<' && str[len] != '>' && str[len] != ';' && str[len] != '(' && str[len] != ')' && str[len] != '\0')
        len++;
    char * word = (char *)malloc(len + 1);
    for (size_t i = 0; i < len; i++)
        word[i] = str[i];
    word[len] = '\0';
    return word;
}

int proc_simple_com(char * scom, int fd[2])
{
    if (scom[0] == '\0')
        return 0;
    size_t c_arg = 1;
    for (size_t i = 0; scom[i] != '\0'; i++)
    {
        if (scom[i] == ' ')
            c_arg++;
    }    
    char * com = get_word(scom);
    char ** args = (char **)malloc((c_arg + 1) * sizeof(char *));
    size_t ptr = 0;
    for (size_t i = 0; i < c_arg; i++)
    {
        args[i] = get_word(scom + ptr);
        while (scom[ptr] != ' ' && scom[ptr] != '\0')
            ptr++;
        ptr++;
    }
    args[c_arg] = NULL;
    if (fd[0] != 0)
    {
        dup2(fd[0], 0);
        close(fd[0]);
    }
    if (fd[1] != 1)
    {
        dup2(fd[1], 1);
        close(fd[1]);
    }
    execvp(com, args);
}

int proc_conv(char * conv, char * fin_name, char * fout_name, int is_app) //is_app = 1 -> append   is_app = 0 -> write
{
    if (conv[0] == '\0')
        return 0;
    if (strlen(conv) >= 4)
    {
        if (conv[0] == 'c' && conv[1] == 'd' && conv[2] == ' ')
        {
            if (chdir(conv + 3) == -1)
                return  1;
            return 0;
        }
    }
    int err = 0;
    int fdc[2];
    fdc[0] = 0;
    fdc[1] = 1;
    if (fin_name != NULL)
    {
        fdc[0] = open(fin_name, O_RDONLY);
    }
    if (fout_name != NULL)
    {
        if (is_app)
            fdc[1] = open(fout_name, O_APPEND | O_WRONLY | O_CREAT, 0777);
        else
            fdc[1] = open(fout_name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    }
    size_t c_scom = 1;
    for (size_t i = 0; conv[i] != '\0'; i++)
    {
        if (conv[i] == '|')
            c_scom++;
    }
    if (c_scom == 1)
    {
        pid_t pidp = fork();
        if (!pidp)
        {
            signal(SIGINT, SIG_DFL);
            exit(proc_simple_com(conv, fdc));
        }
        curpid = pidp;
        wait(&err);
    }
    else
    {
        pid_t pidp = fork();
        if (!pidp)
        {
            signal(SIGINT, SIG_DFL);
            pid_t * pids = (pid_t *)malloc(c_scom * sizeof(pid_t));
            size_t ptr1 = 0, ptr2 = 0;
            int fd[2];
            int fdp[2];
            fd[0] = fdc[0];
            for (size_t i = 0; i < c_scom - 1; i++)
            {
                pipe(fdp);
                fd[1] = fdp[1];
                while (conv[ptr2] != '|')
                    ptr2++;
                conv[ptr2] = '\0';
                pid_t pid = fork();
                if (!pid)
                    exit(proc_simple_com(conv + ptr1, fd));
                pids[i] = pid;
                close(fd[0]);
                close(fd[1]);
                ptr1 = ptr2 + 1;
                ptr2++;
                fd[0] = fdp[0];
            }
            fd[1] = fdc[0];
            while (conv[ptr2] != '\0')
                ptr2++;
            pid_t pid = fork();
            if (!pid)
                exit(proc_simple_com(conv + ptr1, fd));
            pids[c_scom - 1] = pid;
            close(fd[0]);
            close(fd[1]);
            for (size_t i = 0; i < c_scom; i++)
            {
                int status;
                wait(&status);
                if (status && !err)
                {
                    err = status;
                    for (size_t i = 0; i < c_scom; i++)
                        kill(pids[i], SIGINT);
                }
            }
            free(pids);
            exit(0);
        }
        curpid = pidp;
        wait(&err);
    }
    return err;
}

int proc_com(char * com)
{
    if (com[0] == '(')
    {
        com[strlen(com) - 1] = '\0';
        pid_t pid = fork();
        if (!pid)
            exit(shell(com + 1));
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
    int err = 0, is_app = 0;
    char * fin_name = NULL;
    char * fout_name = NULL;
    size_t ptr1 = 0, ptr2 = 0;
    while (com[ptr2] != '\0' && com[ptr2] != '>' && com[ptr2] != '<')
        ptr2++;
    if (com[ptr2] == '\0')
    {
        err = proc_conv(com, fin_name, fout_name, is_app);
    }
    else if (com[ptr2] == '<')
    {
        ptr1 = ptr2;
        ptr2++;
        fin_name = get_word(com + ptr2);
        while (com[ptr2] != ' ' && com[ptr2] != '>' && com[ptr2] != '\0')
            ptr2++;
        if (com[ptr2] == ' ')
        {
            err = proc_conv(com + ptr2 + 1, fin_name, fout_name, is_app);
        }
        else if (com[ptr2] == '\0')
        {
            com[ptr1] = '\0';
            err = proc_conv(com, fin_name, fout_name, is_app);
        }
        else
        {
            ptr2++;
            if (com[ptr2] == '>')
            {
                is_app = 1;
                ptr2++;
            }
            fout_name = get_word(com + ptr2);
            if (com[0] == '<')
            {
                while (com[ptr2] != ' ' && com[ptr2] != '\0')
                    ptr2++;
                if (com[ptr2] == ' ')
                    ptr2++;
                err = proc_conv(com + ptr2, fin_name, fout_name, is_app);
            }
            else
            {
                com[ptr1] = '\0';
                err = proc_conv(com, fin_name, fout_name, is_app);
            }
        }
    }
    else
    {
        ptr1 = ptr2;
        ptr2++;
        if (com[ptr2] == '>')
        {
            is_app = 1;
            ptr2++;
        }
        fout_name = get_word(com + ptr2);
        while (com[ptr2] != ' ' && com[ptr2] != '<' && com[ptr2] != '\0')
            ptr2++;
        if (com[ptr2] == ' ')
        {
            err = proc_conv(com + ptr2 + 1, fin_name, fout_name, is_app);
        }
        else if (com[ptr2] == '\0')
        {
            com[ptr1] = '\0';
            err = proc_conv(com, fin_name, fout_name, is_app);
        }
        else
        {
            ptr2++;
            fin_name = get_word(com + ptr2);           
            if (com[0] == '>')
            {
                while (com[ptr2] != ' ' && com[ptr2] != '\0')
                    ptr2++;
                if (com[ptr2] == ' ')
                    ptr2++;
                err = proc_conv(com + ptr2, fin_name, fout_name, is_app);
            }
            else
            {
                com[ptr1] = '\0';
                err = proc_conv(com, fin_name, fout_name, is_app);
            }
        }
    }
    if (fin_name != NULL)
        free(fin_name);
    if (fout_name != NULL)
        free(fout_name);
    return err;
}

int proc_cond_exec_com(char * cecom)
{
    int err = 0;
    int last_op = 0; //0 - &&   1 - ||
    size_t ptr1 = 0, ptr2 = 0;
    int brac = 0;
    while (cecom[ptr2] != '\0')
    {
        if (cecom[ptr2] == '(')
            brac++;
        if (cecom[ptr2] == ')')
            brac--;
        if (brac < 0)
            return 1;
        else if (brac > 0)
            ptr2++;
        else
        {
            if (cecom[ptr2] == '&' && cecom[ptr2 + 1] == '&')
            {
                cecom[ptr2] = '\0';
                if (err != 0 && last_op != 0 || err == 0 && last_op == 0)
                {
                    err = proc_com(cecom + ptr1);
                    last_op = 0;
                }
                ptr1 = ptr2 + 2;
                ptr2 += 2;
            }
            else if (cecom[ptr2] == '|' && cecom[ptr2 + 1] == '|')
            {
                cecom[ptr2] = '\0';
                if (err != 0 && last_op != 0 || err == 0 && last_op == 0)
                {
                    err = proc_com(cecom + ptr1);
                    last_op = 1;
                }
                ptr1 = ptr2 + 2;
                ptr2 += 2;
            }
            else
                ptr2++;
        }
    }
    if (brac > 0)
        return 1;
    if (err != 0 && last_op != 0 || err == 0 && last_op == 0)
    {
        err = proc_com(cecom + ptr1);
    }
    return err;
}

int shell(char * input)
{
    int err = 0;
    size_t ptr1 = 0, ptr2 = 0;
    int brac = 0;
    while (input[ptr2] != '\0')
    {
        if (input[ptr2] == '(')
            brac++;
        if (input[ptr2] == ')')
            brac--;
        if (brac < 0)
            return 1;
        else if (brac > 0)
            ptr2++;
        else
        {
            if (input[ptr2] == ';')
            {
                input[ptr2] = '\0';
                int status = proc_cond_exec_com(input + ptr1);
                if (status)
                    err = status;
                ptr1 = ptr2 + 1;
                ptr2++;
            }
            else if (input[ptr2] == '&')
            {
                if (input[ptr2 + 1] == '&')
                {
                    ptr2 += 2;
                    continue;
                }
                input[ptr2] = '\0';
                
                pid_t pids = fork();
                if (!pids)
                {
                    signal(SIGINT, SIG_IGN);
                    pid_t pidgs = fork();
                    if (!pidgs)
                    {   
                        int fd = open("/dev/null", O_RDONLY);
                        dup2(fd, 0);
                        signal(SIGINT, SIG_IGN);
                        err = proc_cond_exec_com(input + ptr1);
                        close(fd);
                        exit(err);
                    }
                    exit(0);
                }
                waitpid(pids, NULL, 0);
                ptr1 = ptr2 + 1;
                ptr2++;
            }
            else
                ptr2++;
        }
    }
    if (brac > 0)
        return 1;
    err = proc_cond_exec_com(input + ptr1);
    return err;
}
