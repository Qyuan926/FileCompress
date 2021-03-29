#define _CRT_SECURE_NO_WARNINGS 1
#include"huffCompress.h"



void huffCompress::Compress(const std::string& filePath){
	//���ļ���ͳ���ļ���ÿ���ֽڳ��ֵĴ���
	FILE*  file = fopen(filePath.c_str(), "rb");
	if (file == nullptr)
	{
		std::cout << "Ҫѹ�����ļ�������" << std::endl;
		return;
	}
	unsigned char readBuff[1024] = { 0 };
	while (1)
	{
		size_t readSize = fread((void*)readBuff, 1, 1024, file);
		if (readSize == 0)
		{
			break;
		}
		for (size_t i = 0; i < readSize; ++i)
		{
			_vec[readBuff[i]]._appearNum++;
		}
	}
	//����Ȩֵ��������������
	huffmanTree<charInfo>  Tree;
	huffmanNode<charInfo>* root = Tree.CreateTree(_vec, charInfo(0));
	//���ݹ�����������ȡÿ���ַ��Ĺ���������
	std::string str1;
	huffCode(root, str1);
	//��ȡԴ�ļ����ù����������滻Դ�ļ��ַ�������ѹ����
	//ѹ���ļ�������:Դ�ļ�������
	//                �����������ַ� �����ִ���
	//                ������������ɵ�Դ�ļ�

	fseek(file, 0, SEEK_SET);
	std::string filename = "Compress";
	filename += filePath.substr(0,filePath.rfind("."));
	filename += ".huffzip";
	//CompressHead(filePath, filename, charInfo(0));
	FILE* fileout = fopen(filename.c_str(), "wb");

	charInfo invalid;
	std::string fileExtension = filePath.substr(filePath.find_last_of('.'));
	fileExtension += '\n';
	size_t Count = 0;
	std::string str;
	for (int i = 0; i < 256; ++i)
	{
		if (_vec[i] != invalid)
		{
			++Count;
			//str += std::to_string(_vec[i]._ch);
			str += _vec[i]._ch;
			str += ':';
			str += std::to_string(_vec[i]._appearNum);
			str += "\n";
		}
	}
	
	std::string Counts = std::to_string(Count);
	Counts += "\n";
	fwrite(fileExtension.c_str(), 1, fileExtension.size(), fileout);
	fwrite(Counts.c_str(), 1, Counts.size(), fileout);
	fwrite(str.c_str(), 1, str.size(), fileout);
	//���ֽڵķ�ʽд���ļ�����
	unsigned char  data = 0;
	char bitCount = 0;
	while (1)
	{
		size_t readSize = fread((void*)readBuff, 1, 1024, file);
		if (readSize == 0)
		{
			break;
		}
		for (size_t i = 0; i < readSize; ++i)
		{

			std::string& str = _vec[readBuff[i]]._strCode;
			for (size_t j = 0; j < str.size(); ++j)
			{
				data <<= 1;
				if (str[j] == '1')
					data |= 1;

				bitCount++;
				if (bitCount == 8)
				{
					//������д��ѹ���ļ�
					fputc(data, fileout);
					bitCount = 0;
					data = 0;
				}
			}
		}
	}
	if (bitCount > 0 && bitCount < 8)
	{
		data <<= (8 - bitCount);
		fputc(data, fileout);
		bitCount = 0;
		data = 0;
	}
	fclose(file);
	fclose(fileout);
}

void huffCompress::CompressHead(const std::string& filePath, const std::string& fileout, const charInfo& invalid)
{
	std::string fileExtension = filePath.substr(filePath.find_last_of('.'));
	fileExtension += '\n';
	size_t Count = 0;
	std::string str;
	for (int i = 0; i < 256; ++i)
	{
		if (_vec[i] != invalid)
		{
			++Count;
			//str += std::to_string(_vec[i]._ch);
			str += _vec[i]._ch;
			str += ':';
			str += std::to_string(_vec[i]._appearNum);
			str += "\n";
		}
	}
	FILE* file = fopen(fileout.c_str(), "wb");
	if (file == nullptr)
	{
		std::cout << "ѹ���ļ���ʧ��" << std::endl;
		return;
	}
	std::string Counts = std::to_string(Count);
	Counts += "\n";
	fwrite(fileExtension.c_str(), 1, fileExtension.size(), file);
	fwrite(Counts.c_str(), 1, Counts.size(), file);
	fwrite(str.c_str(), 1, str.size(), file);
	fclose(file);
}



void huffCompress::huffCode(huffmanNode<charInfo>* root, std::string& str)
{
	huffmanNode<charInfo>* cur = root;
	if (cur->_left == nullptr&&cur->_right == nullptr)
	{
		_vec[cur->_weight._ch]._strCode = str;
		//printf("%d  : %s\n", cur->_weight._ch, str.c_str());
		//std::cout << str << std::endl;
		//std::cout << cur->_weight._ch << " :  " << str << std::endl;
		//fflush(stdout);
		return;
	}
	if (cur->_left != nullptr)
	{
		str += '0';
		huffCode(root->_left, str);
		str.pop_back();
	}
	if (cur->_right != nullptr)
	{
		str += '1';
		huffCode(root->_right, str);
		str.pop_back();
	}
}

void huffCompress::UnCompress(const std::string& filePath)
{
	std::string filename = filePath.substr(0, filePath.find_last_of('.'));
	//filename = filename.substr(8);
	filename = filename.substr(6);
	//std::cout << str << std::endl;
	FILE* file = fopen(filePath.c_str(), "rb");
	if (file == nullptr)
	{
		std::cout << "����ѹ�����ļ�������" << std::endl;
		return;
	}
	std::string str;
	GetlineOfFile(file, str);
	filename += str;
	//��ȡ������������Ϣ
	//����������Ҷ�ӽڵ����
	str.clear();
	GetlineOfFile(file, str);
	size_t Count = std::atoi(str.c_str());
	for (size_t i = 0; i < Count; ++i)
	{
		str.clear();
		GetlineOfFile(file, str);
		if (str.empty())
		{
			str += '\n';
			GetlineOfFile(file, str);
		}
		int num = std::atoi(str.c_str() + 2);
		unsigned char index = str[0];
		_vec[index]._appearNum = std::atoi(str.c_str() + 2);
	}
	//�����������
	huffmanTree<charInfo>  Tree;
	huffmanNode<charInfo>* root = Tree.CreateTree(_vec, charInfo(0));

	//std::cout << "����" << std::endl;
	//std::string str11;
	//huffCode(root, str11);
	//�򿪽�ѹ�ļ�
	FILE* fileout = fopen(filename.c_str(), "wb");
	if (fileout == nullptr)
	{
		std::cout << "������ѹ�ļ�ʧ��" << std::endl;
		return;
	}
	unsigned long long charNum = root->_weight._appearNum;
	unsigned char readBuff[1024] = { 0 };
	huffmanNode<charInfo>* cur = root;
	while (1)
	{
		size_t readSize = fread(readBuff, 1, 1024, file);
		if (readSize == 0)
		{
			break;
		}
		for (size_t i = 0; i < readSize; ++i)
		{
			unsigned char ch = readBuff[i];
			for (int j = 0; j < 8; ++j)
			{
				if (ch & 0x80)
				{
					cur = cur->_right;
				}
				else
				{
					cur = cur->_left;
				}
				//Ҷ�ӽڵ㣬��Ҷ�ӽڵ���ַ�д���ѹ�ļ�
				if (cur->_left == nullptr&&cur->_right == nullptr)
				{
					fputc(cur->_weight._ch, fileout);
					cur = root;
					--charNum;
					if (charNum == 0)
					{
						break;
					}
				}
				ch <<= 1;
			}

			ch = 0;

		}

	}
	fclose(fileout);
	fclose(file);

}


void huffCompress::GetlineOfFile(FILE* file, std::string& str)
{
	unsigned char ch = 0;

	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch == '\n')
		{
			break;
		}
		str += ch;
	}
}
