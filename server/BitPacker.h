#ifndef BITPACKER_H
#define BITPACKER_H

#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <bitset>

#define BLOCK 32

using namespace std;

class BitPacker
{
public:
  BitPacker(const int &width, const int &size): mWidth(width), mSize(size) {
    if(width > 32) {
      throw invalid_argument("width should be less than or equal to 32");
    }
    mLength = (int)ceil((mWidth * mSize) / BLOCK);

    mBlocks = new uint[mLength + 1];
    memset(mBlocks, 0, mLength);
  };

  ~BitPacker() {
    delete mBlocks;
  };

  void store(uint index, uint value) {
    int startBit = index * mWidth;
    int endBit = (index + 1) * mWidth - 1;
    int startBlock = startBit / BLOCK;
    int endBlock = endBit / BLOCK;

    // get [index, ... , index + width - 1]
    
    if(startBlock == endBlock) { // getting one block is enough
      uint block = mBlocks[startBlock];
      
      int l = startBit - startBlock * BLOCK;
      int r = BLOCK - (endBit - startBlock * BLOCK);

      uint mask = 0xffffffff;

      mask >>= l + r - 1;
      mask <<= r - 1;
//      cout << "Mask0: " << bitset<32>(mask) << endl;
      
      mBlocks[startBlock] = (block & ~mask) | (value << (r - 1));
//      cout << bitset<32>(mBlocks[startBlock]) << endl;
//      cout << mBlocks[startBlock] << endl;
    }
    else {
      uint block1 = mBlocks[startBlock];
      uint block2 = mBlocks[endBlock];
      
      int l = startBit - startBlock * BLOCK;
      int r = BLOCK - (endBit - endBlock * BLOCK);

      uint mask = 0xffffffff;

//      cout << l << endl;
      mask >>= l;
      mBlocks[startBlock] = (block1 & ~mask) | (value >> (mWidth - (BLOCK - l)));
//      cout << "Mask1: " << bitset<32>(mask) << endl;
//      cout << bitset<32>(mBlocks[startBlock]) << endl;

      mask = 0xffffffff;
      
      mask <<= r - 1;
      
      mBlocks[endBlock] = (block2 & ~mask) | (value << (r - 1));
//      cout << "Mask2: " << bitset<32>(mask) << endl;
//      cout << bitset<32>(mBlocks[endBlock]) << endl;
    }
  }

  uint load(uint index) {
    int startBit = index * mWidth;
    int endBit = (index + 1) * mWidth - 1;
    int startBlock = startBit / BLOCK;
    int endBlock = endBit / BLOCK;
    uint value;

    // get [index, ... , index + width - 1]
    
    if(startBlock == endBlock) { // getting one block is enough
      uint block = mBlocks[startBlock];
      
      int l = startBit - startBlock * BLOCK;
      int r = BLOCK - (endBit - startBlock * BLOCK);

      uint mask = 0xffffffff;

      mask >>= l + r - 1;
      mask <<= r - 1;
      
      //cout << "Mask0: " << bitset<32>(mask) << endl;
      value = (block & mask) >> (r - 1);
      //cout << "value" << ' ' << value << endl;
      return value;
    }
    else {
      uint block1 = mBlocks[startBlock];
      uint block2 = mBlocks[endBlock];
      
      int l = startBit - startBlock * BLOCK;
      int r = BLOCK - (endBit - endBlock * BLOCK);

      uint mask = 0xffffffff;

      mask >>= l;
//      cout << l << ' ' << r << endl;
      value = (block1 & mask) << (mWidth - (BLOCK - l));
//      cout << "v" << ' ' << bitset<32>(value) << endl;

      //cout << "Mask1: " << bitset<32>(mask) << endl;
      //cout << "Value1: " << bitset<32>(value) << endl;
      mask = 0xffffffff;
      
      mask <<= r - 1;
     
      value |= (block2 & mask) >> (r - 1);
      //cout << "Mask2: " << bitset<32>(mask) << endl;
      //cout << "Value2: " << bitset<32>(value) << endl;
      //cout << "value" << ' ' << value << endl;
      return value;
    }
    return 0;
  }

  uint getMemorySize() {
    return sizeof(uint) * mLength;
  }

private:
  int mWidth;
  int mSize; 
  int mLength;
  uint *mBlocks;
};

#endif
