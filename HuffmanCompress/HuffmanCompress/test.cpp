#define _CRT_SECURE_NO_WARNINGS 1


#include"huffCompress.h"
void TesthuffmanCompress()
{
	huffCompress  hc0;
	hc0.Compress("0.txt");
	hc0.UnCompress("Compress0.huffzip");
	huffCompress  hc1;
	hc1.Compress("1.txt");
	hc1.UnCompress("Compress1.huffzip");
	huffCompress  hc2;
	hc2.Compress("2.jpg");
	hc2.UnCompress("Compress2.huffzip");
	huffCompress  hc3;
	hc3.Compress("3.png");
	hc3.UnCompress("Compress3.huffzip");
	huffCompress  hc4;
	hc4.Compress("4.txt");
	hc4.UnCompress("Compress4.huffzip");
}
int main()
{
	TesthuffmanCompress();
	//TestPhuffmanTree();
	//TestLZ();
	//PTesthuffmanCompress();
	return 0;
}