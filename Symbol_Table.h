/**
	Symbol_Table.h
	For the undergraduate EDA course offered at 
	Dept of Micro/Nano-electronics

	(c) Guoyong Shi, Shanghai Jiao Tong University
	All rights reserved, 2006-2015

	If you modify the code, please keep the above copyright info.
*/

#ifndef SYMBLE_TABLE_H
#define SYMBLE_TABLE_H

/**
	A node struct constituting a node table.
	To be used for parsing netlist.
*/
struct node_s 
{
	char *name;	// node name to be allocated
	int index;	// node index for accessing matrix row/column
	void *node; 	// points to a Node instance in the future
	struct node_s *next;
};
typedef struct node_s Node_Entry;

struct device_s {
	char *name;
	int numnodes;	// length of the node list
	Node_Entry **nodelist;
	double value;
	void *device;	// points to a device instance in the future
	struct device_s *next;
};
typedef struct device_s Device_Entry;

struct index_s 
{
    int hash;
    int index;
    struct index_s *next;
};
typedef struct index_s Index_Entry;

extern Node_Entry **NodeTable;
extern int NodeTableSize;

extern Device_Entry **DeviceTable;
extern int DeviceTableSize;

extern Index_Entry **IndexTable;
extern int IndexTableSize;

void Init_Symbol_Tables();
void Destroy_Symbol_Table();

Node_Entry*    	    Lookup_Node_Entry(const char *name);
Node_Entry*    	    Insert_Node_Entry(const char *name);

void Delete_Node_Entry(const char *name);
void Delete_Node_Table();
void Print_Node_Table();

Device_Entry*   	Lookup_Device_Entry(const char *name);
Device_Entry*   	Insert_Device_Entry(const char *name,  const int numnodes, 
				Node_Entry **nodelist, const double value);

void Delete_Device_Entry(const char *name);
void Delete_Device_Table();
void Print_Device_Table();

Index_Entry*        Lookup_Index_Entry(int hash);
int                 Insert_Index_Entry(int hash);

void Delete_Index_Entry(int hash);
void Print_Index_Table();

int NameHash(const char *name, int tsize);

#endif  // SYMBLE_TABLE_H


