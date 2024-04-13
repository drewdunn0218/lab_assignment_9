#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
    struct RecordType *data; // Array of pointers to RecordType to hold records
    struct HashType *next;
};

// Compute the hash function
int hash(int x)
{
    const int size = 23;
    return (x % size); 
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	if(inFile == NULL){
		printf("failed to open");
	}
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			return(0);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    printf("\nRecords in Hash:\n");
    for (int i = 0; i < hashSz; ++i)
    {
        struct HashType *current = &pHashArray[i];
		if(current != NULL){
		printf("index %d -> ", i);
        while (current != NULL)
        {
            if (current->data != NULL)
                printf("%d, %c, %d", current->data->id, current->data->name, current->data->order);
            current = current->next;
			if(current != NULL){
				printf(" -> ");
			}
        }
        printf("\n");
    }
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
  	struct HashType *hashTable = (struct HashType*)malloc(sizeof(struct HashType) * 23);
	for (int i = 0; i < 23; ++i) {
        hashTable[i].data = NULL;
        hashTable[i].next = NULL;
    }
for (int i = 0; i < recordSz; ++i) {
        int index = hash(pRecords[i].id);
        if (hashTable[index].data == NULL) {
            hashTable[index].data = &pRecords[i];
        } else {
            // Handle collision by chaining
            struct HashType *current = &hashTable[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = (struct HashType*)malloc(sizeof(struct HashType));
            if (current->next == NULL) {
                printf("Failed to allocate memory for chaining\n");
                return -1; // Error code
            }
            current->next->data = &pRecords[i];
            current->next->next = NULL;
        }
    }

displayRecordsInHash(hashTable, recordSz);

    // Free memory allocated for records
    free(pRecords);
/*
    // Free memory allocated for hash table
    for (int i = 0; i < recordSz; ++i) {
        struct HashType *current = &hashTable[i];
        while (current != NULL) {
            struct HashType *temp = current;
            current = current->next;
.            free(temp);
        }
    }
	*/
    free(hashTable);


}