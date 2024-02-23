#include<stdio.h>
#include"lodepng.h"
#include"lodepng.c"
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex;

typedef struct {
unsigned int width;
int startRow;
int endRow;
int constCol;
unsigned int height;
unsigned char *Image;
unsigned char *Image1;
}blurData;

void *blur(void *p) {
blurData *data = (blurData *)p;
unsigned int height = data->height;
unsigned int width = data->width;
unsigned char *Image = (data->Image);
unsigned char *Image1 = (data->Image1), red, green, blue, alpha;
int i, j, sumRed, sumBlue, sumGreen, nofEle, avgVal = 0;
pthread_mutex_lock(&mutex);
for(i = data->startRow;i<=data->endRow; i++) {
for(j = 0; j<data->constCol; j++) {
sumRed = 0;
sumGreen = 0;
sumBlue = 0;
red = Image[4*width*i+4*j+0];
green = Image[4*width*i+4*j+1];
blue = Image[4*width*i+4*j+2];
alpha = Image[4*width*i+4*j+3];

if(i == 0 && j == 0) {
sumRed += red + Image[4*width*(i+1)+4*j+0] + Image[4*width*(i+1)+4*(j+1)+0] + Image[4*width*i+4*(j+1)+0];
sumGreen += green + Image[4*width*(i+1)+4*j+1] + Image[4*width*(i+1)+4*(j+1)+1] + Image[4*width*i+4*(j+1)+1];
sumBlue += blue + Image[4*width*(i+1)+4*j+2] + Image[4*width*(i+1)+4*(j+1)+2] + Image[4*width*i+4*(j+1)+2];

nofEle = 4;
}
else if(i == 0 && j == width-1) {
sumRed += red + Image[4*width*i+4*(j-1)+0] + Image[4*width*(i+1)+4*(j)+0] + Image[4*width*(i+1)+4*(j-1)+0];
sumGreen += green + Image[4*width*i+4*(j-1)+1] + Image[4*width*(i+1)+4*(j)+1] + Image[4*width*(i+1)+4*(j-1)+1];
sumBlue += blue + Image[4*width*i+4*(j-1)+2] + Image[4*width*(i+1)+4*(j)+2] + Image[4*width*(i+1)+4*(j-1)+2];
nofEle = 4;

}
else if(i == height-1 && j == 0) {
sumRed += red + Image[4*width*(i-1)+4*j+0] + Image[4*width*(i-1)+4*(j+1)+0] + Image[4*width*i+4*(j+1)+0];
sumGreen += green + Image[4*width*(i-1)+4*j+1] + Image[4*width*(i-1)+4*(j+1)+1] + Image[4*width*i+4*(j+1)+1];
sumBlue += blue + Image[4*width*(i-1)+4*j+2] + Image[4*width*(i-1)+4*(j+1)+2] + Image[4*width*i+4*(j+1)+2];
nofEle = 4;

}
else if(i == height-1 && j == width-1) {
sumRed += red + Image[4*width*(i-1)+4*j+0] + Image[4*width*i+4*(j-1)+0] + Image[4*width*(i-1)+4*(j-1)+0];
sumGreen += green + Image[4*width*(i-1)+4*j+1] + Image[4*width*i+4*(j-1)+1] + Image[4*width*(i-1)+4*(j-1)+1];
sumBlue += blue + Image[4*width*(i-1)+4*j+2] + Image[4*width*i+4*(j-1)+2] + Image[4*width*(i-1)+4*(j-1)+2];
nofEle = 4;
}
else if(i == 0) {
sumRed += red + Image[4*width*i+4*(j-1)+0] + Image[4*width*(i+1)+4*j+0]  + Image[4*width*i+4*(j+1)+0] + Image[4*width*(i+1)+4*(j+1)+0] + Image[4*width*(i+1)+4*(j-1)+0];
sumGreen += green + Image[4*width*i+4*(j-1)+1] + Image[4*width*(i+1)+4*j+1]  + Image[4*width*i+4*(j+1)+1] + Image[4*width*(i+1)+4*(j+1)+1] + Image[4*width*(i+1)+4*(j-1)+1];
sumBlue += blue + Image[4*width*i+4*(j-1)+2] + Image[4*width*(i+1)+4*j+2]  + Image[4*width*i+4*(j+1)+2] + Image[4*width*(i+1)+4*(j+1)+2] + Image[4*width*(i+1)+4*(j-1)+2];
nofEle = 6;

}
else if(i == height - 1) {
sumRed += red + Image[4*width*(i-1)+4*j+0] + Image[4*width*i+4*(j-1)+0] + Image[4*width*i+4*(j+1)+0] + Image[4*width*(i-1)+4*(j-1)+0] + Image[4*width*(i-1)+4*(j+1)+0];
sumGreen += green + Image[4*width*(i-1)+4*j+1] + Image[4*width*i+4*(j-1)+1] + Image[4*width*i+4*(j+1)+1] + Image[4*width*(i-1)+4*(j-1)+1] + Image[4*width*(i-1)+4*(j+1)+1];
sumBlue += blue + Image[4*width*(i-1)+4*j+2] + Image[4*width*i+4*(j-1)+2] + Image[4*width*i+4*(j+1)+2] + Image[4*width*(i-1)+4*(j-1)+2] + Image[4*width*(i-1)+4*(j+1)+2];
nofEle = 6;

}
else if(j == 0) {
sumRed += red + Image[4*width*i+4*(j+1)+0] + Image[4*width*(i+1)+4*j+0] + Image[4*width*(i-1)+4*j+0] + Image[4*width*(i-1)+4*(j+1)+0] + Image[4*width*(i+1)+4*(j+1)+0];
sumGreen += green + Image[4*width*i+4*(j+1)+1] + Image[4*width*(i+1)+4*j+1] + Image[4*width*(i-1)+4*j+1] + Image[4*width*(i-1)+4*(j+1)+1] + Image[4*width*(i+1)+4*(j+1)+1];
sumBlue += blue + Image[4*width*i+4*(j+1)+2] + Image[4*width*(i+1)+4*j+2] + Image[4*width*(i-1)+4*j+2] + Image[4*width*(i-1)+4*(j+1)+2] + Image[4*width*(i+1)+4*(j+1)+2];
nofEle = 6;

}
else if(j == width - 1) {
sumRed += red + Image[4*width*i+4*(j-1)+0] + Image[4*width*(i-1)+4*j+0] + Image[4*width*(i+1)+4*j+0] + Image[4*width*(i+1)+4*(j-1)+0] + Image[4*width*(i-1)+4*(j-1)+0];
sumGreen += green + Image[4*width*i+4*(j-1)+1] + Image[4*width*(i-1)+4*j+1] + Image[4*width*(i+1)+4*j+1] + Image[4*width*(i+1)+4*(j-1)+1] + Image[4*width*(i-1)+4*(j-1)+1];
sumBlue += blue + Image[4*width*i+4*(j-1)+2] + Image[4*width*(i-1)+4*j+2] + Image[4*width*(i+1)+4*j+2] + Image[4*width*(i+1)+4*(j-1)+2] + Image[4*width*(i-1)+4*(j-1)+2];
nofEle = 6;
}
else {
sumRed += red + Image[4*width*(i+1)+4*j+0] + Image[4*width*(i-1)+4*j+0] + Image[4*width*i+4*(j+1)+0] + Image[4*width*i+4*(j-1)+0] + Image[4*width*(i+1)+4*(j+1)+0] + Image[4*width*(i-1)+4*(j-1)+0] + Image[4*width*(i+1)+4*(j-1)+0] + Image[4*width*(i-1)+4*(j+1)+0];
sumGreen += green + Image[4*width*(i+1)+4*j+1] + Image[4*width*(i-1)+4*j+1] + Image[4*width*i+4*(j+1)+1] + Image[4*width*i+4*(j-1)+1] + Image[4*width*(i+1)+4*(j+1)+1] + Image[4*width*(i-1)+4*(j-1)+1] + Image[4*width*(i+1)+4*(j-1)+1] + Image[4*width*(i-1)+4*(j+1)+1];
sumBlue += blue + Image[4*width*(i+1)+4*j+2] + Image[4*width*(i-1)+4*j+2] + Image[4*width*i+4*(j+1)+2] + Image[4*width*i+4*(j-1)+2] + Image[4*width*(i+1)+4*(j+1)+2] + Image[4*width*(i-1)+4*(j-1)+2] + Image[4*width*(i+1)+4*(j-1)+2] + Image[4*width*(i-1)+4*(j+1)+2];
nofEle = 9;

}

Image1[4*width*i+4*j+0] = sumRed/nofEle;
Image1[4*width*i+4*j+1] = sumGreen/nofEle;
Image1[4*width*i+4*j+2] = sumBlue/nofEle;
Image1[4*width*i+4*j+3] = alpha;
//printf("[%d %d %d %d] ", red, green, blue, alpha);
}
//printf("\n");
}
pthread_mutex_unlock(&mutex);
}

void main() {
printf("Gaussian Blur:\n");
while(1) {

unsigned char *Image, *Image1;
unsigned int error, width, height, red, green, blue, alpha;

char pictureName[20];
printf("Enter the name of the png photo to blur: ");
scanf("%s", pictureName);
error = lodepng_decode32_file(&Image, &width, &height, pictureName);
Image1 = (unsigned char *)malloc(width * height * 4 * sizeof(unsigned char));
if (Image1 == NULL) {
       free(Image);
perror("Memory allocation failed\n");
   }
if(error) {
printf("Error loading file(%d): %s\n", error, lodepng_error_text(error));
}

int numOfThreads, i;
printf("Number of Threads: ");
scanf("%d", &numOfThreads);
blurData data[numOfThreads];
pthread_t thread[numOfThreads];
pthread_mutex_init(&mutex, NULL);

int rangeRow = height / numOfThreads;

for(i = 0; i<numOfThreads; i++) {
if(i == 0) {
data[i].startRow = 0;
}
else {
data[i].startRow = (rangeRow * i) + 1;
}
if(i == numOfThreads - 1) {
data[i].endRow = height-1;
}
else {
data[i].endRow = (i + 1) * rangeRow;
}
data[i].constCol = width;
data[i].Image = Image;
data[i].Image1 = Image1;
data[i].width = width;
data[i].height = height;

if(pthread_create(&thread[i], NULL, blur, &data[i]) != 0) {
perror("Error creating threads..");
}
}
for(i = 0; i<numOfThreads; i++) {
if(pthread_join(thread[i], NULL) != 0) {
perror("Error joining the thread");
}
}



unsigned char *outputImage;
size_t outputImageSize;
char newImageName[20];
error = lodepng_encode32(&outputImage, &outputImageSize, Image1, width, height);
printf("Name of new blurred Image(imgName.png): ");
scanf("%s", newImageName);
if(!error) {
lodepng_save_file(outputImage, outputImageSize, newImageName);
printf("'%s' created successfully !\n",newImageName);
}

free(Image);
free(Image1);
free(outputImage);
pthread_mutex_destroy(&mutex);

char conn;
printf("Do wanna blur another image?(y/n): ");
scanf(" %c", &conn);

if(conn == 'Y' || conn == 'y') {
continue;
}
else {
printf("Thank You for using the Program...");
break;
}
}

} 
 

