/***********************************/
/* A shared header file for error  */
/* message						   */
/***********************************/

/* include guard				   */
#ifndef PGM_ERROR_H
#define PGM_ERROR_H

/* macro constant for success	   */
#define PROGRAMME_SUCCESS 0
#define FUNCTION_SUCCESS 0

/* macro constant for errors	   */
#define ERROR_BAD_ARGUMENT_COUNT 1
#define ERROR_BAD_FILE 2
#define ERROR_DATA_READ_FAILED 3
#define ERROR_EDIT_FAILED 4
#define ERROR_FIND_PATH_FAILED 5
#define ERROR_VISULIZE_FAILED 6
#define ERROR_HASH_ADD 7
#define ERROR_HASH_LOOK_UP 8
#define ERROR_MALLOC_FAILED 9
#define NULL_POINTER 10

/* end of include guard			   */
#endif /*PGM_ERROR_H*/
