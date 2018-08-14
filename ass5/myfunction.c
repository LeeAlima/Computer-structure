// 313467441 Lee Alima

/*
* MOTIVES:
* 1. In general, I preferrd using local variables over reading from
* the memory or using global variables.
* 2. Sometimes I decided to use pointers over reading directly from memory address
* 3. I tried to avoid many funcion calls, doing it directly in the caller function
* it's faster than calling a function from it.
* 4. I made the function less generic and more "built-in" to the 2 different
* possible masks
* 5. I preffered creating and reading information inside inner funcion over
* delivering the information as arguments.
* 6. I choose to transform the constructive loop of applyKernel into 9 readings
* from src and updating the sum of each parameter
* 7. I used arithmetic operations as little as I could think, eg: I multiplied
* just once after adding all of the info to the sum of colors instead  of multipling
* after each adding to the sum
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

// I decided to user macro functions because it works faster
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

/*
 * I decides to split the smooth functions into 2 functions,
 * in this way I can get the kernelScale in O(1) and can even
 * know the weight of each cell in the matrix because each function
 * fits only one mask.
 */

// smooth function for Blur mask(kernel)
void smoothBlur(int dim, pixel *src, pixel *dst) {
	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/

	int i, j, iDim;
	int end = dim - 1;
	register pixel p;
	pixel current_pixel;
	// save those variable in registers in order to use them faster (there is a lot
	// of use in them ) , moreover, I decided to use integers local variables instead
	// of using the sum_pixel struct because local vars are faster to use.
	// There is no use in numS
	register int redSum, greenSum, blueSum;
	for (i = 1 ; i < end; ++i) {
		// calculate iDim only dim-2 times
		iDim = i*dim;
		for (j =  1 ; j < end ; ++j) {
			++iDim;
			// I chose to iniatialize the field inside the function and not in an
			// another function because it's faster not to call another function
			redSum = 0;
			greenSum = 0;
			blueSum = 0;
			
			// I use location a lot so I save it in a register
			int register location = (i-1)*dim;
			location += (j-1);

			// going over all of the cells in src, and add there field to the red, green
			// and blue sums.
			// I add one to location to move in row and I added dim-2 to it to move
			// to the next col
			p = src[location];
			redSum += p.red;
			greenSum += p.green;
			blueSum += p.blue;

			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			// move to the second line in the matrix
			location += (dim-2);
			p = src[location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			// middle
			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;
			
			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			// move to the third line in the matrix
			location += (dim-2);
			p = src[location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			// devide each sum by the kernel scale(9)
			redSum /= 9;
			greenSum /= 9;
			blueSum /= 9;

			// truncate each pixel's color values to match the range [0,255]	
			current_pixel.red = (unsigned char) (min(max(redSum, 0), 255));
			current_pixel.green = (unsigned char) (min(max(greenSum, 0), 255));
			current_pixel.blue = (unsigned char) (min(max(blueSum, 0), 255));

			dst[iDim] = current_pixel;

		}
	}
}

/*
 * very similar to smoothBlur.
 * In this function, I couln't avoid multiplying by the weight of each cell in the mask,
 * So, I added all of the cells of src instead of the one representing the middle in the
 * matrix and than multiplied them by -1. Just then I added the middle one * 9
 */
void smoothSharpen(int dim, pixel *src, pixel *dst) {
	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int i, j, iDim;
	int end = dim - 1;
	register pixel p;
	pixel current_pixel;
	register int redSum, greenSum, blueSum;
	// there is a need to save the middle cell location
	int middleLocation;	
	for (i = 1 ; i < end; ++i) {
		iDim = i*dim;
		for (j =  1 ; j < end ; ++j) {
			++iDim;
			redSum = 0;
			greenSum = 0;
			blueSum = 0;

			int register location = (i-1)*dim;
			location += (j-1);

			p = src[location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;
			
			// move to the next line in the matrix
			location += (dim-2);
			p = src[location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			// save the middle location, using ++location instead of location++ to make
			// it faster
			middleLocation = ++location;
			
			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			location += (dim-2);
			p = src[location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			p = src[++location];
			redSum += p.red ;
			greenSum += p.green;
			blueSum += p.blue;

			// mult by -1 (the weight)
			redSum *= -1;
			greenSum *= -1;
			blueSum *= -1;

			// adding the middle pixel and mult it by its weight (9)
			p = src[middleLocation];
			redSum += p.red * 9 ;
			greenSum += p.green * 9;
			blueSum += p.blue *9;

			// truncate each pixel's color values to match the range [0,255]	
			current_pixel.red = (unsigned char) (min(max(redSum, 0), 255));
			current_pixel.green = (unsigned char) (min(max(greenSum, 0), 255));
			current_pixel.blue = (unsigned char) (min(max(blueSum, 0), 255));

			dst[iDim] = current_pixel;

		}
	}
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName) {
	pixel *pixelsImg ;
	pixel *backupOrg ;
	int mSize = m;
	int size = mSize*n*sizeof(pixel);

	// I didn't use doconvolutions ,as I wrote in my motives at the beggining of the file,
	// I preferred not to call functions
	// save image data in pixelsImg and allocate place to backUpOrg
	// by using casting there is no need to call charsToPixels and pixelsToChars
	pixelsImg = (pixel *)image->data;
	backupOrg = malloc(size);

	// memcpy is faster than copyPixels
	memcpy(backupOrg,pixelsImg,size);
	// smooth for blur image
	smoothBlur(mSize, backupOrg, pixelsImg);
	free(backupOrg);
	writeBMP(image, srcImgpName, blurRsltImgName);

	// sharpen the resulting image
	pixelsImg = (pixel *)image->data;
	backupOrg =  malloc(size);
	
	// memcpy is faster than copy pixel
	memcpy(backupOrg,pixelsImg,size);
	// call for sharp image
	smoothSharpen(mSize, backupOrg, pixelsImg);
	free(backupOrg);
	// write result image to file
	writeBMP(image, srcImgpName, sharpRsltImgName);
}
