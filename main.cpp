#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "CLSDhTree.h"
#include "CPoint.h"
#include "CBucket.h"
#include <ctime>
#include <ratio>
#include <chrono>

using namespace std;

using namespace std::chrono;

int main()
{
	CLSDhTree<double, 91> myLSDhTree(1000);
    //CLSDhTree<double, 2> myLSDhTree(2);

    ifstream infile( "YearPredictionMSD.txt" );
    //ifstream infile( "N.txt" );

    int x = 0;
    vector <double> r;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    while (infile)
    {
      cout<<x<<" reading"<<endl;
      string s;
      if (!getline( infile, s )) break;

      istringstream ss( s );

      vector <double> record;

      while (ss)
      {
          string s;
          if (!getline( ss, s, ',' )) break;

          string::size_type sz;     // alias of size_t

      double tempNumber = std::stod (s, &sz);

          record.push_back( tempNumber );
      }
      
      myLSDhTree.insert(CPoint<double>(record));

      if (x == 0)
        r = record;

      x++;
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double> >(t2 - t1);

    cout << "It took me " << time_span.count() << " seconds."<<endl;

    //vector <double> record;
    cout<<"Query Point"<<endl;
    for (int i=0; i<r.size(); i++)
        cout<<r[i]<<"; ";
    cout<<endl;

    //myLSDhTree.k_NearestNeighbors(3, CPoint<double>(r));
	

	cout<<"Hello World!"<<endl;
	return 0;
}
