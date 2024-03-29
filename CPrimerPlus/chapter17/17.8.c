/**
 * @file 17.8.c
 * @author your name (you@domain.com)
 * @brief
 * Modify the Pet Club program so that all pets with the same name are stored in a list in
the same node. When the user chooses to find a pet, the program should request the pet
name and then list all pets (along with their kinds) having that name.
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pet.h"
typedef struct
{
    BiNode *parent;
    BiNode *child;
} Pair;

static void DeleteAllNodes(BiNode *root);
static void Delete(BiNode **p);
static void InOrder_1(const BiNode *root, void (*visit)(Item));
static void InOrder_2(const BiNode *root, void (*visit)(Item));
static void InOrder_3(const BiNode *root, void (*visit)(Item));
static Pair SeekNode(Item *i, BiTree *tree);
static Bool ToRight(const Item *i1, const Item *i2);
static Bool ToLeft(const Item *i1, const Item *i2);
static Bool Add(BiNode *new_node, BiNode *root);
static BiNode *MakeNode(const Item *i);
static void DeleteFLNode(Item *i);
static void countsize(Item);

static int count;

void InitBiTree(BiTree *tree)
{
    tree->root = NULL;
    tree->size = 0;
}

Bool BiTreeEmpty(const BiTree *tree)
{
    return tree->size == 0 ? TRUE : FALSE;
}

int BiTreeSize(const BiTree *tree)
{
    extern int count;

    count = 0;
    TraverBiTree(tree, countsize);
    return count;
}

Bool AddNode(const Item *i, BiTree *tree)
{
    BiNode *new_node;

    /*if(SeekItem(i,tree).child!=NULL){
        fprintf(stderr,"Attempted to add duplicate item\n");
        return FALSE;
    }*/
    new_node = MakeNode(i);
    if (new_node == NULL)
    {
        fprintf(stderr, "Can't create node\n");
        return FALSE;
    }
    if (tree->root == NULL)
    {
        tree->root = new_node;
        tree->size++;
    }
    else
    {
        if (Add(new_node, tree->root))
            tree->size++;
        else
            return FALSE;
    }
    return TRUE;
}

List *InBiTree(Item *i, BiTree *tree)
{
    return &(SeekNode(i, tree).child->i);
}

Bool DeleteNode(Item *i, BiTree *tree)
{
    Pair look;
    look.parent = NULL;
    look.child = tree->root;

    if (look.child == NULL)
        return FALSE;
    while (look.child != NULL)
    {
        if (ToLeft(i, &(look.child->i)))
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if (ToRight(i, &(look.child->i)))
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else
        {
            if (strcmp(i->petkind, look.child->i.petkind) == 0)
                if (ListSize(&(look.child->i)) == 1)
                    break;
                else
                {
                    DeleteFLNode(&(look.child->i));
                    return TRUE;
                }
            else if (DeleteLNode(i->petkind, &(look.child->i)))
            {
                return TRUE;
            }
            else
                return FALSE;
        }
    }
    if (look.child == NULL)
        return FALSE;
    if (look.parent == NULL)
        Delete(&tree->root);
    else if (look.parent->left == look.child)
        Delete(&look.parent->left);
    else
        Delete(&look.parent->right);
    tree->size--;
    return TRUE;
}

void TraverBiTree(const BiTree *tree, void (*visit)(Item))
{
    if (tree->size != 0)
        InOrder_1(tree->root, visit);
}

void DeleteBiTree(BiTree *tree)
{
    if (tree->size != 0)
        DeleteAllNodes(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

static void countsize(Item i)
{
    extern int count;
    count += i.count;
}

static void DeleteFLNode(Item *item)
{
    LNode *next = item->next;

    strcpy(item->petkind, next->petkind);
    strcpy(item->petname, next->petname);
    item->next = next->next;
    item->count--;
}
static BiNode *MakeNode(const Item *i)
{
    BiNode *new_node;

    new_node = malloc(sizeof(BiNode));
    if (new_node != NULL)
    {
        new_node->i = *i;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}

static Bool Add(BiNode *new_node, BiNode *root)
{
    if (ToLeft(&new_node->i, &root->i))
    {
        if (root->left == NULL)
            root->left = new_node;
        else
            return Add(new_node, root->left);
    }
    else if (ToRight(&new_node->i, &root->i))
    {
        if (root->right == NULL)
            root->right = new_node;
        else
            return Add(new_node, root->right);
    }
    else
    {
        if (root->i.count == 1)
            InitList(&(root->i));
        if (!AddLNode(new_node->i, &(root->i)))
            return FALSE;
    }
    return TRUE;
}

static Bool ToLeft(const Item *i1, const Item *i2)
{
    int comp1;

    if ((comp1 = strcmp(i1->petname, i2->petname)) < 0)
        return TRUE;
    else if (comp1 == 0 && strcmp(i1->petkind, i2->petkind) < 0)
        return TRUE;
    else
        return FALSE;
}
static Bool ToRight(const Item *i1, const Item *i2)
{
    int comp1;

    if ((comp1 = strcmp(i1->petname, i2->petname)) > 0)
        return TRUE;
    else if (comp1 == 0 && strcmp(i1->petname, i2->petname) > 0)
        return TRUE;
    else
        return FALSE;
}
static Pair SeekNode(Item *i, BiTree *tree)
{
    Pair look;
    look.parent = NULL;
    look.child = tree->root;

    if (look.child == NULL)
        return look;
    while (look.child != NULL)
    {
        if (ToLeft(i, &(look.child->i)))
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if (ToRight(i, &(look.child->i)))
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else
            break;
    }
    return look;
}

static void Delete(BiNode **p)
{
    BiNode *temp;
    if ((*p)->left == NULL)
    {
        temp = *p;
        *p = (*p)->right;
        free(temp);
    }
    else if ((*p)->right == NULL)
    {
        temp = *p;
        *p = (*p)->left;
        free(temp);
    }
    else
    {
        for (temp = (*p)->left; temp->right != NULL; temp = temp->right)
            continue;
        temp->right = (*p)->right;
        temp = *p;
        *p = (*p)->left;
        free(temp);
    }
}

static void InOrder_1(const BiNode *root, void (*visit)(Item))
{
    if (root != NULL)
    {
        visit(root->i);
        InOrder_1(root->left, visit);
        InOrder_1(root->right, visit);
    }
}
static void InOrder_2(const BiNode *root, void (*visit)(Item))
{
    if (root != NULL)
    {
        InOrder_2(root->left, visit);
        visit(root->i);
        InOrder_2(root->right, visit);
    }
}
static void InOrder_3(const BiNode *root, void (*visit)(Item))
{
    if (root != NULL)
    {
        InOrder_3(root->left, visit);
        InOrder_3(root->right, visit);
        visit(root->i);
    }
}

static void DeleteAllNodes(BiNode *root)
{
    BiNode *pright;
    if (root != NULL)
    {
        pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}

static void CopyToNode(Item item, LNode *pnode);

void InitList(List *plist)
{
    plist->next = NULL;
}

Bool ListEmpty(const List *plist)
{
    if (plist->next == NULL)
        return TRUE;
    else
        return FALSE;
}

int ListSize(const List *plist)
{
    return plist->count;
}

Bool AddLNode(Item item, List *plist)
{
    LNode *newnode;
    LNode *p = plist->next;

    newnode = malloc(sizeof(LNode));
    if (newnode == NULL)
    {
        printf("Allocate memory failed!\n");
        return FALSE;
    }
    CopyToNode(item, newnode);
    if (ListEmpty(plist))
    {
        if (strcmp(newnode->petkind, plist->petkind) != 0)
        {
            plist->next = newnode;
            plist->count++;
            return TRUE;
        }
        else
        {
            printf("petkind %s is the same.\n", newnode->petkind);
            return FALSE;
        }
    }
    else
    {
        while (p->next)
        {
            if (strcmp(newnode->petkind, p->petkind) == 0)
            {
                printf("petkind %s is the same.\n", newnode->petkind);
                return FALSE;
            }
            else
                p = p->next;
        }
        if (strcmp(newnode->petkind, p->petkind) == 0)
        {
            printf("petkind %s is the same.\n", newnode->petkind);
            return FALSE;
        }
    }
    p->next = newnode;
    plist->count++;
    return TRUE;
}
Bool SeekLNode(char petkind[], List *plist)
{
    LNode *p = plist->next;

    if (strcmp(plist->petkind, petkind) == 0)
        return TRUE;
    else
        while (p)
        {
            if (strcmp(p->petkind, petkind) == 0)
                return TRUE;
            p = p->next;
        }
    return FALSE;
}

Bool DeleteLNode(char kind[], List *plist)
{
    LNode *pre, *current;

    current = pre = plist->next;
    while (current != NULL)
    {
        if (strcmp(kind, current->petkind) == 0)
        {
            if (pre == current)
                plist->next = current->next;
            else
                pre->next = current->next;
            free(current);
            plist->count--;
            return TRUE;
        }
        pre = current;
        current = current->next;
    }
    return FALSE;
}

void TraverList(const List *plist, void (*visit)(LNode))
{
    LNode *temp = plist->next;

    printf("The petname %s count to %d\n", plist->petname, plist->count);
    printf("The petname %-15s is from kind %-15s\n", plist->petname, plist->petkind);
    while (temp != NULL)
    {
        visit(*temp);
        temp = temp->next;
    }
}

void ClearList(List *plist)
{
    LNode *temp = plist->next;
    LNode *del = temp;

    while (temp != NULL)
    {
        temp = temp->next;
        free(del);
        del = temp;
    }
}

char *s_gets(char *st, int n)
{
    char *ret_val;
    char *find;
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n'); // look for newline
        if (find)                // if the address is not NULL,
            *find = '\0';        // place a null character there
        else
            while (getchar() != '\n')
                continue; // dispose of rest of line
    }
    return ret_val;
}

void visit(LNode node)
{
    printf("The petname %-15s is from kind %-15s\n", node.petname, node.petkind);
}

static void CopyToNode(Item item, LNode *pnode)
{
    strcpy(pnode->petname, item.petname);
    strcpy(pnode->petkind, item.petkind);
    pnode->next = NULL;
}

static void uppercase(char *);
static void printitem(Item);
static void printnext(LNode *item);
static Bool do_continue(void);

char menu(void)
{
    int ch;

    puts("Nerfville Pet Club Membership Program");
    puts("Enter the letter corresponding to your choice:");
    puts("a)add a pet              l)show list of pets");
    puts("n)number of pets         f)find pets");
    puts("d)delete a pet           q)quit");
    while ((ch = getchar()) != EOF)
    {
        CLEAN();
        ch = tolower(ch);
        if (strchr("alrfndq", ch) == NULL)
            puts("Please enter an a,l,f,n,d,or q:");
        else
            break;
    }
    if (ch == EOF)
        ch = 'q';

    return ch;
}

void addpet(BiTree *t)
{
    Item temp;

    while (1)
    {
        puts("Please enter name of pet:");
        s_gets(temp.petname, PETNAME);
        puts("Please enter pet kind:");
        s_gets(temp.petkind, PETKIND);
        uppercase(temp.petkind);
        uppercase(temp.petname);
        printf("%s the %s\n", temp.petname, temp.petkind);
        temp.count = 1;
        temp.next = NULL;
        if (AddNode(&temp, t) == FALSE)
        {
            printf("AddNode failed??\n");
        }
        if (!do_continue())
            break;
    }
}

void showpets(const BiTree *t)
{
    if (BiTreeEmpty(t))
        puts("No entries!");
    else
        TraverBiTree(t, printitem);
}

void findpet(BiTree *t)
{
    Item *temp;

    if (BiTreeEmpty(t))
    {
        puts("No entries!");
        return;
    }

    temp = malloc(sizeof(Item));
    puts("Please enter name of pet you wish to find:");
    s_gets(temp->petname, PETNAME);
    uppercase(temp->petname);
    if (temp = InBiTree(temp, t))
        TraverList(temp, visit);
    else
        printf("is not a member.\n");
}

void droppet(BiTree *t)
{
    Item temp;

    if (BiTreeEmpty(t))
    {
        puts("No entries!");
        return;
    }

    puts("Please enter name of pet you wish to delete:");
    s_gets(temp.petname, PETNAME);
    puts("Please enter pet kind:");
    s_gets(temp.petkind, PETKIND);
    uppercase(temp.petname);
    uppercase(temp.petkind);
    printf("%s the %s ", temp.petname, temp.petkind);
    if (DeleteNode(&temp, t))
        printf("is dropped from the club.\n");
    else
        printf("is not a member.\n");
}

static Bool do_continue(void)
{
    char c;

    printf("continue ? (y for yes/n for no)");
    while (scanf("%c", &c) && c != 'y' && c != 'n')
    {
        CLEAN();
        printf("continue ? (y for yes/n for no)");
    }
    CLEAN();
    return c == 'y' ? TRUE : FALSE;
}
static void uppercase(char *ch)
{
    while (*ch)
    {
        *ch = toupper(*ch);
        ch++;
    }
}

static void printitem(Item item)
{
    printf("Pet:%-19s Kind:%-19s\n", item.petname, item.petkind);
    if (item.count > 1)
        printnext(item.next);
}
static void printnext(LNode *item)
{
    while (item)
    {
        printf("Pet:%-19s Kind:%-19s\n", item->petname, item->petkind);
        item = item->next;
    }
}

int main(void)
{
    BiTree pets;
    char choice;

    InitBiTree(&pets);
    while ((choice = menu()) != 'q')
    {
        switch (choice)
        {
        case 'a':
            addpet(&pets);
            break;
        case 'l':
            showpets(&pets);
            break;
        case 'f':
            findpet(&pets);
            break;
        case 'n':
            printf("%d pets in club\n", BiTreeSize(&pets));
            break;
        case 'd':
            droppet(&pets);
            break;
        default:
            puts("Switching error");
        }
    }
    DeleteBiTree(&pets);
    puts("Bye.");

    return 0;
}