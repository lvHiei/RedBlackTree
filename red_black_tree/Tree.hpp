#ifndef _TREE_HPP_
#define _TREE_HPP_

#include <ostream>
#include <string>

#include "Tree.h"

template<class T, class V>
RedBlackTree<T, V>::RedBlackTree()
{
    m_null = new RedBlackNode();
    m_root = m_null;
    m_root->m_parent = m_null;
}

template<class T, class V>
RedBlackTree<T, V>::~RedBlackTree()
{
    clear();
    delete m_null;
}

template<class T, class V>
bool RedBlackTree<T, V>::isEmpty()
{
    return m_root == m_null;
}

template<class T, class V>
bool RedBlackTree<T, V>::RedBlackNode::isNullNode(node_pointer_type node)
{
    if (NULL == node->m_right && NULL == node->m_left)
    {
        return true;
    }
    return false;
}

template<class T, class V>
void RedBlackTree<T, V>::insertToTree(const key_type& iKey, const value_type& iValue)
{
    // 1.找到要插入节点的位置
    node_pointer_type pos = m_null;
    node_pointer_type trace = m_root;
    while(m_null != trace)
    {
        pos = trace;
        if (trace->m_key < iKey)
        {
            trace = trace->m_right;
        }
        else if(trace->m_key > iKey)
        {
            trace = trace->m_left;
        }
        else
        {
            pos->m_value = iValue;
            return ;
        }
    }

    // 2. 创建一个新节点，并将该节点放到其对应的位置中
    node_pointer_type node = new RedBlackNode(iKey, iValue);
    node->m_left = m_null;
    node->m_right = m_null;
    node->m_color = RED;
    node->m_parent = pos;
    if (m_root == m_null)
    {
        m_root = node;
    }

    if (pos != m_null)
    {
        if (iKey < pos->m_key)
        {
            pos->m_left = node;
        }
        else
        {
            pos->m_right = node;
        }
    }

    // 3. 插入修复
    fixRedBlackTreeInsert(node);
}

template<class T, class V>
void RedBlackTree<T, V>::deleteFromTree(const key_type& dKey)
{
    // 1. 找到要删除节点的位置
    node_pointer_type delNode = findValue(dKey);
    if (m_null == delNode)
    {
        return;
    }

    // 2. 找到代替该删除节点位置的节点（中序直接后继，右子树的最左子树）
    node_pointer_type sucessor = delNode;
    node_pointer_type delChild = delNode;
    if (m_null == delNode->m_left || m_null == delNode->m_right)
    {
        sucessor = delNode;
    }
    else
    {
        sucessor = getSuccessor(delNode);
    }

    // 3. 将后继节点的子节点（最多只有一个）放到后继节点的位置
    if (m_null != sucessor->m_left)
    {
        delChild = sucessor->m_left;
    }
    else
    {
        delChild = sucessor->m_right;
    }
    
    delChild->m_parent = sucessor->m_parent;

    if (m_null == sucessor->m_parent)
    {
        m_root = delChild;
    }
    else if(sucessor == sucessor->m_parent->m_left)
    {
        sucessor->m_parent->m_left = delChild;
    }
    else
    {
        sucessor->m_parent->m_right = delChild;
    }

    // 4. 将后继结点的值替换要删除节点的值
    if (delNode != sucessor)
    {
        delNode->m_key = sucessor->m_key;
        delNode->m_value = sucessor->m_value;
        delNode = sucessor;
    }
    
    // 5. 如果删除的节点的值是黑色的，则修复红黑树
    if (BLACK == sucessor->m_color)
    {
        fixRedBlackTreeDelete(delChild);
    }

    delete delNode;
}

template<class T, class V>
typename RedBlackTree<T, V>::node_pointer_type RedBlackTree<T, V>::findValue(const key_type& fKey)
{
    node_pointer_type p = m_root;
    while( m_null != p && p->m_key != fKey)
    {
        if (p->m_key > fKey)
        {
            p = p->m_left;
        }
        else
        {
            p = p->m_right;
        }
    }

    return p;
}

template<class T, class V>
bool RedBlackTree<T, V>::hasKey(const key_type& fKey)
{
    return m_null != findValue(fKey);
}

template<class T, class V>
void RedBlackTree<T, V>::fixRedBlackTreeInsert(node_pointer_type node)
{
    // node的parent是黑色的话加入一个红色的节点不会影响红黑树的特性
    // node的parent是红色的话不可能是null节点
    // 如果node的parent是红色的，则node的grandparent 也必然不是null节点
    while (RED == node->m_parent->m_color)
    {
        // node的parent是node的grandparent的左孩子
        if(node->m_parent == node->m_parent->m_parent->m_left)
        {
            node_pointer_type uncle = node->m_parent->m_parent->m_right;

            // case1: 叔叔的颜色是红色的
            // 将叔叔和老爸的颜色都涂黑，将爷爷的颜色涂红，并将当前节点设置为爷爷节点继续
            if (RED == uncle->m_color)
            {
                uncle->m_color = BLACK;
                node->m_parent->m_color = BLACK;
                node->m_parent->m_parent->m_color = RED;
                node = node->m_parent->m_parent;
            }
            // case 2: 叔叔的颜色是黑色的，并且node是右孩子
            // 将当前节点设置成parent，并且绕该节点左旋
            else if (node == node->m_parent->m_right)
            {
                node = node->m_parent;
                leftRotate(node);
            }
            // case 3 : 叔叔的颜色是黑色的，并且node是左孩子
            // 将parent染黑，然后grandparent染红，然后绕grandparent右旋
            else 
            {
                node->m_parent->m_color = BLACK;
                node->m_parent->m_parent->m_color = RED;
                rightRotate(node->m_parent->m_parent);
            }
        }
        // node的parent是node的grandparent的右孩子
        // 与上面左右互换就行了
        else
        {
            node_pointer_type uncle = node->m_parent->m_parent->m_left;

            if (RED == uncle->m_color)
            {
                uncle->m_color = BLACK;
                node->m_parent->m_color = BLACK;
                node->m_parent->m_parent->m_color = RED;
                node = node->m_parent->m_parent;
            }
            else if(node == node->m_parent->m_left)
            {
                node = node->m_parent;
                rightRotate(node);
            }
            else
            {
                node->m_parent->m_color = BLACK;
                node->m_parent->m_parent->m_color = RED;
                leftRotate(node->m_parent->m_parent);
            }
        }
    }

    m_root->m_color = BLACK;
}

template<class T, class V>
void RedBlackTree<T, V>::fixRedBlackTreeDelete(node_pointer_type node)
{
    while(m_root != node && BLACK == node->m_color)
    {
        if (node == node->m_parent->m_left)
        {
            // 这里brother 一定不是m_null，因为他的兄弟是一个黑色的节点，也就是node的这个节点
            node_pointer_type brother = node->m_parent->m_right;
            if (RED == brother->m_color)
            {
                // case1 
                // 当要删除的节点是红色的时候，将兄弟的颜色染成黑色，将老爸的颜色染成红色
                // 然后绕老爸节点左旋，同时兄弟节点变成未旋转之前兄弟的左孩子
                // 同时这个新的兄弟节点一定是一个黑色的节点，而且不是NULL
                // 因为之前他叔叔是黑色的，他老爸是红色的
                // 通过这个操作，并不会改变任何路径上黑节点的个数，但是可以将当前节点的兄弟转换成黑色，从而情况变成case2,3,4的其中一种
                brother->m_color = BLACK;
                node->m_parent->m_color = RED;
                leftRotate(node->m_parent);
                brother = node->m_parent->m_right;
            }

            if ( BLACK == brother->m_left->m_color && BLACK == brother->m_right->m_color)
            {
                // case2 
                // 兄弟节点是黑色的，并且兄弟节点的两个孩子都是黑色的
                // 将兄弟节点染红，然后把当前节点设置为当前节点的双亲节点
                // 将兄弟节点染红，导致双亲节点到其左右两个分支的黑色节点个数相同，
                // 但是对于双亲节点的祖父节点等祖宗节点到这两个分支的节点的黑色少一个，所以我们把当前节点置为双亲节点，然后循环
                // 这样就使得这个双重黑色的节点（当前节点）朝根节点方向前进了一步
                brother->m_color = RED;
                node = node->m_parent;
            }
            else if(BLACK == brother->m_right->m_color)
            {
                // case 3 
                // 兄弟节点是黑色的，兄弟左子是红色的，右子是黑色的
                // 将兄弟节点染红，兄弟左子染黑，绕兄弟节点右旋
                // 这样也不会改变路径上黑色节点的个数，但是可以把兄弟节点的右孩子染成红色
                // 通过这个操作可以转换成case4
                brother->m_left->m_color = BLACK;
                brother->m_color = RED;
                rightRotate(brother);
            }
            else if (RED == brother->m_right->m_color)
            {
                // case4
                // 兄弟节点是黑色的，兄弟左子随意，右子是红色的
                // 将兄弟节点染成双亲节点的颜色，将双亲节点染黑，兄弟右子染黑，然后绕双亲节点左旋
                // 这个操作可以简单的认为是这样的，自己这边缺少一个黑色的节点，而兄弟那边正好有一个红色的节点。
                // 所以通过旋转把兄弟节点的那个红色搞到自己这边来，然后把它染成黑色，这样就满足了红黑树的性质
                // 把当前节点设置为根节点，退出循环
                brother->m_color = node->m_parent->m_color;
                node->m_parent->m_color = BLACK;
                brother->m_right->m_color = BLACK;
                leftRotate(node->m_parent);
                node = m_root;
            }
        }
        else
        {
            // 与上面的左右互换就好了
            node_pointer_type brother = node->m_parent->m_left;
            if (RED == brother->m_color)
            {
                brother->m_color = BLACK;
                node->m_parent->m_color = RED;
                rightRotate(node->m_parent);
                brother = node->m_parent->m_left;
            }

            if (BLACK == brother->m_left->m_color && BLACK == brother->m_right->m_color)
            {
                brother->m_color = RED;
                node = node->m_parent;
            }
            else if(BLACK == brother->m_left->m_color)
            {
                brother->m_right->m_color = BLACK;
                brother->m_color = RED;
                leftRotate(brother);
            }
            else if (RED == brother->m_left->m_color)
            {
                brother->m_color = node->m_parent->m_color;
                node->m_parent->m_color = BLACK;
                brother->m_left->m_color = BLACK;
                rightRotate(node->m_parent);
                node = m_root;
            }
        }
    }

    node->m_color = BLACK;
}

template<class T, class V>
void RedBlackTree<T, V>::leftRotate(node_pointer_type node)
{
    // 左旋操作步骤
    // 1. 将node的右子树放到node位置
    // 2. 将node设置成node的右子树的左子树
    // 3. 将node的右子树的左子树设置成node的右子树
    node_pointer_type rchild = node->m_right;
    if (m_null != rchild)
    {
        node->m_right = rchild->m_left;
        if (m_null != rchild->m_left)
        {
            rchild->m_left->m_parent = node;
        }
        rchild->m_left = node;
        node_pointer_type p = node->m_parent;
        rchild->m_parent = p;
        node->m_parent = rchild;
        if (m_null != p)
        {
            if(node == p->m_left)
            {
                p->m_left = rchild;
            }
            else
            {
                p->m_right = rchild;
            }
        }
        else
        {
            m_root = rchild;
        }
    }
}

template<class T, class V>
void RedBlackTree<T, V>::rightRotate(node_pointer_type node)
{
    // 右旋操作与左旋大致一样
    node_pointer_type lchild = node->m_left;
    if (m_null != lchild)
    {
        node->m_left = lchild->m_right;
        if(m_null != lchild->m_right)
        {
            lchild->m_right->m_parent = node;
        }
        lchild->m_right = node;
        node_pointer_type p = node->m_parent;
        lchild->m_parent = p;
        node->m_parent = lchild;
        if (m_null != p)
        {
            if (node == p->m_left)
            {
                p->m_left = lchild;
            }
            else
            {
                p->m_right = lchild;
            }
        }
        else
        {
            m_root = lchild;
        }
    }
}

template<class T, class V>
typename RedBlackTree<T, V>::node_pointer_type RedBlackTree<T, V>::getSuccessor(node_pointer_type node)
{
    if(m_null == node)
    {
        return m_null;
    }
    
    node_pointer_type successor = node->m_right;

    while(m_null != successor && m_null != successor->m_left)
    {
        successor = successor->m_left;
    }

    if (m_null == successor)
    {
        node_pointer_type ps = node->m_parent;
        successor = node;
        while(m_null != ps && ps->m_right == successor)
        {
            successor = ps;
            ps = ps->m_parent;
        }
        successor = ps;
    }

    return successor;
}

template<class T, class V>
void RedBlackTree<T, V>::clear()
{
    clearTree(m_root);
    m_root = m_null;
    m_root->m_parent = m_null;
}

template<class T, class V>
void RedBlackTree<T, V>::clearTree(node_pointer_type node)
{
    if (m_null == node)
    {
        return;
    }
    clearTree(node->m_left);
    clearTree(node->m_right);
    delete node;
}

template<class T, class V>
void RedBlackTree<T, V>::print()
{
    printTree(m_root);
}

template<class T, class V>
void RedBlackTree<T, V>::printTree(node_pointer_type node)
{
    if (m_null != node)
    {
        if(m_root == node)
        {
            std::cout<< "root = " << m_root->m_key << std::endl;
        }

        std::cout << *node << std::endl;

        printTree(node->m_left);
        printTree(node->m_right);
    }
}

#endif
