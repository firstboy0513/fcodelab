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
#include <string.h>

/**
 * @brief B tree public method extern to use or private be invoke.
*/
#define PUBLIC
#define PRIVATE static

#define NIL		NULL

#define BT		4
#define FULL_KEY_COUNT (2*BT-1)
#define FULL_CHILD_COUNT (2*BT)

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
* @brief B tree node struct, with x.n and leaf boolean, and n keys and n+1 
* children pointers. The keys seperate the ranges of keys stored in each 
* subtree. All leaves have the same depth, which is the tree's height h.
* Nodes have lower and upper bounds on the number of keys they can contain.
* We express these bounds in terms of a fixed integer t>=2 called the minimum 
* degree of the B-tree.
* 	a. Every node other than the root must have at least t-1 keys. Every 
* 	internal node other than the root must thus has at least t children. If the
* 	tree is nonempty, the root must have at least one key.
* 	b. Every node may contain at most 2t-1 keys. Therefore, an internal node may
* 	have at most 2t children. We say that a node is full if it contains exactly 
* 	2t-1 keys.
*/
typedef struct _NODE {
	int n;				// the numbers of keys currently stored in the node.
	BOOL leaf;			// a boolean value that is TRUE if x is a leaf and FALSE
						// if x is an Internal node.
	ELEMENT* key;		// x.n keys themselves, from key1 to keyn stored in 
						// nondecreasing order.
	struct _NODE** c;	// if node is internal, it contains x.n+1 pointers from
						// c1 to cn+1 to its children. Leaf nodes have no 
						// children, and the ci attributes are undefined.
} NODE;

/**
* @brief search result struct.
*/
typedef struct _SEARCH_RET {
	NODE* x;			// node pointer.
	int i;				// the index of key.
} SEARCH_RET;

#define SEARCH_NIL (SEARCH_RET){NIL, 0}

/**
* @brief B tree struct with root point.
*/
typedef struct _TREE {
	struct _NODE* root;
	/// struct _NODE* nil;		// the nil, uh, maybe not use.
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

// TODO
void _btree_disk_read(NODE* x) {
}

// TODO
void _btree_disk_write(NODE* x) {
}

// OK
/**
* @brief Internal methods of B-Tree structure.
*/
NODE* _btree_allocate_node() {
	NODE* x = (NODE*) malloc(sizeof(NODE));
	memset(x, 0, sizeof(NODE));
	x->c = (struct _NODE**) malloc(sizeof(struct _NODE*) * FULL_CHILD_COUNT);
	x->key = (ELEMENT*) malloc(sizeof(ELEMENT) * FULL_KEY_COUNT);
	memset(x->c, 0, sizeof(struct _NODE*) * FULL_CHILD_COUNT);
	memset(x->key, 0, sizeof(struct _NODE*) * FULL_KEY_COUNT);
	return x;
}

void _btree_create(TREE* T) {
	NODE* x = _btree_allocate_node();
	x->leaf = TRUE;
	x->n = 0;
	_btree_disk_write(x);
	T->root = x;
}

// TODO
void _btree_destory_node(NODE* x) {
	int i = 0;
	for (i = 0; i <= x->n; i ++) {
		_btree_destory_node(x->c[i]);
	}
	free(x);
}

// TODO
void btree_destory(TREE* T) {
	_btree_destory_node(T->root);
	free(T);
}

SEARCH_RET _btree_search(NODE* x, ELEMENT k) {
	int i = 0;
	while (i < x->n && k < x->key[i]) { i ++; }
	if (i < x->n && k == x->key[i]) {
		return (SEARCH_RET){x, i};
	} else if (x->leaf) {
		return SEARCH_NIL;
	} else {
		_btree_disk_read(x->c[i]);
		return _btree_search(x->c[i], k);
	}
}

// OK
int btree_search(TREE* T, ELEMENT k) {
	SEARCH_RET sr;
	sr = _btree_search(T->root, k);
	if (NIL == sr.x) {
		return 0;
	} else {
		return 1;
	}
}

// OK
void _btree_split_child(NODE* x, int i) {
	NODE* z = _btree_allocate_node();
	NODE* y = x->c[i];
	z->leaf = y->leaf;
	z->n = BT-1;
	int j = 0;
	for (j = 0; j <= BT-1; j ++) {
		z->key[j] = y->key[j+BT];
	}
	if (!y->leaf) {
		for (j = 0; j <= BT; j ++) {
			z->c[j] = y->c[j+BT];
		}
	}
	y->n = BT-1;
	for (j = x->n+1; j >= i+1; j --) {
		x->c[j+1] = x->c[j];
	}
	x->c[i+1] = z;
	for (j = x->n; j >= i; j --) {
		x->key[j+1] = x->key[j];
	}
	x->key[i] = y->key[i];
	x->n ++;
	_btree_disk_write(y);
	_btree_disk_write(z);
	_btree_disk_write(x);
}

/// // TODO
/// // It's some more complicted here...
/// void btree_insert(TREE* T, ELEMENT d) {
/// 	NODE* r = T->root;
/// 	if (r->n == FULL_KEY_COUNT) {
/// 	} else {
/// 	}
/// }
/// 
/// // TODO
/// int btree_delete(TREE* T, ELEMENT d) {
/// 	return -132423;
/// }
/// 
/// // TODO
/// void btree_traversal(TREE* T, void function(NODE*)) {
/// }

int main(int argc, const char *argv[])
{
	return 0;
}
