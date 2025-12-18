#include <iostream>
#include <iomanip>
namespace chernikov{
  char * getline(std::istream& in, char * data, size_t & size);
  size_t hasSam(const char * str1, const char * str2, const size_t size1, const size_t size2);
  void uniTwo(char * uni_two,const char * str1, const char * str2, size_t size1, size_t size2, size_t & size_ut);
  size_t min(size_t size1, size_t size2, size_t& diff, size_t& flag);
}

int main(){
  char * str1 = nullptr;
  char * str2 = nullptr;
  size_t size1 = 0;
  size_t size2 = 0;
  try {
    str1 = chernikov::getline(std::cin, str1, size1);
    if (size1 == 1 && str1[0] == '\0') {
      std::cerr << "Memory allocation or string allocation error";
      delete[] str1;
      return 1;
    }
    str2 = new char[5];
    str2[0] = 'f';
    str2[1] = 'i';
    str2[2] = 'v';
    str2[3] = 'e';
    size2 = 4;
  }
  catch (...) {
    std::cerr << "Memory allocation or string allocation error";
    delete[] str1;
    delete[] str2;
    return 1;
  }
  size_t has_sam = chernikov::hasSam(str1, str2, size1, size2);
  char* uni_two = nullptr;
  size_t size_uni_two = size1 + size2;
  try {
    uni_two = new char[size1 + size2 + 1];
  }
  catch (...) {
    std::cerr << "Memory allocation or string allocation error";
    delete[] str1;
    delete[] str2;
    return 1;
  }
  chernikov::uniTwo(uni_two, str1, str2, size1, size2, size_uni_two);
  std::cout << "HAS-SAM: " << has_sam << '\n';
  std::cout << "UNI_TWO: " << '\n';
  for (size_t i = 0; i < size_uni_two; ++i) {
    std::cout << uni_two[i];
  }

  std::cout << std::endl;
  delete[] str1;
  delete[] str2;
  delete[] uni_two;
  return 0;
}

char * chernikov::getline(std::istream& in, char* data, size_t & size) {
  if (in.eof()) {
    size = 0;
    data = new char[1];
    data[0] = '\0';
    return data;
  }
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }
  char ch;
  size = 0;
  char * new_data = nullptr;
  data = nullptr;
  bool read_anything = false;
  while (in >> ch && ch != '\n') {
    read_anything = true;
    new_data = new char[size + 2];
    for (size_t i = 0; i < size; ++i) {
      new_data[i] = data[i];
    }
    new_data[size] = ch;
    new_data[size + 1] = '\0';
    delete[] data;
    data = new_data;
    size++;
  }
  if (!read_anything && in.eof()) {
    if (data) {
      delete[] data;
    }
    size = 1;
    data = new char[1];
    data[0] = '\0';
  }
  else if (size == 0) {
    data = new char[1];
    data[0] = '\0';
    size = 1;
  }
  if (is_skipws) {
    in >> std::skipws;
  }
  return data;
}
size_t chernikov::hasSam(const char * str1, const char * str2, const size_t size1, const size_t size2) {
  size_t len1 = (size1 > 0) ? size1 - 1 : 0;
  size_t len2 = (size2 > 0) ? size2 - 1 : 0;
  for (size_t i = 0; i < len1; ++i) {
    for (size_t j = 0; j < len2; ++j) {
      if (str1[i] == str2[j]) {
        return 1;
      }
    }
  }
  return 0;
}
void chernikov::uniTwo(char * uni_two, const char * str1, const char * str2, size_t size1, size_t size2, size_t & size_ut) {
  size_t diff = 0;
  size_t str_flag = 0;
  size_t len1 = (size1 > 0) ? size1 - 1 : 0;
  size_t len2 = (size2 > 0) ? size2 - 1 : 0;
  size_t size = min(len1, len2, diff, str_flag);
  size_t uni_index = 0;
  for (size_t i = 0; i < size; ++i) {
    uni_two[uni_index++] = str1[i];
    uni_two[uni_index++] = str2[i];
  }
  if (diff > 0) {
    if (str_flag == 1) {
      for (size_t i = 0; i < diff; ++i) {
        uni_two[uni_index++] = str1[size + i];
      }
    }
    else if (str_flag == 2) {
      for (size_t i = 0; i < diff; ++i) {
        uni_two[uni_index++] = str2[size + i];
      }
    }
  }
  uni_two[uni_index] = '\0';
  size_ut = uni_index;
}
size_t chernikov::min(size_t size1, size_t size2, size_t & diff, size_t & flag) {
  if (size1 < size2) {
    flag = 2;
    diff = size2 - size1;
    return size1;
  }
  else if (size2 < size1) {
    flag = 1;
    diff = size1 - size2;
    return size2;
  }
  else {
    flag = 0;
    diff = 0;
    return size2;
  }
}
