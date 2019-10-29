#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100

//파일이름 지정은 여기서!!
char* fileName = "Assembler2.txt";

typedef struct string
{
	char* str;
}String;

typedef struct label
{
	char* str;
	int lc;

}Label;

int main(void)
{
	FILE* fp;

	//firstpass 시작/////////////////////////////////////////////////////////////////////////
	char data[20];
	fp = fopen(fileName, "r");
	int LC = 0;
	int line = 0;
	char Del[4] = { ' ',',','\t','\n' };
	Label* LabelTable = (Label*)malloc(sizeof(Label)*MAX);//라벨테이블
	int index_Label = 0;

	while (fgets(data, 20, fp) != NULL)
	{
		int flag = 0;
		for (int i = 0; i < strlen(data); i++)
		{
			if (data[i] == ',')
			{
				flag = 1;
				break;
			}
		}
		char* ptr = strtok(data, Del);
		if (flag == 1)//라벨인 경우
		{
			LabelTable[index_Label].str = (char*)malloc(strlen(ptr));
			strcpy(LabelTable[index_Label].str, ptr);
			LabelTable[index_Label].lc = LC;
			ptr = strtok(NULL, Del);
			index_Label++;
			LC++;
			line++;
		}
		else//라벨이 아닌 경우
		{
			if (!strcmp(ptr, "ORG"))//ORG인 경우, LC 세팅
			{
				ptr = strtok(NULL, Del);
				LC = atoi(ptr);
			}
			else//아닌 경우
			{
				if (!strcmp(ptr, "END"))//END인 경우, firstpass 끝
				{
					break;
				}
				else
				{
					LC++;
					line++;
				}
			}
		}
	}
	fclose(fp);
	//firstpass 완료 -> 라벨 테이블 작성 완료////////////////////////////////////////////////

	typedef struct _binary
	{
		int bin[4];
	}Binary;

	Binary** binary = (Binary**)malloc(sizeof(Binary*)*line);//이진 값을 저장할 테이블
	for (int i = 0; i < line; i++)
	{
		binary[i] = (Binary*)malloc(sizeof(Binary) * 4);
	}

	int Pseudo_num = 4;
	String* PseudoTable = (String*)malloc(sizeof(String) * Pseudo_num);//슈도테이블
	for (int i = 0; i < Pseudo_num; i++)
	{
		PseudoTable[i].str = (char*)malloc(sizeof(char) * 3);
	}
	strcpy(PseudoTable[0].str, "ORG");
	strcpy(PseudoTable[1].str, "END");
	strcpy(PseudoTable[2].str, "DEC");
	strcpy(PseudoTable[3].str, "HEX");

	int MRI_num = 7;
	String* MRITable = (String*)malloc(sizeof(String) * MRI_num);//MRI테이블
	for (int i = 0; i < MRI_num; i++)
	{
		MRITable[i].str = (char*)malloc(sizeof(char) * 3);
	}
	strcpy(MRITable[0].str, "AND");
	strcpy(MRITable[1].str, "ADD");
	strcpy(MRITable[2].str, "LDA");
	strcpy(MRITable[3].str, "STA");
	strcpy(MRITable[4].str, "BUN");
	strcpy(MRITable[5].str, "BSA");
	strcpy(MRITable[6].str, "ISZ");

	typedef struct non_mri
	{
		char* str;
		char code[5];
	}Non_mri;
	int nonMRI_num = 18;
	Non_mri* nonMRITable = (Non_mri*)malloc(sizeof(Non_mri) * nonMRI_num);//nonMRI테이블
	for (int i = 0; i < nonMRI_num; i++)
	{
		nonMRITable[i].str = (char*)malloc(sizeof(char) * 4);
	}
	strcpy(nonMRITable[0].str, "CLA");
	strcpy(nonMRITable[0].code, "7800");
	strcpy(nonMRITable[1].str, "CLE");
	strcpy(nonMRITable[1].code, "7400");
	strcpy(nonMRITable[2].str, "CMA");
	strcpy(nonMRITable[2].code, "7200");
	strcpy(nonMRITable[3].str, "CME");
	strcpy(nonMRITable[3].code, "7100");
	strcpy(nonMRITable[4].str, "CIR");
	strcpy(nonMRITable[4].code, "7080");
	strcpy(nonMRITable[5].str, "CIL");
	strcpy(nonMRITable[5].code, "7040");
	strcpy(nonMRITable[6].str, "INC");
	strcpy(nonMRITable[6].code, "7020");
	strcpy(nonMRITable[7].str, "SPA");
	strcpy(nonMRITable[7].code, "7010");
	strcpy(nonMRITable[8].str, "SNA");
	strcpy(nonMRITable[8].code, "7008");
	strcpy(nonMRITable[9].str, "SZA");
	strcpy(nonMRITable[9].code, "7004");
	strcpy(nonMRITable[10].str, "SZE");
	strcpy(nonMRITable[10].code, "7002");
	strcpy(nonMRITable[11].str, "HLT");
	strcpy(nonMRITable[11].code, "7001");
	strcpy(nonMRITable[12].str, "INP");
	strcpy(nonMRITable[12].code, "F800");
	strcpy(nonMRITable[13].str, "OUT");
	strcpy(nonMRITable[13].code, "F400");
	strcpy(nonMRITable[14].str, "SKI");
	strcpy(nonMRITable[14].code, "F200");
	strcpy(nonMRITable[15].str, "SKO");
	strcpy(nonMRITable[15].code, "F100");
	strcpy(nonMRITable[16].str, "ION");
	strcpy(nonMRITable[16].code, "F080");
	strcpy(nonMRITable[17].str, "IOF");
	strcpy(nonMRITable[17].code, "F040");

	//secondpass 시작////////////////////////////////////////////////////////////////////////
	fp = fopen(fileName, "r");
	LC = 0;
	int Binary_line = 0;//LC테이블 참조 변수
	int Binary_Tableline = 0;//binary테이블 참조 변수
	int* LC_Table = (int*)malloc(sizeof(int)*line + 1);
	while (fgets(data, 20, fp) != NULL)
	{
		char* ptr = strtok(data, Del);//label이면 패스
		for (int i = 0; i < index_Label; i++)
		{
			if (!strcmp(ptr, LabelTable[i].str))
			{
				ptr = strtok(NULL, Del);
				break;
			}
		}
		int flag_Pseudo = 0;
		for (int i = 0; i < Pseudo_num; i++)//Pseudo-instruction인지 조사
		{
			if (!strcmp(ptr, PseudoTable[i].str))
			{
				flag_Pseudo = 1;
				break;
			}
		}

		if (flag_Pseudo == 1)//Pseudo-instruction이라면
		{
			if (!strcmp(ptr, "ORG"))//ORG라면
			{
				ptr = strtok(NULL, Del);
				LC = atoi(ptr);
			}
			else//ORG가 아니라면
			{
				if (!strcmp(ptr, "END"))//END라면
				{
					break;//secondpass 끝////////////////////////////////////////////////////
				}
				else//END가 아니라면
				{
					if (!strcmp(ptr, "DEC"))
					{
						ptr = strtok(NULL, Del);
						int dec = atoi(ptr);
						char* Hex = (char*)calloc(5, sizeof(char));

						if (dec < 0)//음수일 경우 -> 보수처리 해주어야 함.
						{
							dec = -dec;
							//부호 뺀 상태에서 2진수 변환
							sprintf(Hex, "%04x", dec);
							int h, len = strlen(Hex), buf[4];
							while (len--)
							{
								h = *Hex > 96 ? *Hex++ - 87 : *Hex++ - 48;
								for (int i = 3; i >= 0; i--)
								{
									buf[i] = h % 2;
									h = h / 2;
								}
								for (int i = 0; i < 4; i++)
								{
									binary[Binary_Tableline][3 - len].bin[i] = buf[i];
								}
							}
							//보수처리
							int bo[16];
							int num = 0;
							for (int i = 0; i < 4; i++)
							{
								for (int j = 0; j < 4; j++)
								{
									bo[num++] = binary[Binary_Tableline][i].bin[j];
								}
							}

							for (int i = 0; i < 16; i++)
								bo[i] = !bo[i];

							num = 0;
							if (bo[15] == 0)
							{
								bo[15] = 1;
								for (int i = 0; i < 4; i++)
								{
									for (int j = 0; j < 4; j++)
									{
										binary[Binary_Tableline][i].bin[j] = bo[num++];
									}
								}
							}
							else
							{
								for (int i = 15; i >= 0; i--)
								{
									if (bo[i - 1] == 0)
									{
										bo[i] = !bo[i];
										bo[i - 1] = !bo[i - 1];
										break;
									}
									bo[i] = !bo[i];
								}
								for (int i = 0; i < 4; i++)
								{
									for (int j = 0; j < 4; j++)
									{
										binary[Binary_Tableline][i].bin[j] = bo[num++];
									}
								}
							}//보수처리 완료
						}
						else//음수 아닐 경우
						{
							sprintf(Hex, "%04x", dec);
							int h, len = strlen(Hex), buf[4];
							while (len--)
							{
								h = *Hex > 96 ? *Hex++ - 87 : *Hex++ - 48;
								for (int i = 3; i >= 0; i--)
								{
									buf[i] = h % 2;
									h = h / 2;
								}
								for (int i = 0; i < 4; i++)
								{
									binary[Binary_Tableline][3 - len].bin[i] = buf[i];
								}
							}
						}
					}

					if (!strcmp(ptr, "HEX"))
					{
						ptr = strtok(NULL, Del);
						char* Hex = (char*)calloc(5, sizeof(char));

						if (ptr[0] == '-')//음수일 경우 -> 보수처리 해주어야 함.
						{
							for (int i = 0; i < strlen(ptr); i++)
								ptr[i] = ptr[i + 1];

							//부호 뺀 상태에서 2진수 변환
							sprintf(Hex, "%04s", ptr);
							int h, len = strlen(Hex), buf[4];
							while (len--)
							{
								h = *Hex > 96 ? *Hex++ - 87 : *Hex++ - 48;
								for (int i = 3; i >= 0; i--)
								{
									buf[i] = h % 2;
									h = h / 2;
								}
								for (int i = 0; i < 4; i++)
								{
									binary[Binary_Tableline][3 - len].bin[i] = buf[i];
								}
							}
							//보수처리
							int bo[16];
							int num = 0;
							for (int i = 0; i < 4; i++)
							{
								for (int j = 0; j < 4; j++)
								{
									bo[num++] = binary[Binary_Tableline][i].bin[j];
								}
							}

							for (int i = 0; i < 16; i++)
								bo[i] = !bo[i];

							num = 0;
							if (bo[15] == 0)
							{
								bo[15] = 1;
								for (int i = 0; i < 4; i++)
								{
									for (int j = 0; j < 4; j++)
									{
										binary[Binary_Tableline][i].bin[j] = bo[num++];
									}
								}
							}
							else
							{
								for (int i = 15; i >= 0; i--)
								{
									if (bo[i - 1] == 0)
									{
										bo[i] = !bo[i];
										bo[i - 1] = !bo[i - 1];
										break;
									}
									bo[i] = !bo[i];
								}
								for (int i = 0; i < 4; i++)
								{
									for (int j = 0; j < 4; j++)
									{
										binary[Binary_Tableline][i].bin[j] = bo[num++];
									}
								}
							}//보수처리 완료
						}
						else
						{
							sprintf(Hex, "%04s", ptr);
							int h, len = strlen(Hex), buf[4];
							while (len--)
							{
								h = *Hex > 96 ? *Hex++ - 87 : *Hex++ - 48;
								for (int i = 3; i >= 0; i--)
								{
									buf[i] = h % 2;
									h = h / 2;
								}
								for (int i = 0; i < 4; i++)
								{
									binary[Binary_Tableline][3 - len].bin[i] = buf[i];
								}
							}
						}
					}
					LC_Table[Binary_line++] = LC++;
					Binary_Tableline++;
				}
			}
		}
		else//Pseudo-instruction이 아니라면
		{
			int flag_MRI = 0;
			for (int i = 0; i < MRI_num; i++)//MRI인지 조사
			{
				if (!strcmp(ptr, MRITable[i].str))
				{
					flag_MRI = 1;
					break;
				}
			}
			if (flag_MRI == 1)//MRI라면
			{
				char* Hex = (char*)malloc(sizeof(char) * 4);
				int num_o = 0;
				for (int i = 0; i < MRI_num; i++)
				{
					if (strcmp(ptr, MRITable[i].str))
					{
						num_o++;
						continue;
					}
					break;
				}
				char* operation = (char*)malloc(sizeof(char) * 1);
				itoa(num_o, operation, 10);//operation code 1자리

				ptr = strtok(NULL, Del);
				int num_a = 0;
				for (int i = 0; i < index_Label; i++)
				{
					if (strcmp(LabelTable[i].str, ptr))
					{
						num_a++;
						continue;
					}
					break;
				}
				char* address = (char*)malloc(sizeof(char) * 3);
				itoa(LabelTable[num_a].lc, address, 10);//address symbol 3자리

				sprintf(Hex, "%s%s", operation, address);//4자리 16진수 완성

				int h, len = strlen(Hex), buf[4];
				while (len--)
				{
					h = *Hex > 96 ? *Hex++ - 87 : *Hex++ - 48;
					for (int i = 3; i >= 0; i--)
					{
						buf[i] = h % 2;
						h = h / 2;
					}
					for (int i = 0; i < 4; i++)
					{
						binary[Binary_Tableline][3 - len].bin[i] = buf[i];
					}
				}
				Binary_Tableline++;

				ptr = strtok(NULL, Del);
				if (ptr != NULL)
				{
					if (!strcmp(ptr, "I"))//그 다음에 I가 있다면
					{
						binary[Binary_Tableline - 1][0].bin[0] = 1;
						LC_Table[Binary_line++] = LC++;
					}
					else//I가 없다면
					{
						binary[Binary_Tableline - 1][0].bin[0] = 0;
						LC_Table[Binary_line++] = LC++;
					}
				}
				else
					LC_Table[Binary_line++] = LC++;
			}
			else//MRI가 아니라면
			{
				int flag_nonMRI = 0;
				int num = 0;
				for (int i = 0; i < nonMRI_num; i++)
				{
					if (strcmp(ptr, nonMRITable[i].str))
					{
						num++;
						continue;
					}
					else
					{
						flag_nonMRI = 1;
						break;
					}
				}
				if (flag_nonMRI == 1)//nonMRI라면
				{
					char* Hex = (char*)malloc(sizeof(char) * 4);
					Hex = nonMRITable[num].code;
					int h, len = strlen(Hex), buf[4];
					while (len--)
					{
						h = *Hex > 96 ? *Hex++ - 87 : *Hex++ - 48;
						for (int i = 3; i >= 0; i--)
						{
							buf[i] = h % 2;
							h = h / 2;
						}
						for (int i = 0; i < 4; i++)
						{
							binary[Binary_Tableline][3 - len].bin[i] = buf[i];
						}
					}
					Binary_Tableline++;
					LC_Table[Binary_line++] = LC++;
				}
				else//nonMRI가 아니라면
				{
					printf("Error in %dline of code\n", LC);
					LC_Table[Binary_line++] = LC++;
				}
			}
		}
	}

	for (int i = 0; i < line; i++)
	{
		printf("%d  ", LC_Table[i]);
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				printf("%d", binary[i][j].bin[k]);
			}
			printf(" ");
		}
		printf("\n");
	}

	return 0;
}