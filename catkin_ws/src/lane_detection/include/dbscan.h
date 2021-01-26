#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include <math.h>

#define UNCLASSIFIED -1
#define CORE_POINT 1
#define BORDER_POINT 2
#define NOISE -2
#define SUCCESS 0
#define FAILURE -3

using namespace std;

typedef struct PointC_
{
    float x, y;  // X, Y position
    int clusterID;  // clustered ID
}PointC;

class DBSCAN {
public: 
    vector<PointC> m_points;
    DBSCAN(unsigned int minPts, float eps, vector<PointC> points){
        m_minPoints = minPts;
        m_epsilon = eps;
        m_points = points;
        m_pointSize = points.size();
    }
    ~DBSCAN(){}

    int run();
    vector<int> calculateCluster(PointC point);
    int expandCluster(PointC point, int clusterID);
    inline double calculateDistance(PointC pointCore, PointC pointTarget);

    int getTotalPointSize() {return m_pointSize;}
    int getMinimumClusterSize() {return m_minPoints;}
    int getEpsilonSize() {return m_epsilon;}
private:
    unsigned int m_pointSize;
    unsigned int m_minPoints;
    float m_epsilon;
};
#endif // DBSCAN_H
