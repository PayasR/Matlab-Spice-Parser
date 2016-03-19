/**
	Symbol_Table.c
	For the undergraduate EDA course offered at 
	Dept of Micro/Nano-electronics

	(c) Guoyong Shi, 
	All rights reserved, 2006-2015
	Shanghai Jiao Tong University

	If you modify the code, please keep the above copyright info.
*/

#ifdef MATLAB
#include "mex.h"
#endif

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "Symbol_Table.h"

// Electric type of a node variable
#define SP_NOTYPE 	0
#define SP_VOLTAGE 	1
#define SP_CURRENT 	2

#define LINE_INFO   "\n[" << __FILE__ << " : line " << __LINE__ << " : " << __FUNCTION__ << "()] - "
#define PRINT_LINE_INFO 	printf("\n[%s : line %d : %s()]\n", __FILE__, __LINE__, __FUNCTION__);


Node_Entry **NodeTable;
int NodeTableSize;

Device_Entry **DeviceTable;
int DeviceTableSize;

Index_Entry **IndexTable;
int IndexTableSize;

void Init_Symbol_Tables()
{
    // Using dynamically adjusting sizes of linked lists.

    NodeTable = malloc(sizeof(Node_Entry*));
    DeviceTable = malloc(sizeof(Device_Entry*));
    IndexTable = malloc(sizeof(Index_Entry*));

    memset(NodeTable, 0, sizeof(Node_Entry*));
    memset(DeviceTable, 0, sizeof(Device_Entry*));
    memset(IndexTable, 0, sizeof(Index_Entry*));

    NodeTableSize = 0;
    DeviceTableSize = 0;
    IndexTableSize = 0;
}


void Destroy_Symbol_Table()
{
	Delete_Node_Table();
	Delete_Device_Table();
}


void Delete_Node_Table()
{
    for (int i = 0; i < DeviceTableSize; ++i)
    {
        Delete_Node_Entry(DeviceTable[i]->name);
        //free(DeviceTable[j]);
    }

    free(DeviceTable);	
}

void Delete_Device_Table()
{
	for (int i = 0; i < DeviceTableSize; ++i)
	{
		Delete_Device_Entry(DeviceTable[i]->name);
        //free(DeviceTable[j]);
	}

	free(DeviceTable);
}

void Delete_Node_Entry(const char *name)
{
    Node_Entry *itr = *NodeTable;
    while(itr->next->next != NULL)
    {
        // if found, delete the contents
        if (!strcmp(itr->next->name, name))   
        {
            Node_Entry *temp = itr->next;

            free(temp->name);

            itr->next = temp->next;
            free(temp);

            NodeTableSize--;
            return;
        }

        itr = itr->next;   
    }
}

void Delete_Device_Entry(const char *name)
{
    Device_Entry *itr = *DeviceTable;
    while(itr->next->next != NULL)
    {
        // if found, delete the contents
        if (!strcmp(itr->next->name, name))   
        {
            Device_Entry *temp = itr->next;

            free(temp->name);

            for (int i = 0; i < temp->numnodes; ++i)
                free(temp->nodelist[i]);
            
            // Assuming that *device is unused. 
            // If used in future, free here.       
            itr->next = temp->next;
            free(temp);

            DeviceTableSize--;
            return;
        }

        itr = itr->next; 
    }
}


void Delete_Index_Entry(int hash)
{
    Index_Entry *itr = *IndexTable;
    while(itr->next->next != NULL)
    {
        // if found, delete the contents
        if (itr->hash == hash)   
        {
            Index_Entry *temp = itr->next;

            itr->next = temp->next;
            free(temp);

            IndexTableSize--;
            return;
        }

        itr = itr->next;   
    }
}


Node_Entry* Lookup_Node_Entry(const char *name)
{
	Node_Entry *itr = *NodeTable;
    while(itr != NULL)
    {
        // if found, return
        if (!strcmp(itr->name, name))   
        {
            // printf("%c\n", name->temp);
            // printf("%d\n", name->numnodes);
            // printf("%c\n", name->nodelist);
            // printf("%f\n", name->value);
            return itr;            
        }

        itr = itr->next;
    }
    
    return NULL;
}


Device_Entry* Lookup_Device_Entry(const char *name)
{
    Device_Entry *itr = *DeviceTable;
    while(itr != NULL)
    {
        // if found, return
        if (!strcmp(itr->name, name))   
        {
            // printf("%c\n", name->temp);
            // printf("%d\n", name->numnodes);
            // printf("%c\n", name->nodelist);
            // printf("%f\n", name->value);
            return itr;            
        }    

        itr = itr->next;
    }
	
	return NULL;
}


Index_Entry* Lookup_Index_Entry(int hash)
{
    Index_Entry *itr = *IndexTable;
    while(itr != NULL)
    {
        // if found, return
        if (itr->hash == hash)   
            return itr;            

        itr = itr->next;
    }
    
    return NULL;
}


Node_Entry* Insert_Node_Entry(const char *name)
{
    // Create a new node
	Node_Entry *temp = malloc(sizeof(Node_Entry));

	temp->name = name;
    temp->index = 0;
    temp->node = NULL;      // Unused    
    
    // Add it to the list
    temp->next = *NodeTable;
    *NodeTable = temp;
    NodeTableSize++;

	return *NodeTable;
}


Device_Entry* Insert_Device_Entry(const char *name,  const int numnodes, 
				Node_Entry **nodelist, const double value)
{
    // Create a new device
	Device_Entry *temp = malloc(sizeof(Device_Entry));

	temp->name = name;
	temp->numnodes = numnodes;
	temp->nodelist = nodelist;
	temp->value = value;
    temp->device = NULL;    // Unused 

    // Add it to the table
    temp->next = *DeviceTable;
    *DeviceTable = temp;
    DeviceTableSize++;

	return *DeviceTable;
}


int Insert_Index_Entry(int hash)
{
    // Check if the hash is already in the index.
    Index_Entry *lookup = Lookup_Index_Entry(hash);
    
    if(!lookup)    // Insert the hash in the index.
    {       
        // Create a new node
        Index_Entry *temp = malloc(sizeof(Index_Entry));

        temp->hash = hash;
        temp->index = IndexTableSize;

        // Add it to the list
        temp->next = *IndexTable;
        *IndexTable = temp;
      
        IndexTableSize++;
        return temp->index;
    }
    else
        return lookup->index;
}


void Print_Node_Table()
{
	Node_Entry *itr = *NodeTable;
    while(itr != NULL)
    {
        printf("node name = %s", itr->name);
        printf("\tindex = %d\n", itr->index);   

        itr = itr->next;
    }
}


void Print_Device_Table()
{
    Device_Entry *itr = *DeviceTable;
    while(itr != NULL)
    {
        printf("device name = %s\n", itr->name);
        printf("number of nodes = %d\n", itr->numnodes);
        
        for (int i = 0; i < itr->numnodes; ++i)
            printf("\tnode name =%s\n", itr->nodelist[i]->name);

        printf("device value = %f\n", itr->value);

        itr = itr->next;
    }
}


void Print_Index_Table()
{
    Index_Entry *itr = *IndexTable;
    while(itr != NULL)
    {
        printf("node hash = %d\n", itr->hash);
        printf("node index = %d\n", itr->index);   

        itr = itr->next;
    }
}


int NameHash(const char *name,  int tsize)
{
	const char *s; 
	register int i = 0; 

	// In case no name
	if (!name)	return (0);
	
	for (s = name; *s; s++) {
		i += (unsigned) *s; 
	}
	
	return (i % tsize);
}


