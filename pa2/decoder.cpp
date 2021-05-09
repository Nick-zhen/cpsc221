#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

    PNG copyBase = PNG(mapImg);
    vector<vector<bool>> isVisited;
    for(int i = 0; i < (int)mapImg.width(); i++){
        vector<bool> newIsVisited;
        for(int j = 0; j < (int)mapImg.height(); j++){
            newIsVisited.push_back(false);
        }
        isVisited.push_back(newIsVisited);
    }

    
    vector<vector<int>> pathLength;
    
    for(int i = 0; i < (int)mapImg.width(); i++){
        vector<int> newPathLength;
        for(int j = 0; j < (int)mapImg.height(); j++){
            newPathLength.push_back(0);
        }
        pathLength.push_back(newPathLength);
    }

    vector<vector<pair<int,int>>> prevPts;
    for(int i = 0; i < (int)mapImg.width(); i++){
      vector<pair<int,int>> newPrevPts;
      for(int j = 0; j < (int)mapImg.height(); j++){
         newPrevPts.push_back(pair<int, int> (-1, -1));
      }
      prevPts.push_back(newPrevPts);
    }

    Queue<pair<int,int>> q;

    isVisited[start.first][start.second] = true;
    pathLength[start.first][start.second] = 0;
    q.enqueue(start);

    int maxLength = 0;
    pair<int,int> maxPts;
    maxPts.first = 0;
    maxPts.second = 0;

    while (!q.isEmpty()) {
        pair<int,int> curr = q.dequeue();
        vector<pair<int,int>> p = neighbors(curr);
        for (int index = 0; index < 4; index++) {
            pair<int,int> realP = p[index];
            if (good(isVisited, pathLength, curr, realP)) {
               isVisited[realP.first][realP.second] = true;
               int length = pathLength[curr.first][curr.second] + 1;
               pathLength[realP.first][realP.second] = length;
               pair<int,int> cur(curr.first, curr.second);
               prevPts[realP.first][realP.second] = cur;
               pathsPts.push_back(realP);
               q.enqueue(realP);

               if (length > maxLength) {
                    maxLength = length;
                    maxPts = realP;
               }
            }
        }
    }

    pair<int,int> temp = maxPts;
    while (temp != start) {
       pathPts.push_back(temp);
       temp = prevPts[temp.first][temp.second];
    }
    pathPts.push_back(temp);
}

PNG decoder::renderSolution() {
    // PNG copyBase = PNG(mapImg);

    PNG copy = PNG(mapImg);
    for (pair<int,int> each : pathPts) {
      
         RGBAPixel* i = copy.getPixel(each.first,each.second);
         i->r = 255;
         i->g = 0;
         i->b = 0;
    }

    return copy;
}

PNG decoder::renderMaze(){
    PNG copy = PNG(mapImg);

    for (int i = 0; i < copy.width(); i++) {
        for (int j = 0; j < copy.height(); j++) {
            if((*copy.getPixel(start.first,start.second)) == (*copy.getPixel(i,j))){
                pair<int,int> loc(i,j);
                setGrey(copy,loc);
            }
        }
    }
//    for (pair<int,int> each : pathsPts) {
//       setGrey(copy,each);
//    }

   for (int x = start.first-3; x <= start.first+3; x++) {
        for (int y = start.second-3; y <= start.second+3; y++) {
            if (x >= 0 && y >= 0 && x < copy.width() && y < copy.height()) {
                RGBAPixel* i = copy.getPixel(x,y);
                i->r = 255;
                i->g = 0;
                i->b = 0;
            } 
        }
    }
    return copy;
}

void decoder::setGrey(PNG & im, pair<int,int> loc){
   im.getPixel(loc.first,loc.second)->r = 2*((im.getPixel(loc.first,loc.second)->r)/4);
   im.getPixel(loc.first,loc.second)->g = 2*((im.getPixel(loc.first,loc.second)->g)/4);
   im.getPixel(loc.first,loc.second)->b = 2*((im.getPixel(loc.first,loc.second)->b)/4);
}

pair<int,int> decoder::findSpot(){
   return pathPts[pathPts.size() - 1];
}

int decoder::pathLength(){
   return pathPts.size();
}

//1. within the image, 2. unvisited, and 3. coloured so that
    // lower order bits fit the pattern prescribed by the encoder.
bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
//    bool isWithin = (next.first <= mapImg.width()) && (next.second <= mapImg.height());
//    bool isVisited = (v[next.first][next.second] == false);
//    bool compared = compare(*mapImg.getPixel(next.first,next.second),d[curr.first][curr.second]);

//    return isWithin && isVisited && compared;

   if(next.second < mapImg.height()  && next.second >= 0 && next.first < mapImg.width() && next.first >= 0){
        if((v[next.first][next.second]==false)){
            if(compare(*mapImg.getPixel(next.first,next.second),d[curr.first][curr.second])) {
                return true;
            }
        }
    }

    return false;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
    vector<pair<int,int>> locations;
    
    pair<int,int> left;
    pair<int,int> below;
    pair<int,int> right;
    pair<int,int> above;

    left.first = curr.first-1;
    left.second = curr.second;

    below.first = curr.first;
    below.second = curr.second+1;

    right.first = curr.first+1;
    right.second = curr.second;

    above.first = curr.first;
    above.second = curr.second-1;
    
    locations.push_back(left);
    locations.push_back(below);
    locations.push_back(right);
    locations.push_back(above);

    return locations;
}


bool decoder::compare(RGBAPixel p, int d){

   int re = (p.r % 4) * 16;
   int gr = (p.g % 4) * 4;
   int bl = p.b % 4;
    int x = ((re+gr+bl)%64);
    int y = (d+1)%64;
   if ( x == y) {
       return true;
   }
   return false;
}
