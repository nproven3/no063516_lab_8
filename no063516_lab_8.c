#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;
void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);
void mergeSort(int pData[], int l, int r);

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	 int i;
	 int temp;
    // Uses recursion for the heapify function
    for (i = n / 2 - 1; i >= 0; i--)
	{
    	heapify(arr, n, i);
	}
    //Goes through all elements until the end
    for (i = n - 1; i > 0; i--) {
        // Swap current index with first element
        temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        //Call the heapify function again on the reduced heaps
        heapify(arr, i, 0);
    }
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1; //Left child 
    int r = 2 * i + 2; //Right child
    int temp;
    //If left child is larger than root
    if (l < n && arr[l] > arr[largest])
	{
        largest = l;
	}
    //If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
	{
        largest = r;
	}
    //If largest is not equal to index
    if (largest != i) {
        //Swap the index with the largest child
        temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        //Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}


//implement merge sort
//extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
        int mid = (l + r) / 2; 
        mergeSort(pData, l, mid); //Divides array into two halves
        mergeSort(pData, mid + 1, r);

        //Merge the two sorted subarrays
        int TEMP1 = mid - l + 1;
        int TEMP2 = r - mid;
        
        int * LEFT = (int *)malloc(sizeof(int) * TEMP1);
        //extraMemoryAllocated += sizeof(LEFT); //Gets data of LEFT to extraMem
        int * RIGHT = (int *)malloc(sizeof(int) * TEMP2);
        //extraMemoryAllocated += sizeof(RIGHT); //Gets data of RIGHT to extraMem

        for (int i = 0; i < TEMP1; i++) { //Fills the arrays with data
            LEFT[i] = pData[l + i];
        }
        for (int j = 0; j < TEMP2; j++) {
            RIGHT[j] = pData[mid + 1 + j];
        }

        int i = 0;
        int j = 0; //resets all values to zero
        int k = l;
        while (i < TEMP1 && j < TEMP2) {
            if (LEFT[i] <= RIGHT[j]) {
                pData[k] = LEFT[i];
                i++;
            } else {
                pData[k] = RIGHT[j];
                j++;
            }
            k++;
        }
        while (i < TEMP1) {
            pData[k] = LEFT[i];
            i++;
            k++;
        }
        while (j < TEMP2) {
            pData[k] = RIGHT[j];
            j++;
            k++;
        }

        free(LEFT);
        free(RIGHT); //Frees the memory
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	if(dataSz < 100)
	{
		for(int i = 0; i < dataSz; i++)
		{
			printf("%d ", pData[i]);
		}
	}else{

		int i, sz = dataSz - 100;
		printf("\tData:\n\t");
		for (i=0;i<100;++i)
		{
			printf("%d ",pData[i]);
		}
		printf("\n\t");
	
		for (i=sz;i<dataSz;++i)
		{
			printf("%d ",pData[i]);
		}
		printf("\n\n");
	}
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
		//If statement below just makes code look prettier
		printf("\n");
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		if(dataSz < 100)
		{
			printf("\n"); //Just makes output more readable
			printf("\n");
		}

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		extraMemoryAllocated = dataSz * 4; //Finds the total memory allocated for the functions in bytes
		mergeSort(pDataCopy, 0, dataSz - 1);
		
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}