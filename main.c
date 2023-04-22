#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjList.h"
#include "mapReader.h"

/****************************************/
/* Version: Store data in adj list */
/* Author: Yifan Xiang		    */
/****************************************/

/****************************************/
/* Main Routine                    */
/****************************************/

int main()
{
    AdjacencyList adj_list; // Define an adjacency list variable
    init_adjacency_list(&adj_list); // Initialize the adjacency list

    read_map(&adj_list);

    return 0;
}
