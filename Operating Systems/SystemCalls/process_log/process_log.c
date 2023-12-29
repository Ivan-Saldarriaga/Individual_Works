//define functions!
#include <unistd.h>
#include <stdlib.h>
#include "process_log.h"
int get_proc_log_level(){
	int output = syscall(435);
	return output;
}

int set_proc_log_level(int new_level){
	int output = syscall(436, new_level);
	return output;
}
int proc_log_message(int level, char *message){
	int output = syscall(437, message, level);
	return output;
}
//harness functions
int* retrieve_set_level_params(int new_level){
	//create an integer pointer to return
	//call #, # params, param therefore of size 3
	int* returnParameters = (int*) malloc(3*sizeof(int));
	//add data
	returnParameters[0] = 436;
	returnParameters[1] = 1;
	returnParameters[2] = new_level;
	return returnParameters;
}
int* retrieve_get_level_params(){
	//call #, params, therefore of size 2
	int* returnParameters = (int*) malloc(2*sizeof(int));
	returnParameters[0] = 435;
	returnParameters[1] = 0;
	return returnParameters;
}
int interpret_set_level_result(int ret_value){
	return ret_value;
}
int interpret_get_level_result(int ret_value){
	return ret_value;
}
int interpret_log_message_result(int ret_value){
	return ret_value;
}
