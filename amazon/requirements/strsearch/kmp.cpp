#include <iostream>
#include <string>

void calculate_prefix_function(const std::string &s, int *p) {
   int n = s.length();

   p[0] = 0;
   for (int i = 1; i < n; ++i) {
      // length of max prefix equal to suffix of s[0..i]
      int prefix = p[i - 1];
      char c = s[i];

      // prefix + one symbol equals suffix + current symbol
      if (s[prefix] == c) {
         p[i] = prefix + 1;
      }
      else {
         while (prefix > 0) {
            // prefix of this length is not equal to smaller suffix + current symbol
            if (c != s[prefix]) {
               // check next prefix with smaller length
               prefix = p[prefix - 1];
            }
            else {
               prefix = prefix + 1;
               break;
            }
         }

         // prefix is either 0 or prefix + 1
         p[i] = prefix;
      }
   }
}

int main() {
   std::string text;
   std::string pattern;

   std::cin >> text;
   std::cin >> pattern;

   int n = pattern.length();
   int *p = new int[n];
   memset(p, 0, n*sizeof(int));
   calculate_prefix_function(pattern, p);

   int m = text.length();
   int prefix = 0;

   for (int i = 0; i < m; ++i) {
      char c = text[i];

      // same as above in calculate_prefix_function
      if (pattern[prefix] == c) {
         prefix++;
      }
      else {
         while (prefix > 0) {
            if (c != pattern[prefix]) {
               prefix = p[prefix - 1];
            }
            else {
               prefix = prefix + 1;
               break;
            }
         }
      }

      if (prefix == n) {
         std::cout << i - n + 1 << " ";
         // same as prefix = 0;
         prefix = p[prefix];
         std::cout << "(" << prefix <<  ") ";
      }
   }

   delete[] p;

   return 0;
}
