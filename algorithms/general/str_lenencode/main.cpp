#include <iostream>
#include <string>
#include <sstream>

std::string deduplicate(const std::string &line) {
  std::string s = line;
  size_t i = 0, j = 0;
  while (j < s.size()) {
    char c = s[j];
    while (j < s.size() && s[j] == c) { j++; }
    s[i++] = c;
  }
  s.resize(i);
  return s;
}

std::string lenencode(const std::string &s) {
  std::stringstream ss;
  size_t i = 0, j = 0;
  while (j < s.size()) {
    char c = s[j];
    size_t n = 0;
    while (j < s.size() && s[j] == c) { j++; n++; }
    ss << c;
    ss << n;
  }
  return ss.str();
}

int main() {
  std::string tests[] = {"a", "abcd", "aabbccdd", "aaabbcccddd", 
                         "abcddd", "abcdd", "aabcd", "aaabcd"};
  for (size_t i = 0; i < 8; i++) {
    std::string &s = tests[i];
    std::cout << s << " " << deduplicate(s) << " " << lenencode(s) << std::endl;
  }
  return 0;
}
