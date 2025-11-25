#ifndef KDTREE_H
#define KDTREE_H

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>

typedef struct KDNode
{
  float *point;
  sqlite3_int64 rowid;
  struct KDNode *left;
  struct KDNode *right;
  int axis;
} KDNode;

typedef struct KDTree
{
  KDNode *root;
  int dimensions;
  int size;
} KDTree;

typedef struct KDSearchCtx
{
  const float *query;
  int dims;
  KDNode *best;
  float bestDist;
} KDSearchCtx;
KDNode *kdtree_insert_rec(KDNode *node, float *point, sqlite3_int64 rowid,
                          int depth, int dims);

KDNode *kdtree_insert(KDNode *root, float *point, sqlite3_int64 rowid, int dims);
// void kdtree_free(KDNode *node)
// {
//   if (!node)
//     return;
//   kdtree_free(node->left);
//   kdtree_free(node->right);
//   sqlite3_free(node->point);
//   sqlite3_free(node);
// }

// Debug/test functions

// void kd_print_node(KDNode *node, int depth, int dims)
// {
//   if (!node)
//     return;

//   for (int i = 0; i < depth; i++)
//     printf("  ");

//   printf("rowid=%lld, vector=[", node->rowid);
//   for (int i = 0; i < dims; i++)
//   {
//     printf("%f", node->point[i]);
//     if (i < dims - 1)
//       printf(", ");
//   }
//   printf("]\n");

//   kd_print_node(node->left, depth + 1, dims);
//   kd_print_node(node->right, depth + 1, dims);
// }

// void kd_print_tree(KDTree *tree)
// {
//   if (!tree)
//   {
//     printf("KDTree is NULL\n");
//     return;
//   }
//   printf("KDTree (size=%d, dims=%d):\n", tree->size, tree->dimensions);
//   kd_print_node(tree->root, 0, tree->dimensions);
// }

#endif KDTREE_H
