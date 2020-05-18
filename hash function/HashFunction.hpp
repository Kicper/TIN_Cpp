#include <gcrypt.h>
#include <iostream>
#include <sstream>
#include <iomanip>

template<gcry_md_algos Algo, gcry_md_flags Flags>
class HashFunction {
public:
  HashFunction() {
    gcry_error_t error = gcry_md_open(&handler, Algo, Flags);
    if(error) throw std::runtime_error("Libgcrypt error!");
  }

  ~HashFunction() {
    gcry_md_close(handler);
  }

  void setKey(const std::string& key) {
    gcry_error_t error = gcry_md_setkey(handler, key.c_str(), key.size());
    if(error) throw std::runtime_error("Libgcrypt error!");
  }

  void setText(const std::string& text) {
    gcry_md_write(handler, text.c_str(), text.size());
  }

  unsigned int getHashLength() {
    return gcry_md_get_algo_dlen(Algo);
  }

  unsigned char* getHash() {
    return gcry_md_read(handler, Algo);
  }

  std::string getHexHash() {
    std::ostringstream oss;
    unsigned int hashLength = getHashLength();
    unsigned char* hash = getHash();
    oss << std::setfill('0');
    for(unsigned int i = 0; i < hashLength; ++i){
        oss << std::setw(2) << std::hex << hash[i];
        }
    return oss.str();
  }

private:
  gcry_md_hd_t handler;
};
