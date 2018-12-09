#include <iostream>
#include "TypeList.h"
#include "Reader.h"

using std::cout;
using std::endl;


//prints void* consisting of 4 ints
void print_r(void* r) {
    int *int_r;

    for( int i = 0; i < 4; ++i) {
        int_r = reinterpret_cast<int*>((char*)r + sizeof(int)*i);
        cout << *int_r << endl;
    }
}

//"decompresses" 0 to 1, anything else to 0
class DecInt {
 public:
  static DecOutput decompress(const string& s) {
      int* dec_i = (int*)malloc(sizeof(int));
      int cur = std::stoi(s);
      if (cur == 0)
          *dec_i = 1;
      else
          *dec_i = 0;
      DecOutput decOutput;
      decOutput.first = (void*)dec_i;
      decOutput.second = sizeof(int);
      return decOutput;
  }
};

//"decompresses" 0 to 0, anything else to 5, s is int*
//used with DecInt
void d_int_after(void *s, void *t) {
    int* s_i = (int*)s;
    int* t_i = (int*)t;
    if( *s_i == 0 )
        *t_i = 0;
    else
        *t_i = 5;
}

//"decompresses" 0 to 0, anything else to 5, s is char*;
//used w/o DecInt
void d_int_solo(void *s, void *t) {
    char* s_c = (char*)s;
    int* t_i = (int*)t;

    int s_i = atoi(s_c);
    if( s_i == 0 )
        *t_i = 0;
    else
        *t_i = 5;
}

int main() {

    //TESTING
    //case 1: no decompressors
    //case 2: only typelist decompressor
    //case 3: both
    //case 4: only function decompressor
    using TL = TypeList<int, int, int, int>;
    using DL = TypeList<NoneType, DecInt, DecInt, NoneType>;
    vector<decompress_func*> fs(4);
    fs[2] = d_int_after;
    fs[3] = d_int_solo;


    //For file "10 0 0 0" output is "10 1 5 0"
    Reader<TL, DL> reader("test.txt", fs);
    void *r = reader.readNextLine();
    print_r(r);


    return 0;
}