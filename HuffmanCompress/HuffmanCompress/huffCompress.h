#pragma once
#include "huffmanTree.h"
#include<string>
#include<iostream>
#include<vector>


class charInfo{
public:
	

	charInfo(const unsigned long long appearnum=0)
		:_appearNum(appearnum)
	{}


	unsigned char        _ch;         //字符
	unsigned long long   _appearNum;   //该字符的出现次数
    std::string          _strCode;    //该字符对应的哈夫曼编码

	charInfo operator+(const charInfo ch)
	{
		return charInfo(this->_appearNum + ch._appearNum);
	}
	bool operator>(const charInfo ch)  const
	{
		return this->_appearNum > ch._appearNum;
	}
	bool operator!=(const charInfo ch)
	{
		return this->_appearNum != ch._appearNum;
	}
};



class huffCompress{

public:
	huffCompress()
	{
		_vec.resize(256);
		for (int i = 0; i < 256; ++i)
		{

			_vec[i]._ch = i;
			_vec[i]._appearNum = 0;
			_vec[i]._strCode = "";
		}
	}
	void Compress(const std::string& filePath);
	void UnCompress(const std::string& filePath);
	void huffCode(huffmanNode<charInfo>* root, std::string& str);
	void CompressHead(const std::string& filePath, const std::string& fileout, const charInfo& invalid);
	void GetlineOfFile(FILE* file, std::string& str);
private:
	std::vector<charInfo>  _vec;
};


