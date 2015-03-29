#include <iostream>
#include <utility>
#include <cstdio>
#include <string>
#include <vector>

bool is_delimiter(char c) {
   switch(c) {
      case ' ':
      case '\n':
      case '\t':
      case '\r':
         return true;
      default:
         return false;
   }
}

void str_tokenize(const std::string &text, std::vector<std::pair<int, int> > &tokens) {
   int i = 0;
   int size = text.size();
   int lastStart = -1;

   while (i < size) {
      char c = text[i];
      bool isDelimiter = is_delimiter(c);

      if (!isDelimiter) {
         if (lastStart == -1) {
            lastStart = i;
         }
      } else {
         if (lastStart != -1) {
            tokens.push_back(std::make_pair(lastStart, i - 1));
            lastStart = -1;
         }
      }

      i++;
   }

   if (lastStart != -1) {
      tokens.push_back(std::make_pair(lastStart, size - 1));
   }
}

void print_tokens(const std::string &text, std::vector<std::pair<int, int> > &tokens) {
   int size = tokens.size();

   for (int i = 0; i < size; ++i) {
      std::pair<int, int> p = tokens[i];
      for (int j = p.first; j <= p.second; ++j) {
         printf("%c", text[j]);
      }

      printf("\n");
   }
}

int main() {
   std::string s;
   std::getline(std::cin, s);
   std::vector<std::pair<int, int> > tokens;

   str_tokenize(s, tokens);
   print_tokens(s, tokens);

   return 0;
}
