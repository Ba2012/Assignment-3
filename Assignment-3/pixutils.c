#include "pixutils.h"
#include "lodepng.h"


//private methods
static pixMap *pixMap_init(); //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
static void pixMap_reset();  //free the allocated memoray and set to zero but do not free memory for pixMap
static void pixMap_copy(pixMap *dest,pixMap *source); //copy one pixmap to another
static int pixMap_read(pixMap *p,char *filename); //read in a pixmap from file

static pixMap* pixMap_init(){
	pixMap *p = malloc(sizeof(pixMap));
	 p-> width = 0;
	 p-> height = 0;
	 p-> pixArray = 0;
	 p-> image = 0;
	return p;
	
 //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
}	
pixMap* pixMap_init_filename(char *filename){
	
	pixMap *p = pixMap_init();
	pixMap_read(p,filename);
	//pixMap *temp = pixMap_init();
	//pixMap_copy(temp, p);		
	return p;
}
static int pixMap_read(pixMap *p,char *filename){
	 

	
	lodepng_decode32_file(&(p->image), &(p->width), &(p->height), filename);
	fprintf(stderr, "width = %d\nHeight = %d\n", (p->width), (p->height));
	p->pixArray = (malloc(p->height*sizeof(rgba*)));
	
	p->pixArray[0] = (rgba*) p->image;
	for(int i = 1; i<p->height; i++) {
		p->pixArray[i] = (p->pixArray[i-1])+(p->width);
	}	
	return 0;
}	
static void pixMap_copy(pixMap *dest,pixMap *source){
	fprintf(stderr, "COPY SOURCE\n");
	//if source image is zero then reset dest and copy width and height
	if(!source->image) {
		dest->height = 0;
		dest->width = 0;
		dest->image = 0;
		dest->pixArray = 0;
	}
		//if source image is not zero
	else {
		
	  //if width or height are different
		if(dest->height != source->height || dest->width != source->width){
		//if width*height is different then we need to allocate dest->image			
			if((dest->height)*(dest->width) != (source->height)*(source->width)) {
			
			//if dest->image is zero use malloc to allocate memory for dest->image	
				if(dest->image ==0) {
					fprintf(stderr, "Dest Image = 0\n");
					dest->image = malloc((source->height)*(source->width)*sizeof(rgba));
					//fprintf(stderr, "dest->image %d\n", dest->image);
					
				} 
				//else use realloc to allocate memory dest->image
				else { dest->image = realloc(dest->image,(source->height)*(source->width)*sizeof(rgba));
					fprintf(stderr, "Dest Image realloc\n");
				//if dest->height is different
				}
			}
		}	
				if(dest->height != source->height) {
					if(!dest->pixArray) {
						fprintf(stderr, "!dest->pixArray\n");
						//malloc or realloc dest->pixArray depending on whether dest->pixArray is zero
						dest->pixArray = malloc(source->height*sizeof(rgba*));
						 //even if the height is the same set dest->pixArray[0] to dest->image and dest->pixArray[i]=dest->pixArray[i-1]+source->width
						
						
					}
					
					else {
						fprintf(stderr, "dest->pixArray\n");
						dest->pixArray = realloc(dest->pixArray, source->height * sizeof(rgba*));
						
											
						}
					}
				}
			
			
		dest->pixArray[0] = (rgba*) dest->image;
		for(int i = 1; i<source->height; i++) {
			dest->pixArray[i] = dest->pixArray[i-1]+dest->width;
		}
	
   
	  //do a memcpy from source->image to dest->image
	  //set dest->width and dest->height to source values
	//
	memcpy(dest->image, source->image, ((source->height)*(source->width))*4); 
	dest->width = source->width;
	dest->height = source->height;
	
	}	
	


static void pixMap_reset(pixMap *p){
	

	free(p->pixArray);
	free(p->image);
	p-> width = 0;
	p-> height = 0;
	p-> pixArray = 0;
	p-> image = 0;

	//free all the memory in the fields and set everything to zero
}	


void pixMap_destroy(pixMap *p){
	if(p) {
		free(p->pixArray);
		free(p->image);
		free(p);
	}
	//reset pixMap and free the object p
}


void pixMap_rotate (pixMap *p, float theta){
 //make a new temp blank pixMap structure
 pixMap *temp=pixMap_init();
 //copy p to temp
 pixMap_copy(temp, p);
 //set the values in the image to zero using memset - Note that the 3rd argument of memset is the size in BYTES
memset(temp->image, 0, (p->height)*(p->width)*4);
 //calculate the coordinates ox and oy of the middle of the png graphic
 //double x=(temp->width)/2;
 //double y=(temp->height)/2;

int ox=(int)(temp->width)/2;
int oy=(int)(temp->height)/2;
//fprintf(stderr, "X: %d\nY:%d\n", ox,oy);
 
 //calculate the values of sine and cosine used by the rotation formula 

	for(int y=0;y<p->height;y++){   //two for loops to loop through each pixel in the original
		for(int x=0;x<p->width;x++){
			float rads = degreesToRadians(-theta);
			double c = cos(rads);
			double s = sin(rads);
			
			float rotx = c*(x-ox) -s *(oy-y) +ox;
			float roty = -(s*(x-ox) + c *(oy-y) -oy);
			//fprintf(stderr, "Old x: %d\nOld y: %d\n",x,y);
			//fprintf(stderr, "new x: %.4f\nNew y: %.4f\n",rotx,roty);
			//check if the new x and y are within the confines of the image
			//fprintf(stderr, "%d\n" ,(temp->pixArray[roty][rotx]).r);
			int rotateX = (int)(rotx +0.5);
			int rotateY = (int)(roty +0.5);
			//fprintf(stderr, "NEWEST x: %d\nNEWEST y: %d\n",rotateX,rotateY);
			if((rotateY < (p->height) && rotateY >=0) && ((rotateX < (p->width) ) && rotateX >=0)){		
				//memcpy(temp->pixArray[y]+x, p->pixArray[rotateY] + rotateX,sizeof(rgba));
				memcpy(temp->pixArray[rotateY]+rotateX,p->pixArray[y]+x,(sizeof(rgba)*4));
		}
		
		}
		
	}

 	   //round the coordinates to the nearest integer in your calculations (add 0.5 and cast to integer)	
	
	    //if rotated coordinates are within the height and width limits
	      //copy the pixel at the old coords to the pixel to the temporary copy using memcpy
	      //i.e. 	memcpy(temp->pixArray[roty]+rotx,p->pixArray[y]+x,sizeof(rgba))
	    //
	    
	  //
	//  
	//copy the temp pixMap to the original
	pixMap_copy(p, temp);
	//destroy the temp;
	pixMap_destroy(temp);
}	

void pixMap_gray (pixMap *p){
	//for() loop through pixels using two for loops 
	for(int i=0; i<((p->height)); i++) {
		
		for(int j=0; j<((p->width)); j++) {
			//fprintf(stderr, "Red %d\nGreen %d\nBlue %d\n", p->pixArray[i][j].r, p->pixArray[i][j].g, p->pixArray[i][j].b);
			float r = p->pixArray[i][j].r;
			//printf(stderr, "RED %d\n", p->pixArray[i][j].r);
			float g = p->pixArray[i][j].g;
			//fprintf(stderr, "GREEN %d\n", p->pixArray[i][j].g);
			float b = p->pixArray[i][j].b; 
			//fprintf(stderr, "BLUE %d\n", p->pixArray[i][j].b);
			int gray = ((r+g+b)/3) +0.5;
			p->pixArray[i][j].r = gray;
			//fprintf(stderr, "RED Gray %d\n", p->pixArray[i][j].r);
			p->pixArray[i][j].g = gray;
			//fprintf(stderr, "GREEN Gray %d\n", p->pixArray[i][j].g);
			p->pixArray[i][j].b = gray;
			//fprintf(stderr, "BLUE GRAY %d\n", p->pixArray[i][j].b);
			//fprintf(stderr, "AFTER GRAY!! Red %d\nGreen %d\nBlue %d\n", p->pixArray[i][j].r, p->pixArray[i][j].g, p->pixArray[i][j].b);
		}
	}
			
	  //for()
	    //calculate average value of r and g and b values (assign to a float variable)
	    //for example the red value of for the pixel at height i, width j would be p->pixel[i][j].r
	    //round float variable to integer (add 0.5 before casting to integer)
	    //assign the rounded value to r,g and b values
	  //
	//   
}
int pixMap_write(pixMap *p,char *filename){
	//this might be what it needs to do...
	
	lodepng_encode32_file(filename, p->image, p->width, p->height);

 //write out to filename using lodepng_encode32_file
 //example is in lodepng/examples - one liner
	return 0;
}	 
