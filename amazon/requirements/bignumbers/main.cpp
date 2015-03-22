#include <iostream>
#include <sstream>
#include <string>
using namespace std;

inline int min(int a, int b) { return a < b ? a : b; }

typedef unsigned short ushort;

#define BASE 10000
#define MAXN 10000

struct BigNumber {
   BigNumber(int size=MAXN) :
      m_Data(new ushort[size]),
      m_Size(0)
   {
      memset(m_Data, 0, size * sizeof(ushort));
   }

   ~BigNumber() { delete[] m_Data; }

   ushort *m_Data;
   int m_Size;
};

bool equals(BigNumber *a, BigNumber *b) {
   bool areSame = false;

   if (a->m_Size == b->m_Size) {
      bool anyDifference = false;

      for (int i = 0; i < a->m_Size; ++i) {
         if (a->m_Data[i] != b->m_Data[i]) {
            anyDifference = true;
            break;
         }
      }

      areSame = !anyDifference;
   }

   return areSame;
}

void add_numbers(BigNumber *a, BigNumber *b, BigNumber *result) {
   ushort *bigger = a->m_Size > b->m_Size ? a->m_Data : b->m_Data;

   int minSize = min(a->m_Size, b->m_Size);
   int maxSize = a->m_Size + b->m_Size - minSize;

   int i, carry = 0;
   for (i = 0; i < minSize; ++i) {
      int next = a->m_Data[i] + b->m_Data[i] + carry;
      if (next >= BASE) {
         result->m_Data[i] = next - BASE;
         carry = 1;
      }
      else {
         result->m_Data[i] = next;
         carry = 0;
      }
   }

   for (; i < maxSize; ++i) {
      int next = bigger[i] + carry;
      if (next >= BASE) {
         result->m_Data[i] = next - BASE;
         carry = 1;
      }
      else {
         result->m_Data[i] = next;
         carry = 0;
      }
   }

   int size = maxSize;

   if (carry > 0) {
      result->m_Data[i] = carry;
      size++;
   }

   result->m_Size = size;
}

void sub_numbers(BigNumber *a, BigNumber *b, BigNumber *result) {
   // assume a > b

   int carry = 0, i;
   for (i = 0; i < b->m_Size; ++i) {
      int res = a->m_Data[i] - b->m_Data[i] + carry;
      if (res < 0) {
         result->m_Data[i] = BASE + res;
         carry = -1;
      }
      else {
         result->m_Data[i] = res;
         carry = 0;
      }
   }

   while (carry != 0) {
      int temp = a->m_Data[i] + carry;
      if (temp < 0) {
         temp += BASE;
         carry = -1;
      } else {
         carry = 0;
      }

      if (temp != 0) {
         result->m_Data[i] = temp;
         i++;
      }
   }

   result->m_Size = i;
}

void mul_numbers(BigNumber *a, BigNumber *b, BigNumber *result) {
   int carry, i, j;

   for (j = 0; j < b->m_Size; ++j) {
      int bj = b->m_Data[j];
      carry = 0;

      for (i = 0; i < a->m_Size; ++i) {
         int ai = a->m_Data[i];
         int temp = result->m_Data[i + j] + bj * ai + carry;

         result->m_Data[i + j] = temp % BASE;
         carry = temp / BASE;
      }

      // (i + j) is zero
      result->m_Data[i + j] = carry;
   }

   int size = a->m_Size + b->m_Size;
   while (result->m_Data[size] == 0) { size--; }
   result->m_Size = size + 1;
}

ushort my_atoi(const char *data, int sz) {
   int res = 0;
   while (sz--) {
      char a = data[0];
      res = res*10 + (a - '0');
      data++;
   }

   return res;
}

void from_string(const char *number, int size, BigNumber *a) {
   int i = 0;
   int start;

   start = size;

   while (start > 4) {
      int temp = my_atoi(&number[start - 4], 4);
      //std::cout << temp << std::endl;
      a->m_Data[i] = temp;
      start -= 4;
      i++;
   }

   if (start > 0) {
      int temp = my_atoi(&number[0], start);
      //std::cout << temp << std::endl;
      a->m_Data[i++] = temp;
   }

   a->m_Size = i;
}

void to_string(BigNumber *a, std::string &str) {
   stringstream ss;

   ss << a->m_Data[a->m_Size - 1];

   for (int i = a->m_Size - 2; i >= 0; i--) {
      ushort n = a->m_Data[i];

      int len = 0, m = n;
      while (m) {len++; m/=10;}
      len = 4 - len;

      while (len--) { ss << '0'; }

      if (n) { ss << n; }
   }

   str = ss.str();
}

void print_number(BigNumber *a) {
   std::string s;
   to_string(a, s);
   std::cout << s << std::endl;
}

void test_from_string() {
   std::string number = "34198723400000080173489193611324134";
   BigNumber a;
   from_string(number.c_str(), 35, &a);
   std::string result;
   to_string(&a, result);

   if (result != number) {
      std::cout << "to_string fails (expected, actual): " << std::endl << number << std::endl << result << std::endl;
   }
}

void test_add() {
   BigNumber a, b;
   from_string("34198723491874980173489193611324134", 35, &a);
   from_string("98189326491649816398461934619824352", 35, &b);

   BigNumber expected;
   std::string result_str = "132388049983524796571951128231148486";
   from_string(result_str.c_str(), 36, &expected);

   BigNumber actual;
   add_numbers(&a, &b, &actual);

   if (!equals(&expected, &actual)) {
      std::cout << "Error: numbers are different" << std::endl;
   }
}

void test_sub() {
   BigNumber a, b;
   std::string a_str = "234234141314134143134";
   from_string(a_str.c_str(), a_str.length(), &a);
   std::string b_str = "99999999765789291099279270980";
   from_string(b_str.c_str(), b_str.length(), &b);

   BigNumber sum;
   std::string sum_str = "100000000000023432413413414114";
   from_string(sum_str.c_str(), sum_str.length(), &sum);

   BigNumber result;
   sub_numbers(&sum, &a, &result);

   if (!equals(&result, &b)) {
      std::cout << "Sub is not working" << std::endl;
   }
}

void test_mul() {
   BigNumber a, b;
   std::string a_str = "234234141314134143134";
   from_string(a_str.c_str(), a_str.length(), &a);
   std::string b_str = "99999999765789291099279270980";
   from_string(b_str.c_str(), b_str.length(), &b);

   BigNumber mul;
   std::string mul_str = "23423414076553270027465045403771914894270492451320";
   from_string(mul_str.c_str(), mul_str.length(), &mul);

   BigNumber result;
   mul_numbers(&a, &b, &result);

   if (!equals(&mul, &result)) {
      std::cout << "Mul is not working" << std::endl;
   }
}

int main() {
   test_from_string();
   test_add();
   test_sub();
   test_mul();

   std::cout << "DONE" << std::endl;
   return 0;
}
