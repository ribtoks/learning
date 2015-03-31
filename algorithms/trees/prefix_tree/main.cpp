#include <iostream>
#include <cstring>
#include <string>

class Node {
public:
   Node(const char *s, int len) :
      m_KeyLength(len),
      m_Child(NULL),
      m_Sibling(NULL)
   {
      m_Key = new char[len];
      strncpy(m_Key, s, len);
   }

   ~Node() { delete[] m_Key; }

public:
   void splitNode(int k) {
      Node *splitter = new Node(m_Key + k, m_KeyLength - k);
      splitter->m_Child = m_Child;
      m_Child = splitter;

      char *keyPrefix = new char[k];
      strncpy(keyPrefix, m_Key, k);

      delete[] m_Key;
      m_Key = keyPrefix;
      m_KeyLength = k;
   }

   void mergeWithChild() {
      Node *node = m_Child;

      char *key = new char[m_KeyLength + node->m_KeyLength];
      strncpy(key, m_Key, m_KeyLength);
      strncpy(key + m_KeyLength, node->m_Key, node->m_KeyLength);

      delete[] m_Key;
      m_Key = key;
      m_KeyLength += node->m_KeyLength;

      m_Child = m_Child->m_Child;
      delete m_Child;
   }

public:
   char *m_Key;
   int m_KeyLength;
   Node *m_Child;
   Node *m_Sibling;
};

int findCommonPrefixLength(const char *s1, int len1, const char *s2, int len2) {
   int i = 0;
   while (i < len1 && i < len2) {
      if (s1[i] != s2[i]) {
         break;
      }

      i++;
   }

   return i;
}

class PrefixTree {
public:
   PrefixTree() :
      m_Root(NULL)
   { }

public:
   Node *findValue(const char *text, int textLength) {
      if (textLength == 0) {
         textLength = strlen(text) + 1;
      }

      Node *node = doFindNode(m_Root, text, textLength);
      return node;
   }

   Node *insertValue(const char *text, int textLength) {
      Node *result = NULL;

      if (m_Root == NULL) {
         m_Root = new Node(text, textLength);
         result = m_Root;
      } else {
         result = doInsertNode(m_Root, text, textLength);
      }

      return result;
   }

   Node *deleteValue(const char *text, int textLength) {
      Node *deleted = doDeleteNode(m_Root, text, textLength);
      return deleted;
   }

private:
   Node *doFindNode(Node *node, const char *text, int textLength) {
      Node *result = NULL;

      if (node == NULL) { return NULL; }

      int commonLength = findCommonPrefixLength(
         node->m_Key, node->m_KeyLength,
         text, textLength);

      if (commonLength == 0) {
         result = doFindNode(node->m_Sibling, text, textLength);
      } else if (commonLength == textLength) {
         result = node;
      } else if (commonLength == node->m_KeyLength) {
         result = doFindNode(node->m_Child, text + commonLength, textLength - commonLength);
      }

      return result;
   }

   Node *doInsertNode(Node *node, const char *text, int textLength) {
      if (node == NULL) {
         node = new Node(text, textLength);
         return node;
      }

      int commonLength = findCommonPrefixLength(node->m_Key, node->m_KeyLength, text, textLength);

      if (commonLength == 0) {
         node->m_Sibling = doInsertNode(node->m_Sibling, text, textLength);
      } else if (commonLength < textLength) {
         // if text fits in current node
         if (commonLength < node->m_KeyLength) {
            node->splitNode(commonLength);
         }

         node->m_Child = doInsertNode(node->m_Child, text + commonLength, textLength - commonLength);
      }

      return node;
   }

   Node *doDeleteNode(Node *node, const char *text, int textLength) {
      if (node == NULL) { return NULL; }

      Node *result = NULL;

      int commonLength = findCommonPrefixLength(node->m_Key, node->m_KeyLength, text, textLength);

      if (commonLength == textLength) {
         result = node->m_Sibling;
         delete node;
      } else if (commonLength == 0) {
         result = doDeleteNode(node->m_Sibling, text, textLength);
      } else if (commonLength == node->m_KeyLength) {
         node->m_Child = doDeleteNode(node->m_Child, text + commonLength, textLength - commonLength);

         if (node->m_Child != NULL &&
             node->m_Child->m_Sibling == NULL) {
            node->mergeWithChild();
         }
      }

      return node;
   }

private:
   Node *m_Root;
};

int main() {

   return 0;
}
