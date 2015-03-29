#include <iostream>
#include <cstdlib>
#include <ctime>

class Node;

class MyQueue {
public:
   MyQueue() :
      m_Buffer(new Node*[10]),
      m_Size(100),
      m_Count(0),
      m_First(0),
      m_Last(0)
   {}

   ~MyQueue() { delete m_Buffer; }

public:
   int count() const { return m_Count; }
   bool isEmpty() const { return m_Count == 0; }

   void push(Node *v) {
      m_Buffer[m_Last % m_Size] = v;
      m_Last++;
      m_Count++;
   }

   Node *pop() {
      Node *v = m_Buffer[m_First % m_Size];
      m_First += 1;
      m_Count--;
      return v;
   }

private:
   Node **m_Buffer;
   int m_Size;
   int m_Count;
   int m_First;
   int m_Last;
};

class Node {
public:
   Node (int value, Node *parent, Node *left, Node *right):
      m_Value(value),
      m_Parent(parent),
      m_Left(left),
      m_Right(right)
   { }

public:
   Node *successor() {
      Node *nextNode = NULL;
      Node *node = this;

      if (node->m_Right != NULL) {
         nextNode = node->m_Right->findMinimum();
      } else {
         Node *parent = node->m_Parent;

         while (parent != NULL && node == parent->m_Right) {
            node = parent;
            parent = node->m_Parent;
         }

         nextNode = parent;
      }

      return nextNode;
   }

   Node *predecessor() {
      Node *prevNode = NULL;
      Node *node = this;

      if (node->m_Left != NULL) {
         prevNode = node->m_Left->findMaximum();
      } else {
         Node *parent = node->m_Parent;
         while (parent != NULL && parent->m_Right == node) {
            node = parent;
            parent = node->m_Parent;
         }
      }

      return prevNode;
   }

   Node *findMinimum() {
      Node *node = this;

      while (node->m_Left != NULL) {
         node = node->m_Left;
      }

      return node;
   }

   Node *findMaximum() {
      Node *node = this;

      while (node->m_Right != NULL) {
         node = node->m_Right;
      }

      return node;
   }

public:
   int m_Value;
   Node *m_Parent;
   Node *m_Left;
   Node *m_Right;
};

class BinarySearchTree {
public:
   BinarySearchTree() :
      m_Root(NULL)
   { }

public:
   Node *addValue(int value) {
      Node *placeToAdd = m_Root;
      Node *parent = placeToAdd;

      while (placeToAdd != NULL) {
         parent = placeToAdd;
         if (placeToAdd->m_Value > value) {
            placeToAdd = placeToAdd->m_Left;
         } else {
            placeToAdd = placeToAdd->m_Right;
         }
      }

      Node *newNode = new Node(value, parent, NULL, NULL);

      if (m_Root == NULL) {
         m_Root = newNode;
      } else if (parent->m_Value > value) {
         parent->m_Left = newNode;
      } else {
         parent->m_Right = newNode;
      }

      return newNode;
   }

   Node *findValue(int value) {
      Node *node = m_Root;

      while (node != NULL && node->m_Value != value) {
         if (node->m_Value < value) {
            node = node->m_Left;
         } else {
            node = node->m_Right;
         }
      }

      return node;
   }

   void deleteNode(Node *nodeToDelete) {
      Node *parent = nodeToDelete->m_Parent;

      if (nodeToDelete->m_Right == NULL) {
         swapNodes(nodeToDelete->m_Left, nodeToDelete);
      } else if (nodeToDelete->m_Left == NULL) {
         swapNodes(nodeToDelete->m_Right, nodeToDelete);
      } else {
         Node *nextNode = nodeToDelete->m_Right->findMinimum();

         if (nextNode->m_Parent != nodeToDelete) {
            swapNodes(nextNode->m_Right, nextNode);
            nextNode->m_Right = nodeToDelete->m_Right;
            nodeToDelete->m_Right->m_Parent = nextNode;
         }

         swapNodes(nextNode, nodeToDelete);
         nextNode->m_Left = nodeToDelete->m_Left;
         nodeToDelete->m_Left->m_Parent = nextNode;
      }

      delete nodeToDelete;
   }

   void printByLevels() {
      MyQueue queue;
      int currCount = 0, nextCount = 0;
      queue.push(m_Root);
      currCount = 1;

      while (!queue.isEmpty()) {
         Node *node = queue.pop();
         currCount--;

         if (node) {
            std::cout << node->m_Value;

            queue.push(node->m_Left);
            queue.push(node->m_Right);
            nextCount += 2;
         } else {
            std::cout << ".";
         }

         std::cout << " ";

         if (currCount == 0) {
            std::cout << std::endl;

            currCount = nextCount;
            nextCount = 0;
         }
      }
   }

   Node *findLCA(int n1, int n2) {
      Node *node = m_Root;

      while (node != NULL) {
         if (node->m_Value > n1 && node->m_Value > n2) {
            node = node->m_Left;
         } else if (node->m_Value < n1 && node->m_Value < n2) {
            node = node->m_Right;
         } else {
            break;
         }
      }

      return node;
   }

   Node *findMinimum() {
      Node *minNode = NULL;

      if (m_Root != NULL) {
         minNode = m_Root->findMinimum();
      }

      return minNode;
   }

   Node *findMaximum() {
      Node *maxNode = NULL;

      if (m_Root != NULL) {
         maxNode = m_Root->findMaximum();
      }

      return maxNode;
   }

private:
   void swapNodes(Node *newNode, Node *oldNode) {
      if (oldNode->m_Parent == NULL) {
         m_Root = newNode;
      } else if (oldNode == oldNode->m_Parent->m_Left) {
         oldNode->m_Parent->m_Left = newNode;
      } else {
         oldNode->m_Parent->m_Right = newNode;
      }

      if (newNode != NULL) {
         newNode->m_Parent = oldNode->m_Parent;
      }
   }

private:
   Node *m_Root;
};

int main() {
   BinarySearchTree tree;
   srand(time(0));

   int arr[] = {10, 5, 2, 1, 0, 3, 4, 7, 6, 8, 9, 15, 12, 11, 13, 14, 17, 16, 18, 19};
   int n = 20, a = 0;

   while (n--) {
      //a = rand() % 20;
      tree.addValue(arr[a++]);
   }

   tree.printByLevels();

   Node *lca = tree.findLCA(11, 14);
   if (lca) {
      std::cout << "LCA is " << lca->m_Value << std::endl;
   } else {
      std::cout << "No LCA" << std::endl;
   }

   return 0;
}
