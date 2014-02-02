/* Copyright (C) 
* 2014 - firstboy0513
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file b_tree.c
* @brief B tree implement in C.
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-01
*/
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief B tree public method extern to use or private be invoke.
*/
#define PUBLIC
#define PRIVATE static

/**
* @brief Element compare EQ as equal, LT as little than, GT as greate than.
*/

#define EQ(a, b) (a == b)
#define LT(a, b) (a < b)
#define GT(a, b) (a > b)

/**
* @brief Element copy macro.
*
* @param des destination element.
* @param src source element.
*/
#define ELEMENT_COPY(des, src) { des = src; }

/**
* @brief Element type.
*/
typedef int ELEMENT;

/**
* @brief Boolean type define.
*/
typedef enum { FALSE, TRUE } BOOL;

/**
* @brief B tree node struct, with data also named as key, color, and 3
* point which is parent node point, left child and right child node point.
*/
typedef struct _NODE {
	ELEMENT data;
	struct _NODE* p;
	struct _NODE* left;
	struct _NODE* right;
	int n;
	BOOL leaf;
} NODE;

/**
* @brief B tree struct with nil node and root point.
*/
typedef struct _TREE {
	struct _NODE* root;
	struct _NODE* nil;
} TREE;

/**
* @brief Out use methods of B-Tree structure.
*/
TREE* btree_init();
void btree_destory(TREE* T);
int btree_search(TREE* T, ELEMENT d);
int btree_insert(TREE* T, ELEMENT d);
int btree_delete(TREE* T, ELEMENT d);
void btree_traversal(TREE* T, void function(NODE*));

/**
* @brief Internal methods of B-Tree structure.
*/
NODE* _btree_allocate_node();
void _btree_disk_write(NODE* x);

void _btree_init(TREE* T) {
	NODE* x = _btree_allocate_node();
	x->leaf = TRUE;
	x->n = 0;
	_btree_disk_write(x);
	T->root = x;
}

int main(int argc, const char *argv[])
{
	return 0;
}
