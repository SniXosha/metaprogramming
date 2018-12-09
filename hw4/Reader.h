#pragma once

#include "TypeList.h"

#include <string>
#include <fstream>
#include <string.h>
#include <utility>
#include <vector>
#include <type_traits>

using std::string;
using std::ifstream;
using std::vector;

typedef std::pair<void *, size_t> DecOutput;

//first argument is source, second - target (target is already allocated)
typedef void (decompress_func)(void *, void *);

struct NoneType;

template<typename TL>
void *MallocTL() {
    return malloc(Size<TL>::result);
}

template<typename TL, typename DTL>
void Read(ifstream &file, void *target, vector<decompress_func *> &dec_funcs, size_t i) {
    using H = typename TL::Head;
    using D = typename DTL::Head;
    if constexpr (std::is_same<NoneType, typename DTL::Head>::value) {
        if (dec_funcs[i] == nullptr) {
            H *var = static_cast<H *>(target);
            file >> (*var);
        } else {
            string s;
            file >> s;
            dec_funcs[i]((void *) s.c_str(), target);
        }
    } else {
        string s;
        file >> s;
        DecOutput dec_output = D::decompress(s);

        if (dec_funcs[i] == nullptr) {
            memcpy(target, dec_output.first, dec_output.second);
        } else {
            dec_funcs[i](dec_output.first, target);
        }

        free(dec_output.first);
    }

    void *next = (char *) target + sizeof(H);
    Read<typename TL::Tail, typename DTL::Tail>(file, next, dec_funcs, i + 1);
}

template<>
void Read<TypeList<>, TypeList<>>(ifstream &file, void *target, vector<decompress_func *> &dec_funcs, size_t i) {

}

//Requirements:
// *types of DL must return DecOutput (pointer to decompressed data and size of it)
template<typename TL, typename DL>
class Reader {
 public:
  explicit Reader(const string &filename, vector<decompress_func *> &funcs) {
      file.open(filename);
      dec_funcs = funcs;
  }

  void *readNextLine() {
      void *data = MallocTL<TL>();
      Read<TL, DL>(file, data, dec_funcs, 0);
      return data;
  }

  ~Reader() {
      file.close();
  }

 private:
  ifstream file;
  vector<decompress_func *> dec_funcs;
};