#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
typedef struct node
{
    int val;
    struct node *parent;
    struct node *left;
    struct node *right;
} Node;
typedef struct scapegoatTree
{
    int size;
    Node *root;
    int n; //Number of nodes in the tree
    int maxNodeCount;
} ScapegoatTree;
static int log32(int n)
{
    double const log23 = 2.4663034623764317;
    return (int)ceil(log23 * log(n));
}
void postorder(Node *tree);
void preorder(Node *tree);
void inorder(Node *tree);
bool insert(ScapegoatTree **sgTree, int val);
int insertBST(ScapegoatTree **sgTree, Node *node);
void rebuildTree(ScapegoatTree *sgTree, Node *node);
int storeInArray(Node *ptr, Node *arr[], int i);
int size(Node *node);
Node *buildBalancedFromArray(Node **a, int i, int n);
int deleteN(Node **tree, int value, ScapegoatTree **sgTree);
Node *min(Node *node);
bool search(Node *tree, int i);
//Main Function
int main()
{
    ScapegoatTree *sgTree = NULL;
    printf("\nScapeGoat Tree\n");
    int x;
    int dn = 0, ch, ck = 0;
    do
    {
        printf("\nScapeGoat Tree Operations\n");
        printf("1. Insert a given element \n2. Count number of nodes\n3. Search a given element\n4. Delete a given             element\n5. Check if Tree is Empty\n6. Make the Tree Empty\n7. Exit");
        printf("\nEnter your Choice : ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            printf("Enter an Integer Element to Insert : ");
            scanf("%d", &x);
            if (ck == 0 || (ck == 1 && search(sgTree->root, x) != true))
                insert(&sgTree, x);
            else if (search(sgTree->root, x) == true)
                printf("Duplicate Entry is not allowed!\n");
            ck = 1;
            break;
        case 2:
            if (ck == 1)
                printf("Number of Nodes = %d\n", (*sgTree).size);
            else
            {
                printf("Number of Nodes = 0\n");
                continue;
            }
            break;
        case 3:
            if (ck == 1)
            {
                printf("Enter an Integer Element to Search : ");
                scanf("%d", &x);
                if (search(sgTree->root, x) == true)
                    printf("Element %d is Found in the Tree\n", x);
                else
                    printf("Element %d is Not Found in the Tree\n", x);
            }
            else
            {
                printf("Tree is Empty. Please Insert an Element first.\n");
                continue;
            }
            break;
        case 4:
            if (ck == 1)
            {
                printf("Enter an Integer Element to Delete : ");
                scanf("%d", &x);
                if (search(sgTree->root, x) == true)
                {
                    if ((*sgTree).size == 1)
                    {
                        sgTree->root = NULL;
                        (*sgTree).size = 0;
                        sgTree = NULL;
                        ck = 0;
                        continue;
                    }
                    else
                        deleteN(&(sgTree->root), x, &sgTree);
                }
                else
                    printf("Element %d Not Found in the Tree.\n", x);
            }
            else
            {
                printf("Tree is Empty. Please Insert an Element first.\n");
                continue;
            }
            break;
        case 5:
            if (ck == 0)
            {
                printf("Tree is Empty\n");
                continue;
            }
            else
                printf("Tree is not Empty\n");
            break;
        case 6:
            if (ck == 1)
            {
                sgTree->root = NULL;
                (*sgTree).size = 0;
                sgTree = NULL;
                ck = 0;
            }
            else
                printf("Tree is Already Empty\n");
            continue;
        case 7:
            printf("\nThank You!\n");
            dn = 1;
            continue;
        default:
            printf("Invalid Choice. Please choose a valid option.\n");
            continue;
        }
        printf("\nPost order Traversal : ");
        postorder(sgTree->root);
        printf("\nPre order Traversal : ");
        preorder(sgTree->root);
        printf("\nIn order Traversal : ");
        inorder(sgTree->root);
        printf("\n\n");
    } while (!dn);
    return 0;
}
//Function to search a value in the node.
bool search(Node *r, int i)
{
    Node *temp = r;
    while (temp != NULL)
    {
        if (temp->val == i)
            return true;
        if (temp->val > i)
            temp = temp->left;
        else
            temp = temp->right;
    }
    return false;
}

//Function to show Inorder Traversal of Binary Search Tree
void inorder(Node *r)
{
    if (r != NULL)
    {
        inorder(r->left);
        printf("%d  ", r->val);
        inorder(r->right);
    }
    else
        return;
}

//Function to show Preorder Traversal of Binary Search Tree
void preorder(Node *r)
{
    if (r != NULL)
    {
        printf("%d  ", r->val);
        preorder(r->left);
        preorder(r->right);
    }
    else
        return;
}

//Function to show Postorder Traversal of Binary Search Tree
void postorder(Node *r)
{
    if (r != NULL)
    {
        postorder(r->left);
        postorder(r->right);
        printf("%d  ", r->val);
    }
    else
        return;
}
//Function to insert and rebuild the tree,if possible
bool insert(ScapegoatTree **sgTree, int val)
{
    Node *node = malloc(sizeof(Node));
    node->val = val;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    int h = insertBST(sgTree, node);
    if (h > log32((*sgTree)->n))
    {
        // find the scapegoat
        Node *p = node->parent;
        while (3 * size(p) > 2 * size(p->parent))
            p = p->parent;
        // now rebuild under that scapegoat
        rebuildTree(*sgTree, p->parent);
    }
    while (node->parent)
    {
        node = node->parent;
    }
    (*sgTree)->root = node;
    (*sgTree)->size = size((*sgTree)->root);
    if ((*sgTree)->size > (*sgTree)->maxNodeCount)
        (*sgTree)->maxNodeCount = (*sgTree)->size;
    return h >= 0;
}

//Function to insert into Binary Search Tree
int insertBST(ScapegoatTree **sgTree, Node *node)
{
    // Case 1: Empty tree
    if (*sgTree == NULL)
    {
        *sgTree = malloc(sizeof(ScapegoatTree));
        (*sgTree)->root = node;
        (*sgTree)->n++;
        return 0;
    }
    // Case 2: Find where to insert, done non-recursively
    bool done = false;
    int d = 0;
    while (!done)
    {
        if (node->val < (*sgTree)->root->val)
        {
            if ((*sgTree)->root->left == NULL)
            {
                node->parent = (*sgTree)->root;
                (*sgTree)->root->left = node;
                done = true;
            }
            else
                (*sgTree)->root = (*sgTree)->root->left;
        }
        else if (node->val > (*sgTree)->root->val)
        {
            if ((*sgTree)->root->right == NULL)
            {
                node->parent = (*sgTree)->root;
                (*sgTree)->root->right = node;
                done = true;
            }
            else
                (*sgTree)->root = (*sgTree)->root->right;
        }
        else
            return -1;
        ++d;
    }
    (*sgTree)->n++;
    return d;
}
//Function to rebuild the Binary Search Tree in case the tree is not
void rebuildTree(ScapegoatTree *sgTree, Node *node)
{
    int n = size(node);
    Node *p = node->parent;
    Node *a_1[n];
    Node **a = a_1;
    storeInArray(node, a, 0);
    if (p == NULL)
    {
        sgTree->root = buildBalancedFromArray(a, 0, n);
        sgTree->root->parent = NULL;
    }
    else if (p->right == node)
    {
        p->right = buildBalancedFromArray(a, 0, n);
        p->right->parent = p;
    }
    else
    {
        p->right = buildBalancedFromArray(a, 0, n);
        p->left->parent = p;
    }
}

//Function to find the Size of a Node in Binary Search Tree
int size(Node *node)
{
    if (node == NULL)
        return 0;
    return 1 + size(node->left) + size(node->right);
}

int storeInArray(Node *ptr, Node *arr[], int i)
{
    if (ptr == NULL)
        return i;
    i = storeInArray(ptr->left, arr, i);
    arr[i++] = ptr;
    return storeInArray(ptr->right, arr, i);
}

Node *buildBalancedFromArray(Node **a, int i, int n)
{
    if (n == 0)
        return NULL;
    int m = n / 2;
    // a[m] becomes root of new subtree
    a[i + m]->left = buildBalancedFromArray(a, i, m);
    // a[0] to a[m-1] are stored in left subtree
    if (a[i + m]->left != NULL)
        a[i + m]->left->parent = a[i + m];
    // a[m+1] to a[n-1] are stored in right subtree
    a[i + m]->right = buildBalancedFromArray(a, i + m + 1, n - m - 1);
    if (a[i + m]->right != NULL)
        a[i + m]->right->parent = a[i + m];
    return a[i + m];
}

//Function to Delete a given element from Binary Search Tree
int deleteN(Node **root, int k, ScapegoatTree **sgTree)
{
    Node *tmp = *root;
    if (tmp == NULL)
        return -1;
    if (k < tmp->val)
    {
        int depth = deleteN(&(tmp->left), k, sgTree);
        return depth == -1 ? -1 : 1 + depth;
    }
    else if (k > tmp->val)
    {
        int depth = deleteN(&(tmp->right), k, sgTree);
        return depth == -1 ? -1 : 1 + depth;
    }
    if (tmp->left != NULL && tmp->right != NULL)
    {
        Node *next = min(tmp->right);
        tmp->val = next->val;
        deleteN(&(tmp->right), next->val, sgTree);
        return 0;
    }
    if (tmp->right == NULL && tmp->left == NULL)
        *root = NULL;
    else if (tmp->left == NULL && tmp->right != NULL)
        *root = tmp->right;
    else //if(tmp->right == NULL)
        *root = tmp->left;
    (*sgTree)->n -= 1;
    (*sgTree)->size = size((*sgTree)->root);
    if ((*sgTree)->size < (*sgTree)->maxNodeCount * (3.0 / 2))
        rebuildTree(*sgTree, (*sgTree)->root);
    return 0;
}

//Function to find the Minimum Value present in Binary Search Tree
Node *min(Node *root)
{
    Node *c = root;
    /* loop down to find the leftmost leaf */
    while (c->left != NULL)
        c = c->left;
    return c;
}
