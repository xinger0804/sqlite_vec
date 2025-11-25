#include "kdtree.h"
#include <sqlite3.h>
#include <sqlite3ext.h>

// Internal recursive insert
KDNode *kdtree_insert_rec(KDNode *node, float *point, sqlite3_int64 rowid,
                          int depth, int dims)
{
  if (!node)
  {
    KDNode *n = sqlite3_malloc(sizeof(KDNode));
    if (!n)
      return NULL;
    n->point = sqlite3_malloc(sizeof(float) * dims);
    if (!n->point)
    {
      sqlite3_free(n);
      return NULL;
    }
    memcpy(n->point, point, sizeof(float) * dims);
    n->rowid = rowid;
    n->left = n->right = NULL;
    n->axis = depth % dims;
    return n;
  }

  int axis = node->axis;
  if (point[axis] < node->point[axis])
    node->left = kdtree_insert_rec(node->left, point, rowid, depth + 1, dims);
  else
    node->right = kdtree_insert_rec(node->right, point, rowid, depth + 1, dims);

  return node;
}

// Public insert function
KDNode *kdtree_insert(KDNode *root, float *point, sqlite3_int64 rowid, int dims)
{
  return kdtree_insert_rec(root, point, rowid, 0, dims);
}

// Free KDTree nodes recursively
void kdtree_free(KDNode *node)
{
  if (!node)
    return;
  kdtree_free(node->left);
  kdtree_free(node->right);
  sqlite3_free(node->point);
  sqlite3_free(node);
}

// Debug/test functions

void kd_print_node(KDNode *node, int depth, int dims)
{
  if (!node)
    return;

  for (int i = 0; i < depth; i++)
    printf("  ");

  printf("rowid=%lld, vector=[", node->rowid);
  for (int i = 0; i < dims; i++)
  {
    printf("%f", node->point[i]);
    if (i < dims - 1)
      printf(", ");
  }
  printf("]\n");

  kd_print_node(node->left, depth + 1, dims);
  kd_print_node(node->right, depth + 1, dims);
}

void kd_print_tree(KDTree *tree)
{
  if (!tree)
  {
    printf("KDTree is NULL\n");
    return;
  }
  printf("KDTree (size=%d, dims=%d):\n", tree->size, tree->dimensions);
  kd_print_node(tree->root, 0, tree->dimensions);
}
