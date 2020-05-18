#include <iostream>
#include "HashFunction.hpp"

void printHash(
    const std::string& hashFunctionName,
    const std::string& text,
    const std::string& hash) {
  std::cout <<  hashFunctionName << "(\"" << text << "\") = " << hash << std::endl;
}

int main() {
  const std::string text = "TEST";

  typedef HashFunction<GCRY_MD_MD5, GCRY_MD_FLAG_SECURE> MD5;
  MD5 md5Hasher;
  md5Hasher.setText(text);
  printHash("MD5", text, md5Hasher.getHexHash());
}
