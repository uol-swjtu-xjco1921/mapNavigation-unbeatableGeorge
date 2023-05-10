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
#define ERROR_BAD_MAGIC_NUMBER 3
#define ERROR_EDIT_FAILED 4
#define ERROR_BAD_DIMENSIONS 5
#define NULL_POINTER 6
#define ERROR_IMAGE_MALLOC_FAILED 7
#define ERROR_BAD_DATA_READ 8
#define ERROR_OUTPUT_FAILED 9
#define ERROR_BAD_LAYOUT 10
#define ERROR_MISCELLANEOUS 100
#define ERROR_UNKNOWN 12
#define NUM_ERROR_STRINGS 13

/* end of include guard			   */
#endif /*PGM_ERROR_H*/
