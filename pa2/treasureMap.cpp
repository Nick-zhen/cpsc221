#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    base = baseim;
    maze = mazeim;
    start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){
    im.getPixel(loc.first,loc.second)->r = 2*((im.getPixel(loc.first,loc.second)->r)/4);
    im.getPixel(loc.first,loc.second)->g = 2*((im.getPixel(loc.first,loc.second)->g)/4);
    im.getPixel(loc.first,loc.second)->b = 2*((im.getPixel(loc.first,loc.second)->b)/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){
    int re = (d >> 4) & 0x3;
    int gr = (d >> 2) & 0x3;
    int bl = d & 0x3;

    im.getPixel(loc.first,loc.second)->r = ((im.getPixel(loc.first,loc.second)->r)&(0xfc)) | re;
    im.getPixel(loc.first,loc.second)->g = ((im.getPixel(loc.first,loc.second)->g)&(0xfc)) | gr;
    im.getPixel(loc.first,loc.second)->b = ((im.getPixel(loc.first,loc.second)->b)&(0xfc)) | bl;
}

PNG treasureMap::renderMap(){
    PNG copyBase = PNG(base);

    // vector<vector<bool>> isVisited;
    // for (int i = 0; i < base.width(); i++) {
    //     for (int j = 0; j < base.height(); j++) {
    //         isVisited[i][j] = false;
    //     }
    // }

    vector<vector<bool>> isVisited;
    for(int i = 0; i < base.width(); i++){
        vector<bool> newIsVisited;
        for(int j = 0; j < base.height(); j++){
            newIsVisited.push_back(false);
        }
        isVisited.push_back(newIsVisited);
    }

    // vector<vector<int>> pathLength;
    // for (int i = 0; i < base.width(); i++) {
    //     for (int j = 0; j < base.height(); j++) {
    //         pathLength[i][j] = 0;
    //     }
    // }
    vector<vector<int>> pathLength;
    for(int i = 0; i < base.width(); i++){
        vector<int> newPathLength;
        for(int j = 0; j < base.height(); j++){
            newPathLength.push_back(0);
        }
        pathLength.push_back(newPathLength);
    }
    Queue<pair<int,int>> q;

    // mark the start location as visited (structure in step 1a)
    isVisited[start.first][start.second] = true;
    // set the distance to the start to be 0 (structure in step 1b)
    pathLength[start.first][start.second] = 0;
    // encode the distance within the start location's lower order bits of the treasure map
    setLOB(copyBase, start, 0);
    // enqueue the start location. (structure in step 2) 
    q.enqueue(start);

    while (!q.isEmpty()) {
        pair<int,int> curr = q.dequeue();
        vector<pair<int,int>> p = neighbors(curr);
        for (int index = 0; index < 4; index++) {
            pair<int,int> realP = p[index];
            if (good(isVisited, curr, realP)) {
                isVisited[realP.first][realP.second] = true;
                pathLength[realP.first][realP.second] = pathLength[curr.first][curr.second] + 1;
                setLOB(copyBase, realP, pathLength[realP.first][realP.second]);
                q.enqueue(realP);
            }
        }
    }
    return copyBase;
}

//draws the entire maze on a copy of the base image by darkening the locations
    // so it's visible as a shadow. This can be done by iterating over every
    // pixel, but it will be faster to begin from the start position
    // and find the points in the maze from there. Returns an image whose 
    // non-maze pixels are the same as the base image, and whose maze pixels 
    // have colour values that are 50% of those in the base image, for each of 
    // red, green and blue. (We specify the computation of the colours in the 
    // description of a setGrey function, below.) 
    //
    // This function also draws a red 7px x 7px square at the start                                                                                    
    // location. If the start location is near the edge of the image,
    // just draw the existing portion of the square. The start 
    // should be at the center of the square.

PNG treasureMap::renderMaze(){
    PNG copyBase = PNG(base);

    for (int i = 0; i < base.width(); i++) {
        for (int j = 0; j < base.height(); j++) {
            if((*maze.getPixel(start.first,start.second)) == (*maze.getPixel(i,j))){
                pair<int,int> loc(i,j);
                setGrey(copyBase,loc);
            }
        }
    }

   
    //draws a red 7px x 7px square at the start                                                                                    
    // location. If the start location is near the edge of the image,
    // just draw the existing portion of the square. The start 
    // should be at the center of the square.
    for (int x = start.first-3; x <= start.first+3; x++) {
        for (int y = start.second-3; y <= start.second+3; y++) {
            if (x >= 0 && y >= 0 && x < copyBase.width() && y < copyBase.height()) {
                RGBAPixel* i = copyBase.getPixel(x,y);
                i->r = 255;
                i->g = 0;
                i->b = 0;
            } 
        }
    }
    return copyBase;

}

// 1. within the image, 2. unvisited, and 3. the same colour as curr in
    // the maze image (member var).
bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next) {
    // bool isWithin = (next.first < base.width()) && (next.second < base.height()) && next.first>=0 && next.second>=0;
    // bool isVisited = (v[next.first][next.second] == false);
    // bool isSameColour = (*maze.getPixel(next.first, next.second) == *maze.getPixel(curr.first, curr.second));

    // return isWithin && isVisited && isSameColour;

    if(next.second < maze.height()  && next.second >= 0 && next.first < maze.width() && next.first >= 0){
        if((v[next.first][next.second]==false)){
            if((*maze.getPixel(next.first, next.second) == *maze.getPixel(curr.first, curr.second))){
                return true;
            }
        }
    }

    return false;
}

// left, below, right, above. 
vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
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

