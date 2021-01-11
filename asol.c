/* Assembler for LC */

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXLINELENGTH 1000
#define MAXNUMLABELS 65536
#define MAXLABELLENGTH 7 /* includes the null character termination */

#define ADD 0
#define NAND 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5
#define HALT 6
#define MUL 7
#define DIV 8
#define IMUL 9
#define XIDIV 10
#define AND 11
#define XOR 12
#define CMPGE 13
#define JMAE 14
#define JMNAE 15
#define BSR 16 
#define BSF 17
#define JNE 18
#define PUSH 19
#define POP 20

#define ADD_OPCODE "add"
#define NAND_OPCODE "nand"
#define LW_OPCODE "lw"
#define SW_OPCODE "sw"
#define BEQ_OPCODE "beq"
#define JALR_OPCODE "jalr"
#define HALT_OPCODE "halt"
#define MUL_OPCODE "mul"
#define DIV_OPCODE "div" 
#define IMUL_OPCODE "imul"
#define XIDIV_OPCODE "xidiv"
#define AND_OPCODE "and"
#define XOR_OPCODE "xor"
#define CMPGE_OPCODE "cmpge"
#define JMAE_OPCODE "jmae"
#define JMNAE_OPCODE "jmnae"
#define BSR_OPCODE "bsr"
#define BSF_OPCODE "bsf"
#define JNE_OPCODE "jne"
#define PUSH_OPCODE "push"
#define POP_OPCODE "pop"
#define FILL_DERECTIVE_OPCODE ".fill"

int readAndParse(FILE*, char*, char*, char*, char*, char*);
int translateSymbol(char labelArray[MAXNUMLABELS][MAXLABELLENGTH], int labelAddress[MAXNUMLABELS], int, char*);
int isNumber(char*);
void testRegArg(char*);
void testAddrArg(char*);

int main(int argc, char* argv[])
{
	char* inFileString, * outFileString;
	FILE* inFilePtr, * outFilePtr;
	int address;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH], argTmp[MAXLINELENGTH];
	int i;
	int numLabels = 0;
	int num;
	int addressField;

	char labelArray[MAXNUMLABELS][MAXLABELLENGTH];
	int labelAddress[MAXNUMLABELS];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
			argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* map symbols to addresses */

	/* assume address start at 0 */
	//printf("start check opcode \n");
	for (address = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
		address++) {

		//printf("%d: label=%s, opcode=%s, arg0=%s, arg1=%s, arg2=%s\n",
		//	address, label, opcode, arg0, arg1, arg2);

		//printf("check for illegal opcode \n");
		/* check for illegal opcode */
		if (strcmp(opcode, ADD_OPCODE) && strcmp(opcode, NAND_OPCODE) &&
			strcmp(opcode, LW_OPCODE) && strcmp(opcode, SW_OPCODE) &&
			strcmp(opcode, BEQ_OPCODE) && strcmp(opcode, JALR_OPCODE) &&
			strcmp(opcode, HALT_OPCODE) && strcmp(opcode, MUL_OPCODE) &&
			strcmp(opcode, FILL_DERECTIVE_OPCODE) && strcmp(opcode, DIV_OPCODE) &&
			strcmp(opcode, IMUL_OPCODE) && strcmp(opcode, XIDIV_OPCODE) &&
			strcmp(opcode, AND_OPCODE) && strcmp(opcode, XOR_OPCODE) &&
			strcmp(opcode, CMPGE_OPCODE) && strcmp(opcode, JMAE_OPCODE) &&
			strcmp(opcode, JMNAE_OPCODE) && strcmp(opcode, BSR_OPCODE) &&
			strcmp(opcode, BSF_OPCODE) && strcmp(opcode, JNE_OPCODE) &&
			strcmp(opcode, PUSH_OPCODE) && strcmp(opcode, POP_OPCODE)) {
			printf("error: unrecognized opcode %s at address %d\n", opcode,
				address);
			exit(1);
		}

		//printf("check register fields \n");
		/* check register fields */
		if (!strcmp(opcode, ADD_OPCODE) || !strcmp(opcode, NAND_OPCODE) ||
			!strcmp(opcode, LW_OPCODE) || !strcmp(opcode, SW_OPCODE) ||
			!strcmp(opcode, BEQ_OPCODE) || !strcmp(opcode, JALR_OPCODE) ||
			!strcmp(opcode, MUL_OPCODE) || !strcmp(opcode, DIV_OPCODE) ||
			!strcmp(opcode, IMUL_OPCODE) || !strcmp(opcode, XIDIV_OPCODE) ||
			!strcmp(opcode, AND_OPCODE) || !strcmp(opcode, XOR_OPCODE) ||
			!strcmp(opcode, CMPGE_OPCODE) || !strcmp(opcode, JMAE_OPCODE) ||
			!strcmp(opcode, JMNAE_OPCODE) || !strcmp(opcode, BSR_OPCODE) ||
			!strcmp(opcode, BSF_OPCODE)) {
			testRegArg(arg0);
			testRegArg(arg1);
		}
		if (!strcmp(opcode, PUSH_OPCODE) || !strcmp(opcode, POP_OPCODE)) {
			testRegArg(arg0);
		}
		if (!strcmp(opcode, ADD_OPCODE) || !strcmp(opcode, NAND_OPCODE) ||
			!strcmp(opcode, MUL_OPCODE) || !strcmp(opcode, DIV_OPCODE) ||
			!strcmp(opcode, IMUL_OPCODE) || !strcmp(opcode, XIDIV_OPCODE) ||
			!strcmp(opcode, AND_OPCODE) || !strcmp(opcode, XOR_OPCODE) ||
			!strcmp(opcode, CMPGE_OPCODE)) {
			testRegArg(arg2);
		}

		//printf("check addressField \n");
		/* check addressField */
		if (!strcmp(opcode, LW_OPCODE) || !strcmp(opcode, SW_OPCODE) ||
			!strcmp(opcode, BEQ_OPCODE) || !strcmp(opcode, JMAE_OPCODE) ||
			!strcmp(opcode, JMNAE_OPCODE)) {
			testAddrArg(arg2);
		}
		if (!strcmp(opcode, FILL_DERECTIVE_OPCODE) || !strcmp(opcode, JNE_OPCODE)) {
			testAddrArg(arg0);
		}

		//printf("check for enough arguments \n");
		/* check for enough arguments */
		if ((strcmp(opcode, HALT_OPCODE) && strcmp(opcode, FILL_DERECTIVE_OPCODE) &&
			strcmp(opcode, JALR_OPCODE) && strcmp(opcode, BSR_OPCODE) &&
			strcmp(opcode, BSF_OPCODE) && strcmp(opcode, JNE_OPCODE) &&
			strcmp(opcode, PUSH_OPCODE) && strcmp(opcode, POP_OPCODE) && arg2[0] == '\0') ||
			((!strcmp(opcode, JALR_OPCODE) || !strcmp(opcode, BSR_OPCODE) ||
				!strcmp(opcode, BSF_OPCODE)) &&
				arg1[0] == '\0') ||
			((!strcmp(opcode, FILL_DERECTIVE_OPCODE) || !strcmp(opcode, PUSH_OPCODE) ||
				!strcmp(opcode, POP_OPCODE)) && arg0[0] == '\0')) {
			printf("error at address %d: not enough arguments\n", address);
			exit(2);
		}

		//printf("label check start \n");
		if (label[0] != '\0') {
			/* check for labels that are too long */
			if (strlen(label) >= MAXLABELLENGTH) {
				printf("label too long\n");
				exit(2);
			}

			/* make sure label starts with letter */
			if (!sscanf(label, "%[a-zA-Z]", argTmp)) {
				printf("label doesn't start with letter\n");
				exit(2);
			}

			/* make sure label consists of only letters and numbers */
			sscanf(label, "%[a-zA-Z0-9]", argTmp);
			if (strcmp(argTmp, label)) {
				printf("label has character other than letters and numbers\n");
				exit(2);
			}

			/* look for duplicate label */
			for (i = 0; i < numLabels; i++) {
				if (!strcmp(label, labelArray[i])) {
					printf("error: duplicate label %s at address %d\n",
						label, address);
					exit(1);
				}
			}
			/* see if there are too many labels */
			if (numLabels >= MAXNUMLABELS) {
				printf("error: too many labels (label=%s)\n", label);
				exit(2);
			}

			strcpy(labelArray[numLabels], label);
			labelAddress[numLabels++] = address;
		}
		//printf("label check end \n");

	}

	//printf("end check opcode \n");

	for (i = 0; i < numLabels; i++) {
		/* printf("%s = %d\n", labelArray[i], labelAddress[i]); */
	}

	/* now do second pass (print machine code, with symbols filled in as
	addresses) */
	rewind(inFilePtr);
	for (address = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
		address++) {
		if (!strcmp(opcode, ADD_OPCODE)) {

			num = (ADD << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, NAND_OPCODE)) {

			num = (NAND << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, JALR_OPCODE)) {

			num = (JALR << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16);

		}
		else if (!strcmp(opcode, HALT_OPCODE)) {

			num = (HALT << 24);

		}
		else if (!strcmp(opcode, MUL_OPCODE)) {

			num = (MUL << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, DIV_OPCODE)) {

			num = (DIV << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, IMUL_OPCODE)) {

			num = (IMUL << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, XIDIV_OPCODE)) {

			num = (XIDIV << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, AND_OPCODE)) {

			num = (AND << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, XOR_OPCODE)) {

			num = (XOR << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, CMPGE_OPCODE)) {

			num = (CMPGE << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
				| atoi(arg2);

		}
		else if (!strcmp(opcode, BSR_OPCODE)) {

			num = (BSR << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16);

		}
		else if (!strcmp(opcode, BSF_OPCODE)) {

			num = (BSF << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16);

		}
		else if (!strcmp(opcode, PUSH_OPCODE)) {

			num = (PUSH << 24) | (atoi(arg0) << 20);

		}
		else if (!strcmp(opcode, POP_OPCODE)) {

			num = (POP << 24) | (atoi(arg0) << 20);

		}
		else if (!strcmp(opcode, LW_OPCODE) || !strcmp(opcode, SW_OPCODE) ||
			!strcmp(opcode, BEQ_OPCODE) || !strcmp(opcode, JMAE_OPCODE) || !strcmp(opcode, JMNAE_OPCODE)) {

			/* if arg2 is symbolic, then translate into an address */
			if (!isNumber(arg2)) {
				addressField = translateSymbol(labelArray, labelAddress,
					numLabels, arg2);
				/*
				printf("%s being translated into %d\n", arg2, addressField);
				*/
				if (!strcmp(opcode, BEQ_OPCODE) || !strcmp(opcode, JMAE_OPCODE) || !strcmp(opcode, JMNAE_OPCODE)) {
					addressField = addressField - address - 1;
				}
			}
			else {
				addressField = atoi(arg2);
			}

			if (addressField < -32768 || addressField > 32767) {
				printf("error: offset %d out of range\n", addressField);
				exit(1);
			}

			/* truncate the offset field, in case it's negative */
			addressField = addressField & 0xFFFF;

			if (!strcmp(opcode, BEQ_OPCODE)) {
				num = (BEQ << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
					| addressField;
			}
			else if (!strcmp(opcode, JMAE_OPCODE)) {
				num = (JMAE << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
					| addressField;
			}
			else if (!strcmp(opcode, JMNAE_OPCODE)) {
				num = (JMNAE << 24) | (atoi(arg0) << 20) | (atoi(arg1) << 16)
					| addressField;
			}
			else {
				/* lw or sw */
				if (!strcmp(opcode, LW_OPCODE)) {
					num = (LW << 24) | (atoi(arg0) << 20) |
						(atoi(arg1) << 16) | addressField;
				}
				else {
					num = (SW << 24) | (atoi(arg0) << 20) |
						(atoi(arg1) << 16) | addressField;
				}
			}

		}
		else if (!strcmp(opcode, JNE_OPCODE)) {

			if (!isNumber(arg0)) {
				addressField = translateSymbol(labelArray, labelAddress,
					numLabels, arg0);
				addressField = addressField - address - 1;
			}
			else {
				addressField = atoi(arg0);
			}

			if (addressField < -32768 || addressField > 32767) {
				printf("error: offset %d out of range\n", addressField);
				exit(1);
			}

			/* truncate the offset field, in case it's negative */
			addressField = addressField & 0xFFFF;

			num = (JNE << 24) | addressField;

		}
		else if (!strcmp(opcode, FILL_DERECTIVE_OPCODE)) {

			if (!isNumber(arg0)) {
				num = translateSymbol(labelArray, labelAddress, numLabels,
					arg0);
			}
			else {
				num = atoi(arg0);
			}

		}
		/* printf("(address %d): %d (hex 0x%x)\n", address, num, num); */
		fprintf(outFilePtr, "%d\n", num);
	}

	exit(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE* inFilePtr, char* label, char* opcode, char* arg0,
	char* arg1, char* arg2)
{
	//printf("read and parce \n");

	char line[MAXLINELENGTH];
	char* ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long */
	if (strlen(line) == MAXLINELENGTH - 1) {
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
		opcode, arg0, arg1, arg2);
	return(1);
}

int translateSymbol(char labelArray[MAXNUMLABELS][MAXLABELLENGTH],
	int labelAddress[MAXNUMLABELS], int numLabels, char* symbol)
{
	int i;

	/* search through address label table */
	for (i = 0; i < numLabels && strcmp(symbol, labelArray[i]); i++) {
	}

	if (i >= numLabels) {
		printf("error: missing label %s\n", symbol);
		exit(1);
	}

	return(labelAddress[i]);
}

int isNumber(char* string)
{
	/* return 1 if string is a number */
	int i;
	return((sscanf(string, "%d", &i)) == 1);
}

/*
 * Test register argument; make sure it's in range and has no bad characters.
 */
void testRegArg(char* arg)
{
	int num;
	char c;

	if (atoi(arg) < 0 || atoi(arg) > 7) {
		printf("error: register out of range\n");
		exit(2);
	}
	if (sscanf(arg, "%d%c", &num, &c) != 1) {
		printf("bad character in register argument\n");
		exit(2);
	}
}

/*
 * Test addressField argument.
 */
void testAddrArg(char* arg)
{
	int num;
	char c;

	/* test numeric addressField */
	if (isNumber(arg)) {
		if (sscanf(arg, "%d%c", &num, &c) != 1) {
			printf("bad character in addressField\n");
			exit(2);
		}
	}
}
