#ifndef CPOINT_H
#define CPOINT_H

#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

class CNode
{
public:
	bool type;	//Determine if is a Directory Node (True) or a Bucket (False).
	virtual int getDimensionOfData() = 0;	
	virtual vector<double> getMiddlePoint() = 0;
	virtual vector<double> getSizeOfDimensions() = 0;
};

template <typename T>
class CPoint
{
private:
	int dimensionOfData;
	
public:
	T* data;
	CPoint(vector<T> dataRead);
	int getDimensionOfData();
	bool operator==(CPoint<T> pointToCompare);
	void clearPoint();
	
};

template <typename T>
CPoint<T>::CPoint(vector<T> dataRead)
{
	dimensionOfData = dataRead.size();
	data = new T[dimensionOfData];

	for (int i=0; i<dimensionOfData; i++)
	{
		data[i] = dataRead[i];
	}
}

template <typename T>
int CPoint<T>::getDimensionOfData() { return dimensionOfData; }

template <typename T>
bool CPoint<T>::operator==(CPoint<T> pointToCompare)
{
	bool result = true;

	for (int i = 0; i < dimensionOfData; i++)
	{
		if (data[i] != pointToCompare.data[i])
			result = false;		
	}

	return result;
}

template <typename T>
void CPoint<T>::clearPoint()
{
	delete[] data;
}

#endif
