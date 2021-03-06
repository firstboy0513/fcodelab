/* Copyright (C) 
* 2013 - firstboy0513
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
* @file rebuild_tree.c
* @brief rebuild_tree
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-24
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
	char data;
	struct Node* lChild;
	struct Node* rChild;
} Node;

/**
* @brief Generate a new node of malloc memory and initialize node value.
*
* @param data node data value.
*
* @return node point.
*/
Node* GenerateNewNode(char data) {
	Node* pNode = (Node*) malloc(sizeof(Node));
	pNode->data = data;
	pNode->lChild = NULL;
	pNode->rChild = NULL;
	return pNode;
}

/**
* @brief Destory tree.
*
* @param root tree root.
*/
void DestoryTree(Node* root) {
	if (NULL == root) return;
	DestoryTree(root->lChild);
	DestoryTree(root->rChild);
	free(root);
}

/**
* @brief Print tree node at given level.
*
* @param root tree root.
* @param level given level.
*
* @return return 0 if root is null or level is negative, return 1 if level is 
* reach.
*/
int PrintNodeAtLevel(Node* root, int level) {
	if (!root || level < 0) {
		return 0;
	}
	if (0 == level) {
		printf("%c  ", root->data);
		return 1;
	}
	return PrintNodeAtLevel(root->lChild, level - 1) 
		+ PrintNodeAtLevel(root->rChild, level - 1);
}

/**
* @brief Print tree node data at all level from root 0 level.
*
* @param root tree level.
*
* @return return 1.
*/
int PrintNodeAllLevel(Node* root) {
	int level = 0;
	while (PrintNodeAtLevel(root, level++) > 0) {
		printf("\n");
	}
	return 1;
}

#define NewCharArray(p, len)                        \
	char* p = (char*) malloc(sizeof(char) * len);   \
	memset(p, 0, len);                              \
	
/**
* @brief Rebuild binary tree by given preorder and inorder.
*
* @param pPreOrder given preorder.
* @param pInOrder given inorder.
* @param nTreeLen given order length.
* @param pRoot output binary tree root.
*/
void Rebuild(const char* pPreOrder, 
	const char* pInOrder, 
	int nTreeLen, Node** pRoot) {
	*pRoot = NULL;
	if (NULL == pPreOrder 
		|| 0 == strlen(pPreOrder)
		|| NULL == pInOrder 
		|| 0 == strlen(pInOrder)
		|| nTreeLen != (int)strlen(pPreOrder) 
		|| nTreeLen != (int)strlen(pInOrder) ) {
		return;
	}
	char curroot = pPreOrder[0];
	// find leftInOrder and rightInOrder
	NewCharArray(leftInOrder, nTreeLen);
	NewCharArray(rightInOrder, nTreeLen);
	int i = 0, j = 0, n_leftlen = 0;
	while (i < nTreeLen && pInOrder[i] != curroot) {
		leftInOrder[j++] = pInOrder[i++];
	}
	if (i >= nTreeLen) {
		return;
	}
	n_leftlen = j;
	*pRoot = GenerateNewNode(pInOrder[i]);
	i ++, j = 0;
	while (i < nTreeLen && '\0' != pInOrder[i]) {
		rightInOrder[j++] = pInOrder[i++];
	}
	// find leftPreOrder and rightPreOrder
	NewCharArray(leftPreOrder, nTreeLen);
	NewCharArray(rightPreOrder, nTreeLen);
	memcpy(leftPreOrder, pPreOrder + 1, n_leftlen);
	memcpy(rightPreOrder, pPreOrder + 1 + n_leftlen, strlen(rightInOrder));
	if ('\0' != leftPreOrder) {
		Rebuild(leftPreOrder, leftInOrder, (int)strlen(leftPreOrder), 
			&((*pRoot)->lChild));
	}
	if ('\0' != rightPreOrder) {
		Rebuild(rightPreOrder, rightInOrder, (int)strlen(rightPreOrder), 
			&((*pRoot)->rChild));
	}
	free(leftInOrder), free(rightInOrder);
	free(leftPreOrder), free(rightPreOrder);
}
	
/**
* @brief Rebuild binary tree by given preorder and inorder, 2th simple way.
* the Rebuild2 may is more less resource useage than Rebuild way.
*
* @param pPreOrder given preorder.
* @param pInOrder given inorder.
* @param nTreeLen given order length.
* @param pRoot output binary tree root.
*/
void Rebuild2(const char* pPreOrder, 
	const char* pInOrder, 
	int nTreeLen, Node** pRoot) {
	if (NULL == pPreOrder 
		|| NULL == pInOrder
		|| nTreeLen <= 0) {
		return;
	}
	char curroot = pPreOrder[0];
	*pRoot = GenerateNewNode(curroot);
	int i = 0;
	while (i < nTreeLen && curroot != pInOrder[i]) {
		i ++;
	}
	if (i >= nTreeLen) return;
	int nLeftLen = i;
	int nRightLen = nTreeLen - nLeftLen - 1;
	if (nLeftLen > 0) {
		Rebuild2(pPreOrder + 1, pInOrder, nLeftLen, &((*pRoot)->lChild));
	}
	if (nRightLen > 0) {
		Rebuild2(pPreOrder + 1 + nLeftLen, pInOrder + nLeftLen + 1, nRightLen, 
			&((*pRoot)->rChild));
	}
}

/**
* @brief Use for rebuild binary tree without recursive, it store rebuild info.
*/
typedef struct _rebuild_info {
	const char* pPreOrder;
	const char* pInOrder;
	int nTreeLen;
	Node** pRoot;
} REBINFO;

/**
* @brief Set rebuild macro.
*
* @param rebinfo REBINFO struct.
* @param pPreOrderi given preorder.
* @param pInOrderi given inorder.
* @param nTreeLeni given order length.
* @param pRooti output binary tree root.
*/
#define REBINFO_SET(rebinfo, pPreOrderi, pInOrderi, nTreeLeni, pRooti) \
	rebinfo.pPreOrder = pPreOrderi;                                    \
	rebinfo.pInOrder = pInOrderi;                                      \
	rebinfo.nTreeLen = nTreeLeni;                                      \
	rebinfo.pRoot = pRooti;                                            \

#define QUE_PUSH_REBINFO(que, pPreOrderi, pInOrderi, nTreeLeni, pRooti) \
	REBINFO _rebinfo;                                                   \
	REBINFO_SET(_rebinfo, pPreOrderi, pInOrderi, nTreeLeni, pRooti);    \
	que.push(_rebinfo);                                                 \

#include <queue>
using std::queue;

/**
* @brief Rebuild binary tree by given preorder and inorder, 2th simple way.
* the Rebuild2 may is more less resource useage than Rebuild way.
* And implement it without recursive using queue to store rebuild info state.
*
* @param pPreOrder given preorder.
* @param pInOrder given inorder.
* @param nTreeLen given order length.
* @param pRoot output binary tree root.
*/
void Rebuild2_WithoutRecursive(const char* pPreOrder, 
	const char* pInOrder, 
	int nTreeLen, Node** pRoot) {
	if (NULL == pPreOrder 
		|| NULL == pInOrder
		|| nTreeLen <= 0) {
		return;
	}
	queue<REBINFO> que;
	QUE_PUSH_REBINFO(que, pPreOrder, pInOrder, nTreeLen, pRoot)
	while (!que.empty()) {
		REBINFO rebinfo = que.front(); que.pop();
		char curroot = rebinfo.pPreOrder[0];
		*(rebinfo.pRoot) = GenerateNewNode(curroot);
		int i = 0;
		while (i < rebinfo.nTreeLen && curroot != rebinfo.pInOrder[i]) {
			i ++;
		}
		if (i >= rebinfo.nTreeLen) return;
		int nLeftLen = i;
		int nRightLen = rebinfo.nTreeLen - nLeftLen - 1;
		if (nLeftLen > 0) {
			QUE_PUSH_REBINFO(que, 
				rebinfo.pPreOrder + 1, 
				rebinfo.pInOrder, 
				nLeftLen, 
				&((*(rebinfo.pRoot))->lChild));
		}
		if (nRightLen > 0) {
			QUE_PUSH_REBINFO(que, 
				rebinfo.pPreOrder + 1 + nLeftLen, 
				rebinfo.pInOrder + nLeftLen + 1, 
				nRightLen, 
				&((*(rebinfo.pRoot))->rChild));
		}
	}
}

int main(int argc, const char *argv[]) {

	Node* root = NULL;
	const char* pPreOrder = "abdcef";
	const char* pInOrder = "dbaecf";
	// Rebuild(...);
	// Rebuild2(...);
	Rebuild2_WithoutRecursive(pPreOrder, 
		pInOrder, 
		(int)strlen(pPreOrder), 
		&root);
	
	PrintNodeAllLevel(root);
	printf("\n");

	DestoryTree(root);
	return 0;
}
