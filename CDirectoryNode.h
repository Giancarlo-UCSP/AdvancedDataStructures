#ifndef CDIRECTORYNODE_H
#define CDIRECTORYNODE_H

#include "CBucket.h"
#include <iostream>
#include <vector>

template <typename T>
class CDirectoryNode : public CNode
{
public:
	int dimensionOfDirectoryNode, positionOfSplit, numberOfSons;
	int usedBuckets, usedDirectories, usedSons;
	CNode *sons[2];

	CDirectoryNode(int dimensionToStore, int positionToStore);
	int getUsedSons();
	int getUsedBuckets();
	int getUsedDirectories();
	vector<T> getMiddlePoint();	
	vector<T> getSizeOfDimensions();
	int getDimensionOfData();
};	

template <typename T>
CDirectoryNode<T>::CDirectoryNode(int dimensionForNode, int positionForNode)
{
	type = true;

	dimensionOfDirectoryNode = dimensionForNode;
	positionOfSplit = positionForNode;
	usedSons = 0;

	sons[0] = nullptr;
	sons[1] = nullptr;
}

template <typename T>
vector<T> CDirectoryNode<T>::getMiddlePoint()
{
	vector<T> middlePoint;

	/*T extremePoints[sons[0]->getDimensionOfData()][2];

	for (int i=0; i < sons[0]->getDimensionOfData(); i++)
	{
		if (sons[0]->extremePointsForDimensions[i][0] < sons[1]->extremePointsForDimensions[i][0])
			extremePoints[i][0] = sons[0]->extremePointsForDimensions[i][0];
		else
			extremePoints[i][0] = sons[1]->extremePointsForDimensions[i][0];

		if (sons[0]->extremePointsForDimensions[i][1] > sons[1]->extremePointsForDimensions[i][1])
			extremePoints[i][1] = sons[0]->extremePointsForDimensions[i][1];
		else
			extremePoints[i][1] = sons[1]->extremePointsForDimensions[i][1];

	}

	for (int i=0; i < sons[0]->getDimensionOfData(); i++)
	{
		middlePoint.push_back( (extremePoints[i][0] + extremePoints[i][1]) / 2 );
	}*/

	middlePoint = sons[0]->getMiddlePoint();
	middlePoint[dimensionOfDirectoryNode] = positionOfSplit;
		
	return middlePoint;
}

template <typename T>
vector<T> CDirectoryNode<T>::getSizeOfDimensions()
{
	vector<T> sizeOfDimensions;

	sizeOfDimensions = sons[0]->getMiddlePoint();
	sizeOfDimensions[dimensionOfDirectoryNode] += sons[1]->getMiddlePoint()[dimensionOfDirectoryNode];

	return sizeOfDimensions;
}

template <typename T>
int CDirectoryNode<T>::getDimensionOfData()
{
	CNode* head = sons[0];

	while ( (head) && (head->type) )
	{
		CDirectoryNode<T> *directory = (CDirectoryNode<T>*) head;
		head = directory->sons[0];
	}

	if ((head) && ((head->type)==false))
		return head->getDimensionOfData();
	else
		return -1;
}

#endif
