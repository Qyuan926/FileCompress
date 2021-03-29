#pragma once
#include<vector>
#include<queue>



template <class T>
class huffmanNode{
public:
	huffmanNode(T weight)
		:_left(nullptr)
		, _right(nullptr)
		, _weight(weight)
	{}
	

	huffmanNode<T>* _left;
	huffmanNode<T>* _right;
	T              _weight;

};


template <class T>
class huffmanTree{
public:
	typedef huffmanNode<T>  Node;
	huffmanTree()
		:_root(nullptr)
	{}
	~huffmanTree()
	{
		Destory(_root);
	}
	
	struct Compare{
		bool operator()(const Node* node1,const Node* node2)
		{
			return node1->_weight > node2->_weight;
		}
	};

	Node* CreateTree(const std::vector<T>& weight,const T invalid){
		//根据权值创建一个小堆
		std::priority_queue<Node*, std::vector<Node*>, Compare> que;
		for (auto e : weight)
		{
			if (e != invalid)
			que.push(new Node(e));
		}
		//从小堆里取值，构成哈夫曼树
		while (que.size() > 1)
		{
			Node* node1 = que.top();
			que.pop();
			Node* node2 = que.top();
			que.pop();

			T newWeight = node1->_weight + node2->_weight;
			Node* newNode = new Node(newWeight);
			newNode->_left = node1;
			newNode->_right = node2;
			que.push(newNode);
		}
		this->_root = que.top();
		return _root;
	}
private:
	void Destory(Node* root)
	{
		if (root != nullptr)
		{
			Destory(root->_left);

			Destory(root->_right);
			if (root->_left == nullptr&&root->_right == nullptr)
			{
				delete root;
				root = nullptr;
				return;
			}
		}
	}
private:
	Node* _root;
};


