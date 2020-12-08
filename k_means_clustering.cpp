#include <ctime>     // for a random seed
#include <fstream>   // for file-reading
#include <iostream>  // for file-reading
#include <sstream>   // for file-reading
#include <vector>

using namespace std;

//Representing a dataset
struct Point {
    double x, y;     // coordinates
    int cluster;     // no default cluster
    double minDist;  // default infinite dist to nearest cluster

    Point() : 
        x(0.0), 
        y(0.0),
        cluster(-1),
        minDist(__DBL_MAX__) {}
        
    Point(double x, double y) : 
        x(x), 
        y(y),
        cluster(-1),
        minDist(__DBL_MAX__) {}

    double distance(Point p) {
        return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
    }
};

vector<Point> readcsv() {

//link to get the csv file https://github.com/robertmartin8/udemyML/blob/master/06_clustering/Mall_Customers.csv
    //Link to documentation https://reasonabledeviations.com/2019/10/02/k-means-in-cpp/
    vector<Point> points;
    string line;
    ifstream file("mall_data.csv");

    while (getline(file, line)) {
        stringstream lineStream(line);
        string bit;
        double x, y;
        getline(lineStream, bit, ',');
        x = stof(bit);
        getline(lineStream, bit, '\n');
        y = stof(bit);

        points.push_back(Point(x, y));
    }
    return points;
}

void kMeansClustering(vector<Point>* points, int epochs, int k)
{
  //initialising the clusters
  vector<Point> centroids;
  srand(time(0));  // need to set the random seed
  for (int i = 0; i < k; ++i) {
      centroids.push_back(points->at(rand() % n));
  }

  //assigning points to a cluster
  for (vector<Point>::iterator c = begin(centroids); 
       c != end(centroids); ++c) {
      // quick hack to get cluster index
      int clusterId = c - begin(centroids);

      for (vector<Point>::iterator it = points->begin();
           it != points->end(); ++it) {

          Point p = *it;
          double dist = c->distance(p);
          if (dist < p.minDist) {
              p.minDist = dist;
              p.cluster = clusterId;
          }
          *it = p;
      }
  }

  //Computing new centroids
  vector<int> nPoints;
  vector<double> sumX, sumY;

  // Initialise with zeroes
  for (int j = 0; j < k; ++j) {
      nPoints.push_back(0);
      sumX.push_back(0.0);
      sumY.push_back(0.0);
  }

  // Iterate over points to append data to centroids
  for (vector<Point>::iterator it = points->begin(); 
       it != points->end(); ++it) {
      int clusterId = it->cluster;
      nPoints[clusterId] += 1;
      sumX[clusterId] += it->x;
      sumY[clusterId] += it->y;

      it->minDist = __DBL_MAX__;  // reset distance
  }

  // Compute the new centroids
  for (vector<Point>::iterator c = begin(centroids); 
       c != end(centroids); ++c) {
      int clusterId = c - begin(centroids);
      c->x = sumX[clusterId] / nPoints[clusterId];
      c->y = sumY[clusterId] / nPoints[clusterId];
  }
  
  //writing to a file
  ofstream myfile;
  myfile.open("output.csv");
  myfile << "x,y,c" << endl;

  for (vector<Point>::iterator it = points->begin(); 
     it != points->end(); ++it) {
    myfile << it->x << "," << it->y << "," << it->cluster << endl;
  }
  myfile.close();

}

int main()
{
  vector<Point> points = readcsv();
  int epochs=3,k=5;
  // epochs is the number of iterations over which we will do our main k-means loop
  // k is the number of clusters.
  //Define it according to the situation
  kMeansClustering(&points,epochs,k); // pass address of points to function
  return 0;
}
