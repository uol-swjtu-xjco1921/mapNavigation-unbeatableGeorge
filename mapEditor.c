// This program includes header files for standard input/output streams, 
// commonly used utility libraries, and custom implementation files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <ctype.h>

#include "hashTable.h" // Header file for hash table implementation
#include "mapAdjMatrix.h" // Header file for adjacency matrix graph representation
#include "mapEditor.h" // Header file for editing map data
#include "mapError.h" // Header file for error handling in map operations.


int ask_modify_map(AdjacencyMatrix* adj_matrix)
{
    // Variable to store whether the map was modified or not
    int modify = 0;
    // Buffer to store user input
    char line[100];
    // Print current map information to the console
    printf("\nCurrent map has %d points and %d edges, and has been edited %d times.\n",
        adj_matrix->num_points, adj_matrix->num_edges, adj_matrix->num_edit);

    while (1)
    {
        // Ask user if they want to modify the map
        printf("\nDo you want to modify the map properties?\n");
        printf("1. Yes\n");
        printf("2. No\n");
        printf("Enter your choice(1 or 2): ");

        // Read user input
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("Failed to read user input.\n");
            continue;
        }
        // Remove newline character from input
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }

        // Compare input to available options and call edit_map function if user chooses to modify map
        if (strcmp(line, "1") == 0)
        {
            if (edit_map(adj_matrix) == EDIT_SUCCESSFULLY)
            {
                modify = 1;
                adj_matrix->num_edit++;
            }
        }
        // Exit loop if user chooses not to modify map
        else if (strcmp(line, "2") == 0)
        {
            break;
        }
        // Print error message if user input is not valid
        else
        {
            printf("Invalid choice (please enter 1 or 2).\n");
        }
    }

    // If map was modified, prompt user for filename and write map to file
    if (modify)
    {
        while (1)
        {
            // Prompt user for filename
            printf("Please enter the map filename(be written): ");
            char* filename = (char*)malloc(100 * sizeof(char));
            if (fgets(filename, 100, stdin) == NULL)
            {
                // If reading fails, continue to prompt the user
                printf("Failed to read user input, please try again\n");
                free(filename);
                continue;
            }
            int len = strlen(filename);
            if (len == 100)
            {
                printf("File name too long! Retype!\n");
                free(filename);
                continue;
            }
            // Remove newline character from input
            if (filename[len - 1] == '\n')
            {
                filename[len - 1] = '\0';
            }

            // Write map to file
            int result = write_map(adj_matrix, filename);
            if (result != FUNCTION_SUCCESS)
            {
                printf("Write map failed!");
                free_adjacency_matrix(adj_matrix);
                exit(ERROR_BAD_FILE);
            }
                
            break;
        }

        // Print number of modifications made to map
        printf("\nNumber of modifications: %d\n", adj_matrix->num_edit);
    }

    return modify;
}


int edit_map(AdjacencyMatrix* adj_matrix)
{
    // line to store message readed
    char line[100];

    // operation type, 1 for add point, 2 for add edge, 3 for edit edge
    int operate_type;
    printf("\nPlease choose the operation type:\n");
    printf("1. Add Point\n");
    printf("2. Add Edge\n");
    printf("3. Edit Edge\n");
    printf("Enter your choice: ");


    while (1)
    {
        // read a line of data
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("read failed, re-enter\n");
            continue;
        }

        // remove the newline character at the end of the line
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }

        // compare and get the value of the selection
        if (strcmp(line, "1") == 0)
        {
            operate_type = 1;
            break;
        }
        else if (strcmp(line, "2") == 0)
        {
            operate_type = 2;
            break;
        }
        else if (strcmp(line, "3") == 0)
        {
            operate_type = 3;
            break;
        }
        else
        {
            printf("You did not enter a correct option (1 or 2 or 3).\n");
            printf("Please re-enter: ");
        }
    }

    // initialize poi
    int len_poi = 0; // record the number of pois
    int pois[20] = { -1 }; // record the value of pois
    switch (operate_type)
    {
    case 1:
        printf("\nPlease enter id latitude longitude: \n");

        // output id latitude longitude and judge if they are correct
        long long point_id = get_input_ll("Input the node id: ");
        double lat = get_input_double("Input the latitude of node: ");
        double lon = get_input_double("Input the longitude of node: ");

        // check if the point exists
        int result = is_point_exist(adj_matrix, point_id, lat, lon, 1);
        if (result == EXIST)
        {
            // The point already exists
            printf("Editing failure: The point ID %lld has existed in map.\n", point_id);
            return ERROR_EDIT_FAILED;
        }

        // bounding
        if (lat > adj_matrix->bounding.maxLat ||
            lat < adj_matrix->bounding.minLat ||
            lon > adj_matrix->bounding.maxLon ||
            lon < adj_matrix->bounding.minLon)
        {
            printf("Editing failure: point out of bounding.(Valid: lat(%lf-%lf) lon(%lf-%lf))",
                adj_matrix->bounding.minLat, adj_matrix->bounding.maxLat, adj_matrix->bounding.minLon, adj_matrix->bounding.maxLon);
            return ERROR_EDIT_FAILED;
        }
        // add the point to the adjacency matrix and check if it was added successfully
        add_point_to_adjacency_matrix(adj_matrix, point_id, lat, lon);
        printf("New point added successfully, id = %lld, latitude = %lf, longitude = %lf\n\n", point_id, lat, lon);
        break;

    case 2:
        printf("Please enter the link id, start node, end node, length, and other information:\n");
        // “¿¥Œ ‰»Î link_id, node_id_from, node_id_to, way, length, veg, arch, land, POI
        long long link_id = get_input_ll("Input the link ID you want to add: ");
        long long node_id_from = get_input_ll("Input the First Node of the edge: ");
        long long node_id_to = get_input_ll("Input the Second Node of the edge: ");
        // determine whether the points are legal
        if (is_node_exist(adj_matrix, node_id_from) == NOT_EXIST)
        {   // determine if the point exists
            printf("Editing failure: The first node_id %lld is not exist!\n", node_id_from);
            return ERROR_EDIT_FAILED;
        }
        if (is_node_exist(adj_matrix, node_id_to) == NOT_EXIST)
        {   // determine if the point exists
            printf("Editing failure: The second node_id %lld is not exist!\n", node_id_to);
            return ERROR_EDIT_FAILED;
        }
        if (is_edge_exist(adj_matrix, link_id, node_id_from, node_id_to, 1) == EXIST)
        {   // determine if the link already exists (by id or by link)
            return ERROR_EDIT_FAILED;
        }
        if (node_id_from == node_id_to)
        {   // invalid input
            printf("\nEditing failure: You are entering the self-loop edge!\n");
            return ERROR_EDIT_FAILED;
        }

        double length = get_input_double("Input the Length of the edge: ");
        double veg = get_input_double("Input the Greening Degree of the edge(if confused type 0.0 is OK): ");
        double arch = get_input_double("Input the Architectural Information of the edge(if confused type 0.0 is OK): ");
        double land = get_input_double("Input the land Utilization of the edge(if confused type 0.0 is OK): ");
        double speedLimit = get_input_double("Input the Speed Limit of the edge: ");

        if (length <= 0 || veg < 0 || arch < 0 || land < 0 || speedLimit <= 0)
        {   // invalid input
            printf("Editing failure: an invalid input occurs\n");
            return ERROR_EDIT_FAILED;
        }

        // get poi
        int res = get_pois_from_input(&len_poi, pois);
        if (res != FUNCTION_SUCCESS) {
            printf("Failed to get pois from input.\n");
        }

        // start adding edges
        add_edge_to_adjacency_matrix(adj_matrix, link_id, node_id_from, node_id_to, length, veg, arch, land, speedLimit, pois, len_poi);
        printf("Successfully added new edge, id = %lld, node_1 <=> node_2 = %lld <=> %lld, length = %lf, speedLimit = %lf\n", link_id, node_id_from, node_id_to, length, speedLimit);
        printf("veg = %lf, arch = %lf, land = %lf, number of POIs added = %d\n", veg, arch, land, len_poi);
        if (len_poi != 0)
        {
            printf("The POIs are: ");
            for (int i = 0; i < len_poi; i++)
            {
                printf("%d ", pois[i]);
            }
        }
        printf("\n");
        break;

    case 3:

        printf("Please enter the id of the link you want to edit:\n");
        link_id = get_input_ll("Enter the link id: ");
        if (is_edge_id_exist(adj_matrix, link_id) == NOT_EXIST)
        {
            // determine whether the edge link_id already exists
            printf("Editing failure: The link_id %lld is not existed! \n", link_id);
            return ERROR_EDIT_FAILED;
        }
        node_id_from = get_input_ll("Input the First Node of the edge: ");
        node_id_to = get_input_ll("Input the Second Node of the edge: ");
        length = get_input_double("Input the Length of the edge: ");
        veg = get_input_double("Input the Greening Degree of the edge(if confused type 0.0 is OK): ");
        arch = get_input_double("Input the Architectural Information of the edge(if confused type 0.0 is OK): ");
        land = get_input_double("Input the land Utilization of the edge(if confused type 0.0 is OK): ");
        speedLimit = get_input_double("Input the Speed Limit of the edge: ");

        // determines whether the input is correct
        if (is_node_exist(adj_matrix, node_id_from) == NOT_EXIST)
        {   // determine if the point exists
            printf("Editing failure: The first node_id %lld is not exist!\n", node_id_from);
            return ERROR_EDIT_FAILED;
        }
        if (is_node_exist(adj_matrix, node_id_to) == NOT_EXIST)
        {   // determine if the point exists
            printf("Editing failure: The second node_id %lld is not exist!\n", node_id_to);
            return ERROR_EDIT_FAILED;
        }
        if (is_edge_nodes_exist(adj_matrix, link_id, node_id_from, node_id_to) == EXIST)
        {   // determine if the link already exists (by id or by link)
            printf("Editing failure: The link with node_1 <=> node_2 = %lld <=> %lld is existed in this map!!\n", node_id_from, node_id_to);
            return ERROR_EDIT_FAILED;
        }
        if (node_id_from == node_id_to)
        {   // self-loop edge
            printf("\nEditing failure: You are entering the self-loop edge!\n");
            return ERROR_EDIT_FAILED;
        }

        if (length <= 0 || veg < 0 || arch < 0 || land < 0 || speedLimit <= 0)
        {   // invalid input
            printf("Editing failure: an invalid input occurs\n");
            return ERROR_EDIT_FAILED;
        }

        // get poi
        res = get_pois_from_input(&len_poi, pois);
        if (res != FUNCTION_SUCCESS) {
            printf("Failed to get pois from input.\n");
        }

        // start editing edge
        edit_edge(adj_matrix, link_id, node_id_from, node_id_to, length, veg, arch, land, speedLimit, pois, len_poi);
        printf("The edge with id %lld and nodes %lld <=> %lld has been edited.", link_id, node_id_from, node_id_to);
        printf("veg = %lf, arch = %lf, land = %lf, number of POIs added = %d\n", veg, arch, land, len_poi);
        if (len_poi != 0)
        {
            printf("The POIs are: ");
            for (int i = 0; i < len_poi; i++)
            {
                printf("%d ", pois[i]);
            }
        }
        printf("\n");
        break;

    default:
        printf("Error edit operation");
        return ERROR_EDIT_FAILED;
    }

    return EDIT_SUCCESSFULLY;
}


// Function to edit an edge in the adjacency matrix
int edit_edge(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to,
    double length, double veg, double arch, double land, double speedLimit, int POIs[], int len_pois)
{
    // Find the position of the edge in the adjacency matrix
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        // Check for null pointers
        if (edge == NULL)
        {   // free memory and exit with NULL_POINTER
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        // Modify the contents of the edge
        if (edge->id == link_id)
        {
            edge->from = node_id_from;
            edge->to = node_id_to;
            edge->length = length;
            edge->veg = veg;
            edge->arch = arch;
            edge->land = land;
            edge->speedLimit = speedLimit;
            edge->num_pois = len_pois;
            edge->pois = realloc(edge->pois, edge->num_pois * sizeof(int));
            adj_matrix->adj[hashtable_lookup(adj_matrix->point_hash, node_id_from)][hashtable_lookup(adj_matrix->point_hash, node_id_to)] = length;
            for (int j = 0; j < edge->num_pois; j++)
            {
                edge->pois[j] = POIs[j];
            }
            return EDIT_SUCCESSFULLY;
        }
    }
    return ERROR_EDIT_FAILED;
}



int get_pois_from_input(int* len_poi, int pois[]) 
{
    int repetition = 0;
    // init poi
    *len_poi = 0; // count the num of poi
    memset(pois, -1, 20 * sizeof(int)); // initialize all elements in the pois array to -1
    // whether this link has POI property
    printf("is this link has POIs ?\n");
    printf("1. yes\n");
    printf("2. no\n");
    printf("choose an option (1 or 2): ");
    char buf[100];
    // read data line by line
    if (fgets(buf, sizeof(buf), stdin) == NULL)
    {
        printf("Editing failure: invalid input!\n");
        return ERROR_EDIT_FAILED;
    }
    // remove the trailing newline character
    if (buf[strlen(buf) - 1] == '\n')
    {
        buf[strlen(buf) - 1] = '\0';
    }
    if (strcmp(buf, "1") == 0)
    {   // has poi property
        printf("You can input POIs(>0) of this edge! when you type -1, input end!\n");
        while (1)
        {
            int poi = get_input_int("Input a POI of this edge: ");
            if (poi == -1)
            {
                break;
            }
            pois[*len_poi] = poi;
            (*len_poi)++;
        }
        // check if the user inputs duplicate poi values
        for (int i = 0; i < *len_poi; i++)
        {
            for (int j = i + 1; j < *len_poi; j++)
            {
                if (pois[i] == pois[j] && pois[i] != -1)
                {
                    printf("%d is duplicated and is only recorded once\n", pois[j]);
                    // mark the duplicate element as -1 and count the repetition
                    pois[j] = -1;
                    repetition++;
                }
            }
        }
        // create a new array new_poi[]
        int new_poi[20] = { -1 };
        if (repetition != 0) 
        { // there are duplicate elements
            int new_len = 0;
            for (int i = 0; i < *len_poi; i++) 
            {
                if (pois[i] != -1) 
                { // not a duplicate element
                    int is_duplicate = 0;
                    for (int j = 0; j < new_len; j++) 
                    {
                        if (pois[i] == new_poi[j]) 
                        { // already exists
                            is_duplicate = 1;
                            break;
                        }
                    }
                    if (!is_duplicate) 
                    { // not exists, add to the new array
                        new_poi[new_len] = pois[i];
                        new_len++;
                    }
                }
            }
            // assign the new array back to the old array and update the length
            for (int i = 0; i < new_len; i++)
            {
                pois[i] = new_poi[i];
            }
            *len_poi = new_len;
        }
    }
    else if (strcmp(buf, "2") == 0) {}
    else
    {
        printf("Editing failure: invalid input!\n");
        return ERROR_EDIT_FAILED;
    }
    return FUNCTION_SUCCESS;
}


int is_point_exist(AdjacencyMatrix* adj_matrix, long long point_id, double lat, double lon, int flag)
{
    // Check if the point already exists in the adjacency matrix.
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        Point* point = &adj_matrix->points[i];
        
        // Check for null pointer.
        if (point == NULL)
        {
            // Free memory and exit with NULL_POINTER.
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        // Check if the point already exists in the adjacency matrix by comparing its ID or latitude and longitude.
        if (point->id == point_id || (point->lat == lat && point->lon == lon))
        {
            if (flag)
            {
                printf("The point already exists in the adjacency matrix. Its information in the map is id = %lld, latitude = %lf, longitude = %lf. \n", point->id, point->lat, point->lon);
            }
            return EXIST;
        }
    }

    return NOT_EXIST;
}


int is_edge_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to, int flag)
{
    // Check if the edge already exists in the adjacency matrix.
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        
        // Check for null pointer.
        if (edge == NULL)
        {
            // Free memoryand exit with NULL_POINTER.
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        // Check if the edge already exists in the adjacency matrix by comparing its ID or two nodes' IDs.
        if (edge->id == link_id || (edge->from == node_id_from && edge->to == node_id_to))
        {
            if (flag)
            {
                printf("Editing failure: The edge already exists in the adjacency matrix. Its information in the map is id = %lld, node_id_1 = %lld, node_id_2 = %lld. \n", edge->id, edge->from, edge->to);
            }
            return EXIST;
        }
    }

    // Edge does not exist in the matrix
    return NOT_EXIST;
}


int is_edge_id_exist(AdjacencyMatrix* adj_matrix, long long link_id)
{
    // Check if the edge already exists in the adjacency matrix.
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        if (edge->id == link_id)
        {
            return EXIST;
        }
    }

    return NOT_EXIST;
}


int is_edge_nodes_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to)
{
    // Check if the edge already exists in the adjacency matrix.
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        if (edge->from == node_id_from && edge->to == node_id_to && link_id != edge->id)
        {
            return EXIST;
        }
    }

    return NOT_EXIST;
}


int is_node_exist(AdjacencyMatrix* adj_matrix, long long node_id)
{
    // Check if the nodes already exists in the adjacency matrix.
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        Point* point = &adj_matrix->points[i];
        // Check for null pointer.
        if (point == NULL)
        {
            // free memory and exit with NULL_POINTER
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        if (point->id == node_id)
        {
            return EXIST;
        }
    }
    return NOT_EXIST;
}


int is_POI_exist(AdjacencyMatrix* adj_matrix, int POI)
{
    // find POI in edges
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        for (int j = 0; j < adj_matrix->edges[i].num_pois; j++)
        {
            if (adj_matrix->edges[i].pois[j] == POI)
            {
                return EXIST;
            }
        }
    }
    return NOT_EXIST;
}


int get_input_int(const char* prompt)
{
    char buf[1024];
    int result;

    while (1) {
        printf("%s", prompt);

        // Read input string from the user.
        if (fgets(buf, sizeof(buf), stdin) == NULL) 
        {
            printf("Input error! Please enter a valid integer.\n");
            continue;
        }

        // Remove trailing newline character.
        int len = strlen(buf);
        if (buf[len - 1] == '\n') 
        {
            buf[len - 1] = '\0';
        }

        // Check if the input string contains only valid integer characters.
        if (!check_lld(buf)) 
        {
            printf("Input error! Please enter a valid integer.\n");
            continue;
        }

        // Convert the input string to integer.
        if (sscanf(buf, "%d", &result) != 1)
            continue;

        break;
    }

    return result;
}


long long get_input_ll(const char* prompt) 
{
    char buf[1024];
    long long result;

    while (1) {
        printf("%s", prompt);

        // Read input string from the user.
        if (fgets(buf, sizeof(buf), stdin) == NULL) 
        {
            printf("Input error! Please enter a valid integer.\n");
            continue;
        }

        // Remove trailing newline character.
        int len = strlen(buf);
        if (buf[len - 1] == '\n') 
        {
            buf[len - 1] = '\0';
        }

        // Check if the input string contains only valid integer characters.
        if (!check_lld(buf)) 
        {
            printf("Input error! Please enter a valid integer.\n");
            continue;
        }

        // Convert the input string to integer.
        if (sscanf(buf, "%lld", &result) != 1)
            continue;

        break;
    }

    return result;
}


double get_input_double(const char* prompt) 
{
    char buf[1024];
    double result;

    while (1) {
        printf("%s", prompt);

        // Read input string from the user.
        if (fgets(buf, sizeof(buf), stdin) == NULL) 
        {
            printf("Input error! Please enter a valid integer.\n");
            continue;
        }

        // Remove trailing newline character.
        int len = strlen(buf);
        if (buf[len - 1] == '\n') 
        {
            buf[len - 1] = '\0';
        }

        // Check if the input string contains only valid integer characters.
        if (!check_double(buf)) 
        {
            printf("Input error! Please enter a valid integer.\n");
            continue;
        }

        // Convert the input string to integer.
        if (sscanf(buf, "%lf", &result) != 1)
            continue;

        break;
    }

    return result;
}


// cheack a valid input long long
int check_lld(char* s)
{
    int max_len = MAX_LONG_LONG;
    // check NULL
    if (s == NULL || *s == '\0')
    {
        return 0;
    }
    int n = 0;
    if (*s == '+' || *s == '-')
    {
        s++;
        n++;
    }
    while (*s != '\0')
    {
        if (!isdigit(*s))
        {
            return 0;
        }
        s++;
        n++;
        if (n > max_len)
        {
            return 0;
        }
    }
    return 1;
}

// cheack a valid input double
int check_double(const char* s)
{
    int max_len = MAX_DOUBLE;
    // check NULL
    if (s == NULL || *s == '\0')
    {
        return 0;
    }
    int n = 0;
    if (*s == '+' || *s == '-')
    {
        s++;
        n++;
    }
    int dot_seen = 0;
    while (*s != '\0')
    {
        if (*s == '.')
        {
            if (dot_seen)
            {
                return 0;
            }
            else {
                dot_seen = 1;
            }
        }
        else if (!isdigit(*s))
        {
            return 0;
        }
        s++;
        n++;
        if (n > max_len)
        {
            return 0;
        }
    }
    return 1;
}


// Write the adjacency matrix data to a text file with the given name.
int write_map(AdjacencyMatrix* adj_matrix, char* new_map_name)
{
    FILE* fp; // Define file pointer.
    fp = fopen(new_map_name, "w"); // Open the file in write mode.
    if (fp == NULL) 
    { // If file opening failed.
        printf("Failed to open file '%s' for writing!\n", new_map_name);
        return ERROR_BAD_FILE;
    }
    // Write bounding to file.
    fprintf(fp, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>\n", 
        adj_matrix->bounding.minLat, adj_matrix->bounding.minLon,
        adj_matrix->bounding.maxLat, adj_matrix->bounding.maxLon);
   
    // Write link data to file.
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        fprintf(fp, "<link id=%lld node=%lld node=%lld way=-7716 length=%lf veg=%lf arch=%lf land=%lf speedLimit=%lf",
            adj_matrix->edges[i].id,
            adj_matrix->edges[i].from,
            adj_matrix->edges[i].to,
            adj_matrix->edges[i].length,
            adj_matrix->edges[i].veg,
            adj_matrix->edges[i].arch,
            adj_matrix->edges[i].land,
            adj_matrix->edges[i].speedLimit);

        fprintf(fp, "POI=");
        for (int j = 0; j < adj_matrix->edges[i].num_pois; j++)
        {
            fprintf(fp, "%d,", adj_matrix->edges[i].pois[j]);
        }
        fprintf(fp, ";/link>\n");
    }

    // Write node data to file.
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        fprintf(fp, "<node id=%lld lat=%lf lon=%lf /node>\n",
            adj_matrix->points[i].id,
            adj_matrix->points[i].lat,
            adj_matrix->points[i].lon);
    }
    
    // Write geom data to file.
    for (int i = 0; i < adj_matrix->num_geoms; i++)
    {
        fprintf(fp, "<geom id=%lld ", adj_matrix->geoms[i].id);
        for (int j = 0; j < adj_matrix->geoms[i].len_nodes; j++)
        {
            fprintf(fp, "node=%lld ", adj_matrix->points[adj_matrix->geoms[i].nodes[j]].id);
        }
        fprintf(fp, "/geom>\n");
    }

    // Close the file.
    fclose(fp);
    printf("File '%s' written successfully.\n", new_map_name);
    return FUNCTION_SUCCESS;
}
