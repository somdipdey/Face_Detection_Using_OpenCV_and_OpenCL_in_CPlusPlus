//
//  execConcurrent.cpp
//  
//
//  Copyright @ Somdip Dey, 2018.
//

#include "execConcurrent.hpp"

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
// for popen()
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
// for boost split
#include <boost/algorithm/string.hpp>

#define NUM_THREADS 3 //read in number of threads to spawn instead

// for popen executing command
std::string exec(const char* cmd) {
    std::array<char, 256> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 256, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

//execute the command itself
void *execCmd(void *args)
{
    //long tid;
    int exit_code;

    std::string *affinityS = static_cast<std::string*>(args);
    std::string affinity = *affinityS;
    //delete affinityS;
    
    // fetch affinity and unique id for audit
    // split the arguments
    std::vector<std::string> split_args;
    boost::split(split_args,affinity,boost::is_any_of(":"));
    std::string cmd("./faces 0 out.txt ");
    cmd += split_args[1];
    
    // Execute command
    exec(cmd.c_str());
    //system("PAUSE");

    pthread_exit(NULL);
}


int main ()
{
    // how many threads to spwan
    //printf("How many concurrent commands to execute?\n");
    //std::string numThread;
    //std::cin >> numThread;
    //int NUM_THREADS = atoi(numThread.c_str());
    
    // threads
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;
    
    // read the cpu affinities
    printf("Please enter the CPU affinities and Unique ID: \n");
    std::string option;
    std::cin >> option;
    printf("Argument passed: %s", option.c_str());
    
    // split the arguments
    std::vector<std::string> split_strings;
    boost::split(split_strings,option,boost::is_any_of(";"));
    
    for (size_t i = 0; i < split_strings.size(); i++)
    {
        std::cout << "\nFor CMD " << std::to_string(i+1) << ": " << split_strings[i] << "\n" << std::endl;
        
        std::cout << "-- execConcurrent() : creating thread, " << i << "\n\n" << std::endl;
        
        rc = pthread_create(&threads[i], NULL, execCmd, (void *)split_strings[i].c_str());
        
        if (rc) {
            std::cout << "-- Error:unable to create thread," << rc << std::endl;
            exit(-1);
        }
    }

    pthread_exit(NULL);
    
    return 0;
}

