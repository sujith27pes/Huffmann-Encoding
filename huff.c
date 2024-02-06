#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char unencodedmessage[256] = "";

typedef struct Node Node;

struct Node {
	char alphabet;
	unsigned f;
	Node *left, *right;
};

typedef struct {
	Node* node;
} NodeWrapper;

NodeWrapper heap[256];
int heapSize = 0;

int cmp(const void* a, const void* b) {
	Node *nodeA = ((NodeWrapper *)a)->node;
	Node *nodeB = ((NodeWrapper *)b)->node;
	return (nodeA->f > nodeB->f) - (nodeA->f < nodeB->f);
}

void push(Node* value) {
	NodeWrapper wrapper = {value};
	heap[heapSize++] = wrapper;
	qsort(heap, heapSize, sizeof(NodeWrapper), cmp);
}

Node* pop() {
	NodeWrapper wrapper = heap[0];
	memmove(&heap[0], &heap[1], (--heapSize) * sizeof(NodeWrapper));
	qsort(heap, heapSize, sizeof(NodeWrapper), cmp);
	return wrapper.node;
}

Node* newNode(char alphabet, unsigned freq) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->alphabet = alphabet;
	node->f = freq;
	node->left = NULL;
	node->right = NULL;
	return node;
}

Node *mainroot;

typedef struct {
	char key;
	char value[256];
} DictEntry;

DictEntry dict[256];
int dictSize = 0;

void addToDict(char key, const char* value) {
	dict[dictSize].key = key;
	strcpy(dict[dictSize].value, value);
	dictSize++;
}

const char* getDictValue(char key) {
	for(int i = 0; i < dictSize; i++) {
		if(dict[i].key == key)
			return dict[i].value;
	}
	return NULL;
}

void display(Node *root, char* s) {
	if (!root)
		return;
	if (root->alphabet != '~') {
		printf("%c:%s\n", root->alphabet, s);

		addToDict(root->alphabet, s);
	}
	char s0[256], s1[256];
	strcpy(s0, s);
	strcpy(s1, s);
	display(root->left, strcat(s0, "0"));
	display(root->right, strcat(s1, "1"));
}

void compress(char *a, int *b, int len) {
	Node *l, *r, *root;
	for (int i = 0; i < len; ++i) {
		push(newNode(a[i], b[i]));
	}
	while (heapSize != 1) {
		l = pop();
		r = pop();
		root = newNode('~', l->f + r->f);
		root->left = l;
		root->right = r;
		push(root);
	}
	mainroot = root;
}

void decompress(Node* root, char* encodedMessage) {
	char originalMessage[256] = "";
	int index = 0;
	while (*encodedMessage) {
		Node* current = root;
		while (current->left || current->right) {
			if (*encodedMessage == '0') {
				current = current->left;
			} else {
				current = current->right;
			}
			encodedMessage++;
		}
		originalMessage[index++] = current->alphabet; 
	}
	originalMessage[index] = '\0'; // Null-terminate the original message string.
	printf("\nDecompressed Message: %s", originalMessage);
}

void decToBinary(int n, char* ans) {

	int binaryNum[32];
	int i = 0;
	while (n) {
		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}

	ans[0] = '\0';
	for (int j = i - 1; j >= 0; j--) {
		char bit[2] = {binaryNum[j] + '0', '\0'};
		strcat(ans, bit);
	}
	int rem = 8 - strlen(ans);
	while (rem--) {
		strcat(ans, "0");
	}
	strcat(unencodedmessage, ans);
}

void helper(char* s) {
	while (*s) {
		char ans[256];
		decToBinary(*s, ans);
		s++;
	}
}

double savedspace(int a, int b) {
	double ans = (double)(a - b);
	ans /= (double)a;

	return ans * 100.0;
}

DictEntry frequencyEntries[256];
int frequencyEntriesCount = 0;

void addToFrequencyEntries(char key, int freq) {
	for (int i = 0; i < frequencyEntriesCount; i++) {
		if (frequencyEntries[i].key == key) {
			frequencyEntries[i].value[0] += freq;
			return;
		}
	}
	frequencyEntries[frequencyEntriesCount].key = key;
	frequencyEntries[frequencyEntriesCount].value[0] = freq;
	frequencyEntriesCount++;
}

int main() {
	char s[256];
	printf("Enter the Message: ");
	scanf("%s", s);
	int frequency[256] = {0};
	char charvector[256];
	int freqvector[256];
	int vectorSize = 0;

	for (int i = 0; i < strlen(s); i++) {
		frequency[s[i]]++;
	}

	for (int i = 0; i < 256; i++) {
		if (frequency[i]) {
			addToFrequencyEntries(i, frequency[i]);
			charvector[vectorSize] = i;
			freqvector[vectorSize] = frequency[i];
			vectorSize++;
		}
	}

	compress(charvector, freqvector, vectorSize);
	helper(s);

	char compressedval[256] = "";
	double gg = savedspace(8 * strlen(s), 2 * strlen(compressedval) + 8 * vectorSize);

	if (gg <= 0) {
		printf("Huffmann Encoding is not preferred for these type of messages\n");
		return 0;
	}
	else {
		printf("\nThe Encoding Chart \n");
		char emptyString[1] = "";
		display(mainroot, emptyString);
		printf("\nBefore Compression\n");
		printf("%s -> %s\n", s, unencodedmessage);
		printf("After Compression\n");
		printf("%s -> ", s);
		for(int i = 0; i < strlen(s); i++) {
			strcat(compressedval, getDictValue(s[i]));
		}
		printf("%s\n", compressedval);
		printf("\nSpace Saved: %.2f%%", gg);

		decompress(mainroot, compressedval);
	}
	printf("\n");
	return 0;
}
