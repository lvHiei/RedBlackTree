#ifndef  _TREE_H_
#define _TREE_H_

#include <ostream>
#include <sstream>
#include <string>

enum ColorEnum
{
    RED,
    BLACK,
};

template<class T, class V>
class RedBlackTree
{
public:
    typedef T key_type;
    typedef V value_type;

    struct RedBlackNode
    {
        friend class RedBlackTree;
    public:
        typedef RedBlackNode* node_pointer_type;

        RedBlackNode(key_type key = key_type(), value_type val = value_type()):
            m_key(key),
            m_value(val),
            m_color(BLACK),
            m_parent(NULL),
            m_right(NULL),
            m_left(NULL)
        {
        }

        bool isNullNode(node_pointer_type node);

        key_type m_key;
        value_type m_value;
        ColorEnum m_color;
        node_pointer_type m_left;
        node_pointer_type m_right;
        node_pointer_type m_parent;
        
        // 这个地方把函数的实现放在这里的原因是这个类是在一个模板类中的一个类。
        // 没有找到更好的实现方法，就先这样凑合着吧
        friend std::ostream& operator<< (std::ostream& oss, RedBlackNode node )
        {
            ostringstream parentOss;
            ostringstream leftOss;
            ostringstream rightOss;

            if (!node.isNullNode(node.m_parent))
            {
                parentOss << node.m_parent->m_key;
            }
            else
            {
                parentOss << "NULL";
            }

            if (!node.isNullNode(node.m_left))
            {
                leftOss << node.m_left->m_key;
            }
            else
            {
                leftOss << "NULL";
            }

            if (!node.isNullNode(node.m_right))
            { 
                rightOss << node.m_right->m_key;
            }
            else
            {
                rightOss << "NULL";
            }

            oss << "key = " << node.m_key 
                << ", value = " << node.m_value 
                << ", color = " << (node.m_color == RED ? "RED" : "BLACK")
                << ", parent = " << parentOss.str()
                << ", left = " << leftOss.str()
                << ", right = " << rightOss.str()
                ;

            return oss;
        }
    };

    typedef RedBlackNode* node_pointer_type;

public:
    RedBlackTree();
    ~RedBlackTree();
    
    bool isEmpty();
    void insertToTree(const key_type& iKey, const value_type& iValue);
    void deleteFromTree(const key_type& dKey);
    node_pointer_type findValue(const key_type& fKey);
    bool hasKey(const key_type& fKey);
    void clear();
    void print();
private:
    void leftRotate(node_pointer_type node);
    void rightRotate(node_pointer_type node);

    node_pointer_type getSuccessor(node_pointer_type node);

    void fixRedBlackTreeInsert(node_pointer_type node);    
    void fixRedBlackTreeDelete(node_pointer_type node);    

    void printTree(node_pointer_type node);
    void clearTree(node_pointer_type node);

private:
    node_pointer_type m_root;
    node_pointer_type m_null;
};

#endif