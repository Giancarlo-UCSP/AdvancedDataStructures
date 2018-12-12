#ifndef CBUCKET_H
#define CBUCKET_H

#include <iostream>
#include <vector>
#include <cmath>
#include "CPoint.h"
#include "CLSDhTree.h"

using namespace std;

template <typename T, int dimensionExternal, int counterReferenceValues>
class CBucket : public CNode
{
private:
	int maxNumberOfPoints; 							//Bucket size
	int dimensionOfData;   							//Dimension of the points 
	int dimensionOfBucket; 							//In which dimension is the bucket?
	
public:
	vector<CPoint<T> > dataPoints; 					//Set of points for this bucket	
	T extremePointsForDimensions[dimensionExternal][counterReferenceValues];	//Minimum and maximum points of this bucket

	CBucket(int maxPointsSize, int dimensionOfPoints, int dimensionForBucket);	
	void updateLimits(T newLimits[dimensionExternal][counterReferenceValues]);
	bool checkFeatureValues(int dimensionToCheck);
	T getPositionOfSplit(int dimensionToCheck);
	bool checkLimits(CPoint<T> pointToCheck);
	bool searchPoint(CPoint<T> pointToSearch);		
	bool insertPoint(CPoint<T> pointToInsert);
	void clearBucket();
	vector<T> getMiddlePoint();
	vector<T> getSizeOfDimensions();
	int getDimensionOfData();
};

template <typename T, int dimensionExternal, int counterReferenceValues>
CBucket<T, dimensionExternal, counterReferenceValues>::CBucket(int maxPointsSize, int dimensionOfPoints, int dimensionForBucket)
{
	type = false;
	maxNumberOfPoints = maxPointsSize;
	dimensionOfData = dimensionOfPoints;
	dimensionOfBucket = dimensionForBucket;
}

template <typename T, int dimensionExternal, int counterReferenceValues>
void CBucket<T, dimensionExternal, counterReferenceValues>::updateLimits(T newLimits[dimensionExternal][counterReferenceValues])
{
	for (int i = 0; i < dimensionOfData; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			extremePointsForDimensions[i][j] = newLimits[i][j];
		}
	}
}

template <typename T, int dimensionExternal, int counterReferenceValues>
bool CBucket<T, dimensionExternal, counterReferenceValues>::checkFeatureValues(int dimensionToCheck)
{
	T firstValue = dataPoints[0].data[dimensionToCheck];
	//cout<<"first value: "<<firstValue<<endl;
	bool result = false;

	//cout<<"dim to check "<<dimensionToCheck<<endl;
	//cout<<"size "<<dataPoints.size()<<endl;

	for (int i=0; i<dataPoints.size(); i++)
	{
		//cout<<"data "<<dataPoints[i].data[dimensionToCheck]<<endl;
		if (firstValue != dataPoints[i].data[dimensionToCheck])
		{
			//cout<<"first value if: "<<firstValue<<endl;
			return true;
		}
	}

	return false;
}

template <typename T, int dimensionExternal, int counterReferenceValues>
T CBucket<T, dimensionExternal, counterReferenceValues>::getPositionOfSplit(int dimensionToCheck)
{
	T savedSum = 0;

	for (int i=0; i<dataPoints.size(); i++)
	{
		savedSum += dataPoints[i].data[dimensionToCheck];
	}
		
	T prom = (savedSum / dataPoints.size()); 	
	return prom;
}

template <typename T, int dimensionExternal, int counterReferenceValues>
bool CBucket<T, dimensionExternal, counterReferenceValues>::checkLimits(CPoint<T> pointToCheck)
{
	for (int i = 0; i < dimensionOfData; i++)
	{
		if (pointToCheck.data[i]<extremePointsForDimensions[i][0] || pointToCheck.data[i] >= extremePointsForDimensions[i][1])
			return false;
			

	}

	return true;
}

template <typename T, int dimensionExternal, int counterReferenceValues>
bool CBucket<T, dimensionExternal, counterReferenceValues>::searchPoint(CPoint<T> pointToSearch)
{
	for (int i = 0; i < dataPoints.size(); i++)
	{
		if ((dataPoints[i]) == (pointToSearch))
			return true;
	}

	return false;
}

template <typename T, int dimensionExternal, int counterReferenceValues>
bool CBucket<T, dimensionExternal, counterReferenceValues>::insertPoint(CPoint<T> pointToInsert)
{
	if (dataPoints.size() + 1 <= maxNumberOfPoints)
	{
		dataPoints.push_back(pointToInsert);
		return true;
	}

	else 
		return false;
}

template <typename T, int dimensionExternal, int counterReferenceValues>
void CBucket<T, dimensionExternal, counterReferenceValues>::clearBucket()
{
	for (int i=0; i < dataPoints.size(); i++)
	{
		dataPoints[i].clearPoint();
	}
}

template <typename T, int dimensionExternal, int counterReferenceValues>
vector<T> CBucket<T, dimensionExternal, counterReferenceValues>::getMiddlePoint()
{
	//This function reads all the limits of this bucket, minimum and maximum, then add and divide by 2.
	//Does this operation for each dimension.

	vector<T> middlePoint;

	for (int i=0; i < dimensionOfData; i++)
	{
		middlePoint.push_back( (extremePointsForDimensions[i][0] + extremePointsForDimensions[i][1]) / 2 );
	}

	return middlePoint;

}

template <typename T, int dimensionExternal, int counterReferenceValues>
vector<T> CBucket<T, dimensionExternal, counterReferenceValues>::getSizeOfDimensions()
{
	vector<T> sizeOfDimensions;

	for (int i=0; i < dimensionOfData; i++)
	{
		sizeOfDimensions.push_back( abs(extremePointsForDimensions[i][1] - extremePointsForDimensions[i][0]) );
	}

	return sizeOfDimensions;
}


template <typename T, int dimensionExternal, int counterReferenceValues>
int CBucket<T, dimensionExternal, counterReferenceValues>::getDimensionOfData()
{
	return dimensionOfData;
}

#endif
