#include "Pen.h"

namespace jlib {


void Pen::UseDark() {
    color_ = 0x00FFFFFF;

}

void Pen::UseLight() {
    color_ = 0x00000000;
}

} // jlib