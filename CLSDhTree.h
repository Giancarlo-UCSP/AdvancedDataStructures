#ifndef CLSDHTREE_H
#define CLSDHTREE_H

#include "CDirectoryNode.h"
#include "CPoint.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <queue>
#include <cmath>

using namespace std;
using namespace std::chrono;

template <typename T>
class CDistanceCalculator
{
public:
	T pointDistance(CPoint<T> point1, CPoint<T> point2);	//Euclidean distance.
	T dataRegionDistance(CNode* dataRegionReceived, CPoint<T> pointReceived);

};

template <typename T>
T CDistanceCalculator<T>::pointDistance(CPoint<T> point1, CPoint<T> point2)
{
    T distance = 0;

    for (int i=0; i < point1.getDimensionOfData(); i++)
    {
        distance = distance + (pow(point1.data[i] - point2.data[i],2));
    }

    distance = sqrt(distance);

    return distance;
}

template <typename T>
T CDistanceCalculator<T>::dataRegionDistance(CNode* dataRegionReceived, CPoint<T> pointReceived)
{
	T distance = 0;
	
	vector<T> middlePoint = (dataRegionReceived->getMiddlePoint());

	for (int i=0; i < pointReceived.getDimensionOfData(); i++)
	{
		distance = distance + (pow( abs( pointReceived.data[i] - middlePoint[i] ) - ((dataRegionReceived->getSizeOfDimensions()[i]) / 2) ,2));
	}
	
	distance = sqrt(distance);

    return distance;
}

//Following the documentation of Priority Queue, we have to create our own comparator for Points.
template <typename T>
class CPointDistanceComparator
{
	CPoint<T>* internalQueryPoint;
	CDistanceCalculator<T> myDistanceCalculator;
	vector<T> copyData;

public:
	CPointDistanceComparator(CPoint<T> pointReceived);
	bool operator() (CPoint<T> leftPoint, CPoint<T> rightPoint);
  	
};

template <typename T>
CPointDistanceComparator<T>::CPointDistanceComparator(CPoint<T> pointReceived)
{
	copyData.resize(pointReceived.getDimensionOfData());
	
	for (int i=0; i < pointReceived.getDimensionOfData(); i++)
		copyData[0] = pointReceived.data[i];

    internalQueryPoint = new CPoint<T>(copyData);
}

template <typename T>
bool CPointDistanceComparator<T>::operator() (CPoint<T> leftPoint, CPoint<T> rightPoint)
{
	CPoint<T> newQueryPoint(copyData);
	return (myDistanceCalculator.pointDistance(newQueryPoint, leftPoint) > myDistanceCalculator.pointDistance(newQueryPoint, rightPoint));
	//return (myDistanceCalculator.pointDistance(internalQueryPoint, leftPoint) > myDistanceCalculator.pointDistance(internalQueryPoint, rightPoint));
}

//Following the documentation of Priority Queue, we have to create our own comparator for Points.
template <typename T>
class CDataRegionDistanceComparator
{
private:
	CPoint<T>* internalQueryPoint;
	CDistanceCalculator<T> myDistanceCalculator;
	vector<T> copyData;

public:
	CDataRegionDistanceComparator(CPoint<T> pointReceived);
	bool operator() (CNode* leftNode, CNode* rightNode);
  	
};

template <typename T>
CDataRegionDistanceComparator<T>::CDataRegionDistanceComparator(CPoint<T> pointReceived)
{
    copyData.resize(pointReceived.getDimensionOfData());
	
	for (int i=0; i < pointReceived.getDimensionOfData(); i++)
		copyData[0] = pointReceived.data[i];

    internalQueryPoint = new CPoint<T>(copyData);
}

template <typename T>
bool CDataRegionDistanceComparator<T>::operator() (CNode* leftNode, CNode* rightNode)
{
	CPoint<T> newQueryPoint(copyData);
	return (myDistanceCalculator.dataRegionDistance(leftNode, newQueryPoint) > myDistanceCalculator.dataRegionDistance(rightNode, newQueryPoint));
	//return (myDistanceCalculator.dataRegionDistance(leftNode, internalQueryPoint) > myDistanceCalculator.dataRegionDistance(rightNode, internalQueryPoint));
}


template <typename T, int receiveDimensions>
class CLSDhTree
{
private:
	int maxPointsPerBucket;		//Maximum number of points per bucket.
	int dimensionOfData;		//Dimension of the data received.		
	CNode *head;				//Head node.

public:
	CLSDhTree(int numberOfPointsForBuckets);								//Constructor.
	void split(CNode **p, int oldD);										//Split node function.
	bool search(CPoint<T> pointToSearch, CNode ** &p, int &d);				//Search a point in the structure.
	bool insert(CPoint<T> pointToInsert);									//Insert a point in the structure.
	vector<vector<T> > k_NearestNeighbors(int k, CPoint<T> queryPoint);		//K-Nearest Neighbors Query.
};

template <typename T, int receiveDimensions>
CLSDhTree<T, receiveDimensions>::CLSDhTree(int numberOfPointsForBuckets)
{
	maxPointsPerBucket = numberOfPointsForBuckets;
	dimensionOfData = receiveDimensions;
	head = new CBucket<T, receiveDimensions, 2> (maxPointsPerBucket, dimensionOfData, 0);

	CBucket<T, receiveDimensions, 2>* buck = (CBucket<T, receiveDimensions, 2>*)head;

	T extremePoints[dimensionOfData][2];

	for(int i=0; i<dimensionOfData; i++)
    {
        extremePoints[i][0] = -100;
        extremePoints[i][1] = 100;
    }

    buck->updateLimits(extremePoints);

    //for (int i=0; i<(buck->extremePointsForDimensions.size()); i++)
    //{
    //	cout<<(buck->extremePointsForDimensions[i][0])<<"; "<<(buck->extremePointsForDimensions[i][1])<<endl;
    //}
}

template <typename T, int receiveDimensions>
void CLSDhTree<T, receiveDimensions>::split(CNode **p, int oldD)
{
	CBucket<T, receiveDimensions, 2> **bucketNodeP = (CBucket<T, receiveDimensions, 2>**)p;

	int i = 1;

	while ( ((*bucketNodeP)->checkFeatureValues( ((oldD + i) % dimensionOfData) )) != true )
	{	
		i+=1;
	}

	T positionForNewSplit = (*bucketNodeP)->getPositionOfSplit( ((oldD + i) % dimensionOfData) );
	T dimensionForNewSplit = ((oldD + i) % dimensionOfData);

	//cout<<"here gg 2 "<<(*bucketNodeP)->extremePointsForDimensions[1][0]<<endl;

	//cout<<"posicion: "<<positionForNewSplit<<endl;
	//cout<<"dimension: "<<dimensionForNewSplit<<endl;

	//Note that, with the split we have to create a list of minimun and maximum points for every dimension.
	//But we are creating a split in a exact dimension, splitting the bucket on this dimension in two buckets (left and right)
	//This means the positionForNewSplit will be the maximum point for this dimension on the left bucket.
	//On the other hand, we have the right bucket, the minimum point for the dimension on this bucket will be the position that we found.

	T extremePointsForLeftBucket[dimensionOfData][2];
	T extremePointsForRightBucket[dimensionOfData][2];
	
	for (int b=0; b<dimensionOfData; b++)
	{
		//cout<<"here "<<(*bucketNodeP)->extremePointsForDimensions[b][0]<<endl;
		extremePointsForLeftBucket[b][0] = (*bucketNodeP)->extremePointsForDimensions[b][0];
		extremePointsForLeftBucket[b][1] = (*bucketNodeP)->extremePointsForDimensions[b][1];
		extremePointsForRightBucket[b][0] = (*bucketNodeP)->extremePointsForDimensions[b][0];	
		extremePointsForRightBucket[b][1] = (*bucketNodeP)->extremePointsForDimensions[b][1];
	}

	//Now we are setting the minimum and maximum point for the dimension

	extremePointsForLeftBucket[(int)dimensionForNewSplit][1] = positionForNewSplit; //Maximum point for left bucket
	extremePointsForLeftBucket[(int)dimensionForNewSplit][0] = positionForNewSplit; //Minimum point for right bucket 

	CBucket<T, receiveDimensions, 2> *newBuckets[2]; //Creating two new buckets, 0 is for left and 1 is for right.

	newBuckets[0] = new CBucket<T, receiveDimensions, 2>(maxPointsPerBucket, dimensionOfData, dimensionForNewSplit);
	newBuckets[0]->updateLimits(extremePointsForLeftBucket);

	newBuckets[1] = new CBucket<T, receiveDimensions, 2>(maxPointsPerBucket, dimensionOfData, dimensionForNewSplit);
	newBuckets[1]->updateLimits(extremePointsForRightBucket);

	for (int i = 0; i < ((*bucketNodeP)->dataPoints.size()); i++)
	{
		newBuckets[ !(newBuckets[0]->checkLimits( (*bucketNodeP)->dataPoints[i] )) ]->insertPoint( (*bucketNodeP)->dataPoints[i] );
	}

	(*p) = new CDirectoryNode<T>( dimensionForNewSplit , positionForNewSplit );
	CDirectoryNode<T> **directoryNodeP = (CDirectoryNode<T>**)p;

	(*directoryNodeP)->sons[0] = newBuckets[0];
	(*directoryNodeP)->sons[1] = newBuckets[1];
}

template <typename T, int receiveDimensions>
bool CLSDhTree<T, receiveDimensions>::search(CPoint<T> pointToSearch, CNode ** &p, int &d)
{
	p = &head;

	CDirectoryNode<T> **directoryNodeP;

	while ((*p)->type)
	{
		directoryNodeP = (CDirectoryNode<T>**)p;
		d = (*directoryNodeP)->dimensionOfDirectoryNode;

		p = &((*directoryNodeP)->sons[!(pointToSearch.data[d] < (*directoryNodeP)->positionOfSplit)]);
	}	

	CBucket<T, receiveDimensions, 2> **bucketNodeP = (CBucket<T, receiveDimensions, 2>**)p;

	if ((*bucketNodeP)->searchPoint(pointToSearch))
	{
		return true;
	}

	return false;

}

template <typename T, int receiveDimensions>
bool CLSDhTree<T, receiveDimensions>::insert(CPoint<T> pointToInsert)
{
	CNode **p;

	int oldDimension = 0;

	if (search(pointToInsert, p, oldDimension))
	{
		cout<<"Punto encontrado, no insertado."<<endl;
		return false;

	}	

	CBucket<T, receiveDimensions, 2> **bucketNodeP = (CBucket<T, receiveDimensions, 2>**)p;

	//cout<<"tam actual bucket: "<<(*bucketNodeP)->dataPoints.size()<<endl;

	if (!( (*bucketNodeP)->insertPoint(pointToInsert) ))
		split(p, oldDimension);
	

	//cout<<"Insercion: "<<(*bucketNodeP)->insertPoint(pointToInsert)<<endl;
		

	return true;
}

template <typename T, int receiveDimensions>
vector<vector<T> > CLSDhTree<T, receiveDimensions>::k_NearestNeighbors(int k, CPoint<T> queryPoint)
{
	//The author of the structure put in the paper an algorithm to find the k-Nearest Neighbors of a point.
	//The algorithm needs two substructures, a NPQ and OPQ (node priority queue and object priority queue).
	//These priority queues are going to store nodes and points near to the query point based in distance.
	//In this case we are going to use Euclidean Distance.

	//According to the documentation of C++, we can use Priority Queues, they are part of the STL.
	//The paramethers of construction follow this order:
	//First the type of objects for the Priority Queue.
	//Second the container for the objects.
	//Third the comparator, in this case, Euclidean distance.
	

}



#endif
