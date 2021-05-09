/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  stats s(imIn);
  pair<int, int> ul(0, 0);
  root = buildTree(s, ul, imIn.width(), imIn.height(), tol);
}

static double getMaxVariance(stats &s, pair<int,int> & ul,
				 int w, int h, int wsplit, int hsplit){
    // the ul w, h rect splited into 4 part
    // (ul, wsplt, hspl) first
    // (x + wsplit, y, w-wsplit, hspl) second
    // (x, y + hsplit, wsplit, h - hspl) third
    // (x + wsplit, y + hspl, w-wsplit, h - hspl) fourth

    double first = 0.0;
    if (wsplit > 0 && hsplit > 0) 
        first = s.getVar(ul, wsplit, hsplit);
    double maxV = first;
    double second = 0.0;
    if (w - wsplit > 0 && hsplit > 0){ 
        pair<int, int> ultemp (ul.first + wsplit, ul.second);
        second = s.getVar(ultemp, w - wsplit, hsplit);
    }
    if (second > maxV) maxV = second;

    double third = 0.0;
    if (wsplit > 0 && h - hsplit > 0){ 
        pair<int, int> ultemp (ul.first, hsplit + ul.second);
        third = s.getVar(ultemp, wsplit, h - hsplit);
    }
    if (third > maxV) maxV = third;

    double fourth = 0.0;
    if (w - wsplit > 0 && h - hsplit > 0){ 
        pair<int, int> ultemp (ul.first + wsplit, hsplit + ul.second);
        fourth = s.getVar(ultemp, w - wsplit, h - hsplit);
    }
    if (fourth > maxV) maxV = fourth;
    return maxV;
}


/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
  if (w == 0 || h == 0) return nullptr;
  
  Node * root = new Node(s, ul, w, h);

  if (s.getVar(ul, w, h) <= tol) return root;
  
  double min_max_var = -1;
  pair<int, int> min_max_xy;
  for (int i = 0; i < w+1; i++)
  {
      for (int j = 0; j < h+1; j++)
      {
        if (i == j && j == 0) continue;
        if (i == w && j == h) continue;
        double maxV;       
        maxV = getMaxVariance(s, ul, w, h, i, j);
        if (min_max_var < 0 || maxV < min_max_var){
          min_max_var = maxV;
          min_max_xy.first = i;
          min_max_xy.second = j;
        }  
        
      }
         
  }
  if (min_max_var >= 0){
      root->NW = buildTree(s, ul, min_max_xy.first, min_max_xy.second, tol);
      pair<int, int> ultemp(ul.first + min_max_xy.first, ul.second);
      root->NE = buildTree(s, ultemp, w - min_max_xy.first, min_max_xy.second, tol);
      pair<int, int> ultemp1(ul.first, ul.second + min_max_xy.second);
      root->SW = buildTree(s, ultemp1, min_max_xy.first, h - min_max_xy.second, tol);
      pair<int, int> ultemp2(ul.first + min_max_xy.first, ul.second + min_max_xy.second);
      root->SE = buildTree(s, ultemp2, w - min_max_xy.first, h - min_max_xy.second, tol);
  }
  std::cout << root->upLeft.first << " " << root->upLeft.second << " (" << w << ", " << h << ")" << std::endl;
  return root;
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG png(root->width, root->height);
  renderNode(png, root);
  return png;
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  clearNode(root);
}

void SQtree::copy(const SQtree & other) {
  copyNode(root, other.root);
}

int SQtree::size() {
  
  return sizeNode(root);
}

void SQtree::clearNode(Node * n){
  if (n == nullptr) return;
  clearNode(n->NW);
  clearNode(n->NE);
  clearNode(n->SW);
  clearNode(n->SE);
  delete n;
}

void SQtree::copyNode(Node * &self, const Node * other){
    if(other == nullptr) {
      self = nullptr;
      return;
    }

    self = new Node(other->upLeft, other->width, other->height, other->avg, other->var);

    copyNode(self->NW, other->NW);
    copyNode(self->NE, other->NE);
    copyNode(self->SW, other->SW);
    copyNode(self->SE, other->SE);

}

int  SQtree::sizeNode(Node *n){
    if (n == nullptr) return 0;
    return 1 + sizeNode(n->NW) + sizeNode(n->NE) + sizeNode(n->SW) + sizeNode(n->SE);
}

void SQtree::renderNode(PNG & png, Node *n){
  if (n == nullptr) return;

  // if the leaf node
  bool leaf = true;
  if (n->NW) {
    leaf = false;
    renderNode(png, n->NW);
  } 

  if (n->NE) {
    leaf = false;
    renderNode(png, n->NE);
  } 

  if (n->SW) {
    leaf = false;
    renderNode(png, n->SW);
  } 

  if (n->SE) {
    leaf = false;
    renderNode(png, n->SE);
  } 

  if (leaf){
    for (int i = 0; i < n->width; i++)
    {
      for (int j = 0; j < n->height; j++)
      { 
        * png.getPixel(i + n->upLeft.first, j + n->upLeft.second) = n->avg;
      }
    }
    
  }
}

