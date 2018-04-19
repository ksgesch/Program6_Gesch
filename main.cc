/* Kynsee Gesch, ksg160030@utdallas.edu, 3377.002*/



#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <locale>
#include <iomanip>
#include "cdk.h"
#include <sstream>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryFileHeader{
public:

  uint32_t magicNumber; //should be 0xFEEDFACE
  uint32_t versionNumber;
  uint64_t numRecords;
};

const int maxRecordStringLength=25;  
class BinaryFileRecord{
public:

  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};

template <class T>
string to_string(T t, ios_base & (*f)(ios_base&))
{
  ostringstream oss;
  oss << f << t;
  return oss.str();
}

int main()
{

  cout << setprecision(10);
  ifstream binFile;

  BinaryFileHeader *header= new BinaryFileHeader();
  header->magicNumber=0;
  header->versionNumber=0;
  header->numRecords=0;

  BinaryFileRecord *record= new BinaryFileRecord();
  record->strLength=0;

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

 

 
  const char 		*rowTitles[MATRIX_HEIGHT+1] = {"R0", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[MATRIX_WIDTH+1] = {"C0", "a", "b", "c"};
  int		boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH };
  int		boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */

  stringstream ss;
  string temp;

  //set the first row to the header of the binary file
  binFile.open("cs3377.bin", ios::binary | ios::in);
  if(binFile.is_open()){
    binFile.read(reinterpret_cast<char *>(&header->magicNumber), sizeof(header->magicNumber));

    ss << "Magic: 0x" << std::hex << header->magicNumber;
    temp=ss.str();
    // string temp = "Magic: 0x";
    //string temp2 = to_string<long>(header->magicNumber, hex);
    setCDKMatrixCell(myMatrix, 1, 1, temp.c_str());
    ss.clear();
    ss.str("");
    binFile.read(reinterpret_cast<char *>(&header->versionNumber), sizeof(header->versionNumber));

    ss << "Version: " << to_string(header->versionNumber);
    temp=ss.str();
    setCDKMatrixCell(myMatrix, 1, 2, temp.c_str());

    ss.clear();
    ss.str("");
    binFile.read(reinterpret_cast<char *>(&header->numRecords), sizeof(header->numRecords));

    ss << "NumRecords: " << header->numRecords;
    temp=ss.str();
    setCDKMatrixCell(myMatrix, 1, 3, temp.c_str());

    for(int i = 0; i <header->numRecords && i < 4; i++){
      ss.str("");
      ss.clear();
      if(!binFile.eof()){

	binFile.read(reinterpret_cast<char *>(&record->strLength), sizeof(record->strLength));
	binFile.read(record->stringBuffer, sizeof(record->stringBuffer));

	ss << "strlen: " << to_string(record->strLength);
	temp=ss.str();
	setCDKMatrixCell(myMatrix, (i + 2), 1, temp.c_str() );

	setCDKMatrixCell(myMatrix, (i + 2), 2, record->stringBuffer );
      }
      
    }

    drawCDKMatrix(myMatrix, true);    /* required  */
		     }
    else
      cout << "Error file did not open" << endl;

    binFile.close();
  /* so we can see results */
  sleep (10);


  // Cleanup screen
  endCDK();
}
