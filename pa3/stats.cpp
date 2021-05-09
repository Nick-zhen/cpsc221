#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){
    int h = im.height();
    int w = im.width();
    RGBAPixel * rgb;
    unsigned char r, g, b;
    for (int i = 0; i < h; i++)
    {
        vector<long> x(w);
        sumRed.push_back(x);
        sumGreen.push_back(x);
        sumBlue.push_back(x);
        sumsqRed.push_back(x);
        sumsqGreen.push_back(x);
        sumsqBlue.push_back(x);
        if (i == 0){ // special case for the first row
            rgb = im.getPixel(0, i);
            
            r = rgb->r;
            g = rgb->g;
            b = rgb->b;
            sumRed[0][0] = (r);
            sumGreen[0][0] = (g);
            sumBlue[0][0] = (b);
            sumsqRed[0][0] = (r*r);
            sumsqGreen[0][0] = (g*g);
            sumsqBlue[0][0] = (b*b);  
            for (int j = 1; j < w; j++)
            {
                rgb = im.getPixel(j, i);
                double r = rgb->r;
                double g = rgb->g;
                double b = rgb->b;
                sumRed[i][j] = (r + sumRed[i][j-1]);
                sumGreen[i][j] = (g + sumGreen[i][j-1]);
                sumBlue[i][j] = (b + sumBlue[i][j-1]);
                sumsqRed[i][j] = (r*r + sumsqRed[i][j-1]);
                sumsqGreen[i][j] = (g*g + sumsqGreen[i][j-1]);
                sumsqBlue[i][j] = (b*b + sumsqBlue[i][j-1]);
            }                     
        } else{
        
            rgb = im.getPixel( 0, i);        
            r = rgb->r;
            g = rgb->g;
            b = rgb->b;
            sumRed[i][0] = (r + sumRed[i-1][0]);
            sumGreen[i][0] = (g + sumGreen[i-1][0]);
            sumBlue[i][0] = (b + sumBlue[i-1][0]);
            sumsqRed[i][0] = (r*r + sumsqRed[i-1][0]);
            sumsqGreen[i][0] = (g*g + sumsqGreen[i-1][0]);
            sumsqBlue[i][0] = (b*b + sumsqBlue[i-1][0]);
            
            for (int j = 1; j < w; j++)
            {
                rgb = im.getPixel(j, i);
                r = rgb->r;
                g = rgb->g;
                b = rgb->b;
                sumRed[i][j] = r + sumRed[i-1][j] + sumRed[i][j-1] - sumRed[i-1][j-1];
                sumGreen[i][j] = g + sumGreen[i-1][j] + sumGreen[i][j-1] - sumGreen[i-1][j-1];
                sumBlue[i][j] = b + sumBlue[i-1][j] + sumBlue[i][j-1] - sumBlue[i-1][j-1];
                sumsqRed[i][j] = r*r + sumsqRed[i-1][j] + sumsqRed[i][j-1] - sumsqRed[i-1][j-1];
                sumsqGreen[i][j] = g*g + sumsqGreen[i-1][j] + sumsqGreen[i][j-1] - sumsqGreen[i-1][j-1];
                sumsqBlue[i][j] = b*b + sumsqBlue[i-1][j] + sumsqBlue[i][j-1] - sumsqBlue[i-1][j-1];
            }
        }
        
    }
    
    

}

static long getValue(const vector<vector <long>>& data, pair<int,int> ul, int w, int h){
    int x = ul.first;
    int y = ul.second;
    if (x > 0 && y > 0){
        return data[y+h-1][x+w-1] - data[y+h-1][x-1] - data[y-1][x+w-1] + data[y-1][x-1];
    }
    
    if (x > 0 && y == 0){
        return data[h-1][x+w-1] - data[h-1][x-1];
    }

    if (x == 0 && y > 0){
        return data[y + h-1][w-1] - data[y-1][w-1];
    }

    if (x == 0 && y == 0){
        return data[h-1][w-1];
    }

    
    return 0.0;
}

long stats::getSum(char channel, pair<int,int> ul, int w, int h){
   
    
    if (channel == 'r'){
        return getValue(sumRed, ul, w, h);
    }

    if (channel == 'g'){
        return getValue(sumGreen, ul, w, h);
    }

    if (channel == 'b'){
        return getValue(sumBlue, ul, w, h);
    }
    return 0;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    if (channel == 'r'){
        return getValue(sumsqRed, ul, w, h);
    }

    if (channel == 'g'){
        return getValue(sumsqGreen, ul, w, h);
    }

    if (channel == 'b'){
        return getValue(sumsqBlue, ul, w, h);
    }
    return 0;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    double xsqsum = getSumSq('r', ul, w, h);
            xsqsum += getSumSq('g', ul, w, h);
            xsqsum += getSumSq('b', ul, w, h);
    double rsum = getSum('r', ul, w, h); 
    double gsum = getSum('g', ul, w, h);
    double bsum = getSum('b', ul, w, h);
    
    return xsqsum - (rsum * rsum + gsum * gsum + bsum*bsum) / (w*h);

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    RGBAPixel rgb;
    rgb.r = getSum('r', ul, w, h) / (w*h);
    rgb.b = getSum('b', ul, w, h) / (w*h);
    rgb.g = getSum('g', ul, w, h) / (w*h);
    return rgb;
}
