#include "Cell.h"

namespace state {

    Cell::Cell (std::size_t i, std::size_t j) {
            this->i=i;
            this->j=j;
            this->tileContent=nothing;
            this->tileElement=neutral;
    }

}
