#ifndef HFILE_TREE
#define HFILE_TREE



/*
* Class Node
*/
template <typename T>
class Node
{
public:
	T *mData;

	Node *mPrev;
	Node *mLeft;
	Node *mRight;
};

/*
* Class BinaryTree
*/
template <typename T>
class BinaryTree
{
public:
	/*Constructor(s)*/
	BinaryTree();

	/*The "Big 3"*/
	BinaryTree(const BinaryTree &copy);
	~BinaryTree();
	void operator=(const BinaryTree &right);

	/*Accessors*/
	Node<T> *GetRoot() const;
	Node<T> *Search(T *searchData, Node<T> *curNodePtr) const;

	/*Mutators*/
	void Copy(const BinaryTree &copy);
	void Delete(Node<T>** deleteNode);
	void InsertNode(T *insertData, Node<T> *parentNode);
	void RemoveNode(Node<T> *removeNode);

private:
	Node<T> *mRoot;
};



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class SliderHandler
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
template <typename T>
BinaryTree<T>::BinaryTree()
{
	//mRoot = NULL;
	mRoot = new Node<T>();
	mRoot->mData = NULL;
	mRoot->mPrev = NULL;
	mRoot->mLeft = NULL;
	mRoot->mRight = NULL;
}

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree<T> &copy)
{
	Copy(copy);
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
	Node<T> **rootPtr = &mRoot;
	Delete(rootPtr);
}

template <typename T>
Node<T> *BinaryTree<T>::GetRoot() const
{
	return mRoot;
}

template <typename T>
void BinaryTree<T>::Copy(const BinaryTree<T> &copy)
{
	//if( mRoot != NULL )
	//	Delete();

	//Node *movePtr = copy.mRoot;
	//if( movePtr == NULL )
	//	return;

	//mRoot = new Node;
	//mRoot->mPrev = NULL;
	//mRoot->mLeft = NULL;
	//mRoot->mRight = NULL;

	//while( movePtr->mLeft != NULL )
	//{
	//	//while( movePtr->mRight != NULL )

	//	mRoot->mLeft = new Node;
	//	mRoot->mLeft->mStr = movePtr->mStr;

	//	//USE RECURSION
	//	//movePtr = movePtr->m
	//}
}

template <typename T>
void BinaryTree<T>::Delete(Node<T>** deleteNode)
{
	if (*deleteNode == NULL)
		return;

	Delete(&((*deleteNode)->mLeft));
	Delete(&((*deleteNode)->mRight));

	delete (*deleteNode)->mData;
	delete *deleteNode;
	*deleteNode = NULL;
}

template <typename T>
void BinaryTree<T>::InsertNode(T *insertData, Node<T> *parentNode)
{
	Node *newNode = new Node<T>;
	newNode->mData = insertData;

	/*If parentNode is NULL, then newNode becomes the root.*/
	if (parentNode == NULL)
	{
		mRoot = newNode;
	}

	/*If inserting at the root, then insert newNode at either the root's left or right child.*/
	else if (parentNode->mPrev == NULL)
	{
		/*If both of the root's child nodes have data, or if left child is empty,
		  insert new node into the root's left child.*/
		if ((parentNode->mLeft != NULL && parentNode->mRight != NULL) ||
			parentNode->mLeft == NULL)
		{
			newNode->mPrev = parentNode;
			newNode->mLeft = parentNode->mLeft;
			newNode->mRight = NULL;
			parentNode->mLeft = newNode;
		}

		/*Otherwise, insert newNode into the root's right child.*/
		else
		{
			newNode->mPrev = parentNode;
			newNode->mLeft = parentNode->mRight;
			newNode->mRight = NULL;
			parentNode->mRight = newNode;
		}
	}

	/*Insert somewhere below the root node.*/
	else
	{
		/*By default, insert newNode into the left child of parentNode. The left child
		  of newNode will contain the old left child of parentNode. By default, the right
		  child of newNode will be NULL.*/
		newNode->mPrev = parentNode;
		newNode->mLeft = parentNode->mLeft;
		newNode->mRight = NULL;
		parentNode->mLeft = newNode;
	}
}

template <typename T>
void BinaryTree<T>::RemoveNode(Node<T> *removeNode)
{
}

template <typename T>
Node<T>* BinaryTree<T>::Search(T *searchData, Node<T> *curNodePtr) const
{
	if (curNodePtr == NULL)
		return NULL;

	if (curNodePtr->mData == *searchData)
		return curNodePtr;

	Node<T> *retNode = NULL;
	retNode = Search(searchData, curNodePtr->mLeft);
	if (retNode->mData == searchData)
		return retNode;

	retNode = Search(searchData, curNodePtr->mRight);
	if (retNode->mData == searchData)
		return retNode;

	return NULL;
}

template <typename T>
void BinaryTree<T>::operator=(const BinaryTree<T> &right)
{
}



#endif /*HFILE_TREE*/