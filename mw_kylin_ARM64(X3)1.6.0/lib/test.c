#include "mwReader.h"
int main(){
	char list[64] = { 0 };
	mwDevGetUsbList(list);
	return 0;
}
