//#include <stdio.h>
#include <iostream>
#include "dbscan.h"

#define MINIMUM_POINTS 3   // minimum number of cluster
#define EPSILON 10.0// distance for clustering, metre^2


int main()
{    
    vector<PointC> points; 
  
    points.push_back({1,2,UNCLASSIFIED}); 
    points.push_back({1,3,UNCLASSIFIED}); 
    points.push_back({1,1,UNCLASSIFIED});
      
    points.push_back({100,2,UNCLASSIFIED}); 
    points.push_back({100,3,UNCLASSIFIED}); 
    points.push_back({100,1,UNCLASSIFIED});


    points.push_back({100,210,UNCLASSIFIED}); 
    points.push_back({100,211,UNCLASSIFIED}); 
    points.push_back({100,212,UNCLASSIFIED});
    points.push_back({100,213,UNCLASSIFIED});
    points.push_back({100,214,UNCLASSIFIED});
    points.push_back({100,215,UNCLASSIFIED});
    points.push_back({100,220,UNCLASSIFIED});
            

    points.push_back({300,210,UNCLASSIFIED}); 
    points.push_back({300,211,UNCLASSIFIED}); 
    points.push_back({300,212,UNCLASSIFIED});

        

    // constructor
    DBSCAN ds(MINIMUM_POINTS, EPSILON, points);

    // main loop
    cout<<ds.run();
    vector<vector<PointC>>cluster(10);
        for(int i=0;i<ds.m_points.size();i++) {
            if(ds.m_points[i].clusterID >= 0 && ds.m_points[i].clusterID<10)  {
                cluster[ds.m_points[i].clusterID].push_back(ds.m_points[i]);
            }
    }
    
    cout<<cluster[2].size();
    for(auto i:cluster[2])
        cout<<i.x<<","<<i.y<<endl;
    return 0;
}
