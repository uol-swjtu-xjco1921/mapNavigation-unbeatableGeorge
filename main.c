#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjMatrix.h"
#include "mapReader.h"
#include "mapEditor.h"
#include "pathFinder.h"

/***********************************/
/* XJCO 1921M Programming Project  */
/* 2022-23 Spring Semester         */
/*                                 */
/* Version: Store data in adj matrix */
/* Yifan Xiang					   */
/***********************************/

/***********************************/
/* Main Routine                    */
/***********************************/

int main()
{
    AdjacencyMatrix adj_matrix; // Define an adjacency matrix variable

    init_adjacency_matrix(&adj_matrix); // Initialize the adjacency matrix

    int result = read_map(&adj_matrix);

    result = ask_modify_map(&adj_matrix);    // modify_map
    if (result == 1)
    {
        printf("\nÐÞ¸Ä´ÎÊý %d\n", adj_matrix.num_edit);
    }

    result = ask_find_path(&adj_matrix);
    return 0;

    return 0;

}