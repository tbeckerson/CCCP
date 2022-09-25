#include "stdio.h"

#include "../include/globals.h"
#include "../include/libspm.h"
#include "../include/spm/spm.h"
#include "../include/data.h"
#include "../include/utils.h"
#include "../include/ecmp/ecmp.h"
#include "../include/libspm.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



char* names[5] = {"pkg1","pkg2","pkg3","pkg4","pkg5"};
char* versions[5] = {"1.1.0","2.0.8","6.8.7","7.0","5.20"};
char* types[5] = {"bin","src","src","bin","src"};

char* locs[10] = {"b","b/d/e","a","d","b/d","b/c","b/f"};

int test_spm();
int test_data ();
int test_ecmp();
int test_move();

char* assemble(char** list,int count);

int main(int argc, char const *argv[])
{

    if (argc  < 2)
    {
        printf("No arguments provided\n");
        return 1;
    }

    DEBUG = 3;
    QUIET = false;
    OVERWRITE = true;

    init();

    if (strcmp(argv[1],"spm") == 0)
    {
        return test_spm();
    }
    else if (strcmp(argv[1],"data") == 0)
    {
        return test_data();
    }
    else if (strcmp(argv[1],"ecmp") == 0)
    {
        return test_ecmp();
    }
    else if (strcmp(argv[1],"all") == 0)
    {
        int ret = 0;
        ret += test_spm();
        ret += test_data();
        ret += test_ecmp();
        return ret;
    }
    else if (strcmp(argv[1],"install") == 0)
    {
        installSpmFile(argv[2],0);
        return 0;
    }
    else if (strcmp(argv[1],"move") == 0)
    {
        return test_move();
    }
    else if (strcmp(argv[1],"help") == 0)
    {
        printf("Usage: test [spm|data|ecmp|all|help|install]\n");
        return 0;
    }
    else
    {
        printf("Invalid argument\n");
        return 1;
    }


}

int test_move()
{
    ROOT = "tests/dest";
    BUILD_DIR = "tests/locs";

    move_binaries(locs, 7);

    return 0;
}

int test_split()
{
    char* split_str = "Hello,World,This,is,a,test";

    printf("cutils test\n");

    printf("Testing split\n");
    int count;
    char** split_list = split(split_str,',',&count);

        // print list
    printf("split : printing list\n");
    for (int i = 0; i < count; i++)
    {
        printf("  %s\n",split_list[i]);
    }
    return 0;
}

int test_spm()
{
    int EXIT = 0;

    struct package t_pkg;

    EXIT += open_pkg("tests/vim.spm", &t_pkg);

    printf("Creating spm package file \n");
    // create new spm
    EXIT += create_pkg("tests/vim-test.spm",&t_pkg);


    return EXIT;
}
int test_data ()
{
    int EXIT = EXIT_SUCCESS;


    printf("Removing test.db\n");
    remove(ALL_DB);

    printf("Creating test database...\n");

    init_data(ALL_DB);
    
    printf("Adding data to test database...\n");
    for (int i = 0; i < 5; i++)
    {
        struct package a_pkg;
        a_pkg.name = names[i];
        a_pkg.version = versions[i];
        a_pkg.type = types[i];
        store_data(ALL_DB,&a_pkg,0);
    }
    
    printf("Print all data from test database...\n");
    get_all_data(ALL_DB);
    printf("Checking data in test database...\n");
    for (int i = 0; i < 5; i++)
    {
        struct package a_pkg;
        a_pkg.name = names[i];
        printf("Checking %s...\n",a_pkg.name);
        find_data(ALL_DB,&a_pkg);
        printf("  %s => %s %s\n",a_pkg.name,a_pkg.version,a_pkg.type);
        if (strcmp(a_pkg.version,versions[i]) != 0 | strcmp(a_pkg.type,types[i]) !=0 )
        {
            msg(ERROR,"Invalid return values , database check failed");
            EXIT = -1;
        }
        
    }
    printf("Removing data from test database...\n");
    for (int i = 0; i < 5; i++)
    {
        printf("Removing %s\n",names[i]);
        remove_data(ALL_DB,names[i]);
    }
    

    printf("Checking data in test database...\n");
    for (int i = 0; i < 5; i++)
    {
        struct package a_pkg;
        a_pkg.name = names[i];
        a_pkg.version = NULL;
        a_pkg.type = NULL;
        find_data(ALL_DB,&a_pkg);
        printf("  %s => %s %s\n",a_pkg.name,a_pkg.version,a_pkg.type);
        if (a_pkg.type != NULL | a_pkg.version != NULL)
        {
            msg(ERROR,"Database supression failed ");
            EXIT = -1;
        }
    }
    printf("listing all data from test database...\n");
    get_all_data(ALL_DB);

    return EXIT;
}

int test_ecmp(int type)
{
    int EXIT = EXIT_SUCCESS;

    printf("Testing ecmp functions\n");

    struct package pkg;

    open_pkg("tests/vim.ecmp",&pkg);
    
    // print the pkg
    printf("  %s => %s %s\n",pkg.name,pkg.version,pkg.type);

    // print package info
    printf("install script : \n");
    printf("  %s\n",pkg.info.install);
    

    // print dependencies
    printf("dependencies : \n");
    for (int i = 0; i < pkg.dependenciesCount; i++)
    {
        printf("  %s\n",pkg.dependencies[i]);
    }

    msg(INFO,"Creating ecmp package file");

    create_pkg("tests/vim-test.ecmp", &pkg);

    return EXIT;
}

char* assemble(char** list,int count)
{
    char* string = calloc(256,sizeof(char));
    int i;
    for (i = 0; i < count-1; i++)
    {
        strcat(string,list[i]);
        strcat(string,",");
    }
    strcat(string,list[i]);
    return string;
}