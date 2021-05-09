#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {
  return data[0].size();
}

int Block::width() const {
  return data.size();
}

/**
   * Renders the given block onto img with its upper
   * left corner at (x, 0). Assumes block fits on the 
   * image.
   */
void Block::render(PNG &im, int x) const {
  for (int i = 0; i < width(); i++) {
    for (int j = 0; j < height(); j++) {
      *(im.getPixel(x+i, j)) = data[i][j];
      
    }
  }


}

void Block::build(PNG &im, int x, int width) {
  data.resize(width, vector<HSLAPixel>(im.height()));
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < (int) im.height(); j++) {
      data[i][j] = *im.getPixel(x+i,j);
    }
  }
}
