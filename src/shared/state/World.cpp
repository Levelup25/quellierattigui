#include "World.h"

namespace state {

	World::World (int i, int j) {
            for(std::size_t l=0;l<j;l++) {
                for(std::size_t k=0;k<j;k++) {
                    Cell* cell();
                    grid[k][l]=cell;
                }
            }
	}

}


