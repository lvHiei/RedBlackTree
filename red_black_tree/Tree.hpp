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
    // 1.�ҵ�Ҫ����ڵ��λ��
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

    // 2. ����һ���½ڵ㣬�����ýڵ�ŵ����Ӧ��λ����
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

    // 3. �����޸�
    fixRedBlackTreeInsert(node);
}

template<class T, class V>
void RedBlackTree<T, V>::deleteFromTree(const key_type& dKey)
{
    // 1. �ҵ�Ҫɾ���ڵ��λ��
    node_pointer_type delNode = findValue(dKey);
    if (m_null == delNode)
    {
        return;
    }

    // 2. �ҵ������ɾ���ڵ�λ�õĽڵ㣨����ֱ�Ӻ�̣�������������������
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

    // 3. ����̽ڵ���ӽڵ㣨���ֻ��һ�����ŵ���̽ڵ��λ��
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

    // 4. ����̽���ֵ�滻Ҫɾ���ڵ��ֵ
    if (delNode != sucessor)
    {
        delNode->m_key = sucessor->m_key;
        delNode->m_value = sucessor->m_value;
        delNode = sucessor;
    }
    
    // 5. ���ɾ���Ľڵ��ֵ�Ǻ�ɫ�ģ����޸������
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
    // node��parent�Ǻ�ɫ�Ļ�����һ����ɫ�Ľڵ㲻��Ӱ������������
    // node��parent�Ǻ�ɫ�Ļ���������null�ڵ�
    // ���node��parent�Ǻ�ɫ�ģ���node��grandparent Ҳ��Ȼ����null�ڵ�
    while (RED == node->m_parent->m_color)
    {
        // node��parent��node��grandparent������
        if(node->m_parent == node->m_parent->m_parent->m_left)
        {
            node_pointer_type uncle = node->m_parent->m_parent->m_right;

            // case1: �������ɫ�Ǻ�ɫ��
            // ��������ϰֵ���ɫ��Ϳ�ڣ���үү����ɫͿ�죬������ǰ�ڵ�����Ϊүү�ڵ����
            if (RED == uncle->m_color)
            {
                uncle->m_color = BLACK;
                node->m_parent->m_color = BLACK;
                node->m_parent->m_parent->m_color = RED;
                node = node->m_parent->m_parent;
            }
            // case 2: �������ɫ�Ǻ�ɫ�ģ�����node���Һ���
            // ����ǰ�ڵ����ó�parent�������Ƹýڵ�����
            else if (node == node->m_parent->m_right)
            {
                node = node->m_parent;
                leftRotate(node);
            }
            // case 3 : �������ɫ�Ǻ�ɫ�ģ�����node������
            // ��parentȾ�ڣ�Ȼ��grandparentȾ�죬Ȼ����grandparent����
            else 
            {
                node->m_parent->m_color = BLACK;
                node->m_parent->m_parent->m_color = RED;
                rightRotate(node->m_parent->m_parent);
            }
        }
        // node��parent��node��grandparent���Һ���
        // ���������һ���������
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
            // ����brother һ������m_null����Ϊ�����ֵ���һ����ɫ�Ľڵ㣬Ҳ����node������ڵ�
            node_pointer_type brother = node->m_parent->m_right;
            if (RED == brother->m_color)
            {
                // case1 
                // ��Ҫɾ���Ľڵ��Ǻ�ɫ��ʱ�򣬽��ֵܵ���ɫȾ�ɺ�ɫ�����ϰֵ���ɫȾ�ɺ�ɫ
                // Ȼ�����ϰֽڵ�������ͬʱ�ֵܽڵ���δ��ת֮ǰ�ֵܵ�����
                // ͬʱ����µ��ֵܽڵ�һ����һ����ɫ�Ľڵ㣬���Ҳ���NULL
                // ��Ϊ֮ǰ�������Ǻ�ɫ�ģ����ϰ��Ǻ�ɫ��
                // ͨ�����������������ı��κ�·���Ϻڽڵ�ĸ��������ǿ��Խ���ǰ�ڵ���ֵ�ת���ɺ�ɫ���Ӷ�������case2,3,4������һ��
                brother->m_color = BLACK;
                node->m_parent->m_color = RED;
                leftRotate(node->m_parent);
                brother = node->m_parent->m_right;
            }

            if ( BLACK == brother->m_left->m_color && BLACK == brother->m_right->m_color)
            {
                // case2 
                // �ֵܽڵ��Ǻ�ɫ�ģ������ֵܽڵ���������Ӷ��Ǻ�ɫ��
                // ���ֵܽڵ�Ⱦ�죬Ȼ��ѵ�ǰ�ڵ�����Ϊ��ǰ�ڵ��˫�׽ڵ�
                // ���ֵܽڵ�Ⱦ�죬����˫�׽ڵ㵽������������֧�ĺ�ɫ�ڵ������ͬ��
                // ���Ƕ���˫�׽ڵ���游�ڵ�����ڽڵ㵽��������֧�Ľڵ�ĺ�ɫ��һ�����������ǰѵ�ǰ�ڵ���Ϊ˫�׽ڵ㣬Ȼ��ѭ��
                // ������ʹ�����˫�غ�ɫ�Ľڵ㣨��ǰ�ڵ㣩�����ڵ㷽��ǰ����һ��
                brother->m_color = RED;
                node = node->m_parent;
            }
            else if(BLACK == brother->m_right->m_color)
            {
                // case 3 
                // �ֵܽڵ��Ǻ�ɫ�ģ��ֵ������Ǻ�ɫ�ģ������Ǻ�ɫ��
                // ���ֵܽڵ�Ⱦ�죬�ֵ�����Ⱦ�ڣ����ֵܽڵ�����
                // ����Ҳ����ı�·���Ϻ�ɫ�ڵ�ĸ��������ǿ��԰��ֵܽڵ���Һ���Ⱦ�ɺ�ɫ
                // ͨ�������������ת����case4
                brother->m_left->m_color = BLACK;
                brother->m_color = RED;
                rightRotate(brother);
            }
            else if (RED == brother->m_right->m_color)
            {
                // case4
                // �ֵܽڵ��Ǻ�ɫ�ģ��ֵ��������⣬�����Ǻ�ɫ��
                // ���ֵܽڵ�Ⱦ��˫�׽ڵ����ɫ����˫�׽ڵ�Ⱦ�ڣ��ֵ�����Ⱦ�ڣ�Ȼ����˫�׽ڵ�����
                // ����������Լ򵥵���Ϊ�������ģ��Լ����ȱ��һ����ɫ�Ľڵ㣬���ֵ��Ǳ�������һ����ɫ�Ľڵ㡣
                // ����ͨ����ת���ֵܽڵ���Ǹ���ɫ�㵽�Լ��������Ȼ�����Ⱦ�ɺ�ɫ�������������˺����������
                // �ѵ�ǰ�ڵ�����Ϊ���ڵ㣬�˳�ѭ��
                brother->m_color = node->m_parent->m_color;
                node->m_parent->m_color = BLACK;
                brother->m_right->m_color = BLACK;
                leftRotate(node->m_parent);
                node = m_root;
            }
        }
        else
        {
            // ����������һ����ͺ���
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
    // ������������
    // 1. ��node���������ŵ�nodeλ��
    // 2. ��node���ó�node����������������
    // 3. ��node�������������������ó�node��������
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
    // ������������������һ��
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
