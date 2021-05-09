// File:        pa3.cpp
// Author:      cs221 instructors
// Date:        2021-03-03
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets

#include "sqtree.h"

int main() {

  // read in image
  PNG orig;
  orig.readFromFile("images/orig/small1.png");
  
  // orig.readFromFile("images/orig/small2.png");
  // orig.readFromFile("images/orig/pacman.png");
  // orig.readFromFile("images/orig/huckleberry_hound.png");
  // orig.readFromFile("images/orig/rocky.png");
  // orig.readFromFile("images/orig/rocky_bullwinkle.png");
  // orig.readFromFile("images/orig/colour.png");
  // orig.readFromFile("images/orig/klee-flora-on-sand.png");
  // orig.readFromFile("images/orig/stanley-totem-poles.png");
  
  
  // // use it to build a sqtree (try other tolerances)
  // SQtree t(orig,10.0);

  // // copy the tree (to show you can)
  // SQtree tCopy(t);

  // // render the sqtree
  // PNG pic = t.render();

  // // write it out
  // pic.writeToFile("images/out/small1.png");

  stats sta(orig);
  for (int i = 0; i < orig.width(); i++) {
    for (int j = 0; j < orig.height(); j++) {
      std::cout << (int)orig.getPixel(i,j)->r<<" ";
    }
     std::cout<< std::endl;
  }

  std::cout<< "stat: sum" <<std::endl;
  for (int i = 0; i < sta.sumRed.size(); i++) {
    for (int j = 0; j < sta.sumRed[0].size(); j++) {
      std::cout << sta.sumRed[i][j] <<" ";
    }
     std::cout<< std::endl;
  }
  std::cout<<"sum sqare: " <<std::endl;
  for (int i = 1; i < sta.sumRed.size(); i++) {
    for (int j = 1; j < sta.sumRed[0].size(); j++) {
      std::cout << sta.sumsqRed[i][j] <<" ";
    }
     std::cout<< std::endl;
  }

  pair<int,int> p;
  p.first = 0;
  p.second = 0;
  std::cout<<"get sum of r: "<<sta.getSum('r',p,2,2) <<std::endl;
  std::cout<<"width: "<< orig.width()<<std::endl;
  std::cout<<"height: "<< orig.height()<<std::endl;
  RGBAPixel rbg = sta.getAvg(p,4,4);
  int rAve = rbg.r;
  
  std::cout<<"get sum average of r: "<<rAve <<std::endl;
  std::cout<<"get variance of r: "<<sta.getVar(p,4,4) <<std::endl;
  return 0;
}
