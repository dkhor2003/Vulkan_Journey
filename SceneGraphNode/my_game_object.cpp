#include "my_game_object.h"
#include <iostream>

void MySceneGraphNode::addChild(std::shared_ptr<MySceneGraphNode>& obj)
{
	m_vMyChildren.push_back(obj); 
	obj->m_iLevel = m_iLevel + 1; 
	obj->m_pMyParent = shared_from_this().get(); 
}

void MySceneGraphNode::printSceneGraph()
{
	std::string current; 
	std::string offset; 

	for (int i = 0; i < m_iLevel; i++)
	{
		offset += "\t";
	}

	if (m_bIsCurrent)
	{
		current = "*";
	}
	std::cout << offset + current + getName() << std::endl;

	for (int i = 0; i < m_vMyChildren.size(); i++)
	{
		m_vMyChildren[i]->printSceneGraph(); 
	}
}

MySceneGraphNode * MySceneGraphNode::traverseNext(MySceneGraphNode* pCurrentNode)
{
	if (!pCurrentNode->m_bVisited)
	{
		pCurrentNode->m_bVisited = true; 
		pCurrentNode->m_bIsCurrent = true; 
		return pCurrentNode; 
	}
	else if (pCurrentNode->m_vMyChildren.size() == 0)
	{
		pCurrentNode->m_bIsCurrent = false;
		return traverseNext(pCurrentNode->m_pMyParent); 
	}
	else
	{
		for (int i = 0; i < pCurrentNode->m_vMyChildren.size(); i++)
		{
			if (!pCurrentNode->m_vMyChildren[i]->m_bVisited)
			{
				pCurrentNode->m_bIsCurrent = false; 
				return traverseNext(pCurrentNode->m_vMyChildren[i].get()); 
			}
		}
		if (pCurrentNode->m_pMyParent == nullptr)
		{
			unvisitAll(pCurrentNode); 
			pCurrentNode->m_bVisited = true; 
			pCurrentNode->m_bIsCurrent = true; 
			return pCurrentNode;
		}
		pCurrentNode->m_bIsCurrent = false;
		return traverseNext(pCurrentNode->m_pMyParent); 
	}

	return nullptr;
}

void MySceneGraphNode::unvisitAll(MySceneGraphNode* pCurrentNode)
{
	pCurrentNode->m_bVisited = false; 

	for (int i = 0; i < pCurrentNode->m_vMyChildren.size(); i++)
	{
		unvisitAll(pCurrentNode->m_vMyChildren[i].get()); 
	}
}

