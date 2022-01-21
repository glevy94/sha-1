 /**         
Final.                                                                  
Grayson Levy                                    
05/04/2021                                 
*/

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <iomanip>
#define MAX_SIZE 1048576
#define MAX 80

const bool errorCheck = true; //turn true for debugging/ abcde hex values
using namespace std;

//function prtotypes
unsigned int readFile(unsigned char []);
unsigned int calculateBlocks(unsigned int);
void convertCharArrayToIntArray(unsigned char [], unsigned int [], unsigned int);
void addBitCountToLastBlock(unsigned int [], unsigned int, unsigned int);
unsigned int f(unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int K(unsigned int);
void computeMessageDigest(unsigned int [], unsigned int);
unsigned int S(unsigned int, unsigned int);


int main(int argc, char *argv[]){

  unsigned char buffer[MAX_SIZE] = {0};
  unsigned int size = 0, blockCount = 0;
  unsigned int message[MAX_SIZE] = {0};


  size = readFile(buffer);
  if (errorCheck) cout<<"\nchars in buffer:"<<size<<endl; //prints size of buffer
  blockCount = calculateBlocks(size); //calculates block count
  if (errorCheck) cout<<"\nblocks for this message:"<<blockCount<<endl; //prints number of blocks
  convertCharArrayToIntArray(buffer, message, size);//converts array to ints
  addBitCountToLastBlock(message, size, blockCount); //add # of bits to last array position
  computeMessageDigest(message, blockCount); //prints message digest
}

unsigned int readFile(unsigned char buffer[]){
  
  unsigned int i = 1;
  int c = 0;

  //gets first char in file
  c  = getchar();
  if (c == EOF) {
    cout<<"ERROR:The File is empty\n"; 
    exit(1);
  }
  else {
    buffer[0] = c; //if file is not empty assigns char to buffer
    while((c = getchar()) != EOF) { //gets next char 
      buffer[i] = c; //assigns to buffer
      i++; //keeps track of array position and amount of chars in array
      if (i >= (MAX_SIZE-10)) { //limits ffile size 
	cout<<"ERROR:FILE IS TO BIG\n";
	exit(1);
      }
    }
  }

  buffer[i] = 0x80; //adds one bit to array at last index after input
  
  //checking last array position for one bit(int value of 128) 
  if(errorCheck) cout<<"\nvalue of last position of char array:"<<(int)buffer[i]<<endl; 
  
  //returns amount of chars 
  return i;

}

unsigned int calculateBlocks(unsigned int size){
  
  const int block = 512; //bit size of one block
  const int bits = 8; //bits in byte
 
  unsigned int blockCount = 0;

  blockCount = (((bits * size) + 1) / block) + 1;
  if((((bits * size) + 1)%block) > (block - 64)) {
    blockCount++;
  }

  return blockCount;

}

void convertCharArrayToIntArray(unsigned char buffer[], unsigned int message[], unsigned int size) {
  
  int j = 0;
  int shift = 8;
  int k = 0;
  int b = 31;
  
  for (unsigned int i = 1; i<=size+1; i++){     
    //cout<<j<<endl;
    message[j] |= buffer[k];
    //goes to next index after every 32 bits
    //also prevents last 8 bits from being shifted
    if ((i%4) == 0) j++;
    message[j] <<= shift;
    //    cout<<message[j]<<endl;
    k++; //increase index value of buffer
  }

  //apend 0's to unshifted array indexs
  if(size%4 == 0){
    message[j] <<= shift;
    message[j] <<= shift;
  }
  if(size%4 == 1){
    message[j] <<= shift;
  }
  


  //prints first 2 blocks in 32 bit segments for error checking
  if(errorCheck){ 
    cout<<"\nfirst two blocks before bit count is added:"<<endl;
    for (j = 0; j<= b; j++){ 
      cout<<message[j]<<endl;
    }
  }
}

void addBitCountToLastBlock(unsigned int message[], unsigned int size, unsigned int blockCount) {
  
  unsigned int length = size * 8;
  int index = (blockCount * 16) - 1;
  message[index] += length;

  //prints last  block in 32 bit segments for error checking 
  if(errorCheck){
    cout<<"\nlast block bit count added:"<<endl;
    for (int j = (index - 15); j <= index; j++){
      cout<<message[j]<<endl;
    }
  }
}

unsigned int f(unsigned int t, unsigned int B, unsigned int C, unsigned int D) {

  unsigned int f = 0;

  if(0 <= t && t <= 19){
    f = (B&C) | ((~B)&D);
  }
  if(20 <= t && t <= 39){
    f = (B^C^D);
  }
  if(40 <= t && t <= 59){
    f = (B&C) | (B&D) | (C&D);
  }
  if(60 <= t && t <= 79){
    f = (B^C^D);
  }
  
  return f;
}

unsigned int K(unsigned int t) {

  unsigned int K = 0;

  if(0 <= t && t <= 19) (K = 0x5A827999);
  if(20 <= t && t <= 39) (K = 0x6ED9EBA1);
  if(40 <= t && t <= 59) (K = 0x8F1BBCDC);
  if(60 <= t && t <= 79) (K = 0xCA62C1D6);
  return K;
}

//circle operation
unsigned int S(unsigned int num, unsigned int num1){
  
  unsigned int result = 0;
  
  result = (num1 << num) | (num1 >> (32-num));
  return result;
}

void computeMessageDigest(unsigned int message[], unsigned int blockCount) {

  unsigned int A, B, C, D, E;
  unsigned int H_0 = 0x67452301, H_1 = 0xEFCDAB89, H_2 = 0x98BADCFE, H_3 = 0x10325476, H_4 = 0xC3D2E1F0;
  unsigned int temp = 0;
  unsigned int w[MAX];
  int t = 0, i = 0, j = 0;
  unsigned int count = 0;

  while(count < blockCount) {
    
    //assigns values for every block
    A = H_0;
    B = H_1;
    C = H_2;
    D = H_3;
    E = H_4;

    for(i = 0; i < 16; i++) {
      w[i] = message[j];
      j++;
    }
    count++;
    
    if(errorCheck) cout<<"\nA,B,C,D,E hex values:\n"<<endl;

    for(t = 0; t <= 79; t++) {
      
      if(16 <= t && t <= 79) {
	w[t] = S(1, (w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16]));
	temp = (S(5,A) + f(t,B,C,D) + E + w[t] + K(t));
	E = D;
	D = C;
	C = S(30, B);
	B = A;
	A = temp;
      }
      if(0 <= t && t <= 15) {
	temp = (S(5,A) + f(t,B,C,D) + E + w[t] + K(t));
        E = D;
        D = C;
        C = S(30, B);
        B = A;
        A = temp;
      }

      if(errorCheck){
	cout<<dec<<t<<" = "<<hex<<uppercase<<" "<<A<<" "<<B<<" "<<C<<" "<<D<<" "<<E<<endl;
      }
    }

    H_0 = (H_0 + A);
    H_1 = (H_1 + B);
    H_2 = (H_2 + C);
    H_3 = (H_3 + D);
    H_4 = (H_4 + E);
  }

  cout<<"\n\nMessage Digest = "<<uppercase<<hex<<H_0<<" "<<H_1<<" "<<H_2<<" "<<H_3<<" "<<H_4<<endl;


}
