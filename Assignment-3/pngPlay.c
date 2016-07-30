#include "pixutils.h"

int main(int argc, char *argv[]){
  char *inputfile=0,*outputfile=0;
  float degrees=0, grayFlag=0;
  
  //write the parser yourself or use a package like https://github.com/skeeto/optparse


int i;
for(i=0; i<argc; i++){
	fprintf(stderr, "argv[%d] = %s\n", i, argv[i]);
}

	if(argc>9||argc<3) {
		fprintf(stderr, "Not the right number of argument\n");
		//printf(stdout, "Number of args: %d",argc);
		return(1);
	}

	int j=1;

	while(j<argc){
		if(strcmp(argv[j], "-i") ==0){
			inputfile=argv[j+1];
			fprintf(stderr, "the input file is %s\n" , inputfile);
			j+=2;
		}
		else if (strcmp(argv[j],"-o") ==0) {
			outputfile=argv[j+1];
			fprintf(stderr, "the output file is %s\n" , outputfile);
			j+=2;


		}
		else if(strcmp(argv[j], "-r")==0) {
			degrees=atof(argv[j+1]);
			fprintf(stderr, "the degree is %f\n" , degrees);
			j+=2;
		}
		else if(strcmp(argv[j], "-g")==0) {
			grayFlag=1;
			fprintf(stderr, "Gray Scale true\n");
			//fprintf(stderr, "the gray scale is %f\n" , grayFlag);
			j+=1;
		}

	}
  //check for flags -i -o -r -g - can be in any order
  //-i is followed by the input png
  //-o is followed the output png
  //-r is followd by the rotation angle in degrees (float) <optional for user>
  //-g is whether th png should be grayed <optional for user>
  
  pixMap *p=pixMap_init_filename(inputfile);
  if(degrees)pixMap_rotate(p,degrees);
  if(grayFlag)pixMap_gray(p);
  pixMap_write(p,outputfile);
  pixMap_destroy(p);
  
  return 0;
}

