#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//double atof(const char* s);//�����ظ����壬������STDLIB.H
int Initialize();
int Arithtest(int Phy_index, int Log_index); /*����1:�����±� ����2:�߼��±�*/
int ConvertValue(int Phy_index, int Log_index); /*����1:�����±� ����2:�߼��±�*/
void clear(int n);
int JudgeSymbol(int data, int mode);	/*1:�ж��ַ� 2:�ж������ 3:�ж���ֵ*/

int Calculate();
int JudgePrio(int index, int* level, int* ins);			/*�ж����ȼ�*/
int JJud(int index, int* level, int* ins, int n, int bound);	/*������ջ*/
void pop(int sta, int ins, int n);	/*��ջ*/
void Cal(int index, int n);	/*����*/
void ShowCal();			/*������*/
void ShowList(int mode);/*1�������ʽ 2�������ʱת�����ַ���*/

double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double _div(double a, double b);
//double pow(double a, double b); //�����ظ����壬������MATH.H

double(*funs[])(double, double) = { add,sub,mul,_div,pow };
double (**f)(double, double) = funs;

char Symbol[50] = { 0 };
double Value[50] = { 0 };	/*���ʹ���*/
char Stack[50] = { 0 };
char Arith[50] = { 0 };	/*��ʱ�洢*/

char str_symbol[9] = { "0+-*/^()" }; 
char str_value[12] = { "01234567890" };



/*========================*/
int Initialize() {
	int i = 0;
	int p = 0;

	gets_s(Arith);  
	if (Arith[0] == '-') {
		Symbol[0] = 1;
		Value[0] = 0;
		p++;
	}
	for (; i < 50; i++) {
		if (Arith[i] == '\0') break;
		if (JudgeSymbol(Arith[i], 1)) {			/*�ж��ַ�*/
			if (!Arithtest(i, p)) { printf("Arithtest error"); return 0; }
			if (Arith[i] == '-' && Symbol[p - 1] == '(') {
				Symbol[p] = 1;
				Value[p] = 0;
				p++;
			}
			Symbol[p] = Arith[i];
			p++;
			continue;
		}
		if (!JudgeSymbol(Arith[i], 3)) { printf("not Value1 error\n"); return 0; }	/*�ж���ֵ*/

		i = ConvertValue(i, p);
		if (i < 0) return 0;
		Symbol[p] = 1;	/*��־Ϊ1*/

		p++;
	}

	/*ĩλ����Ϊ��ֵ*/
	if (Symbol[p - 1] != 1 && Symbol[p - 1] != ')') { printf("Value error\n"); return 0; }
	return p;
}
int Arithtest(int Phy_index, int Log_index) { /*����1:�����±� ����2:�߼��±�*/
	int i = Phy_index, p = Log_index;

	switch (Arith[i]) {
	case '(': {	/*��������ֵ ���Ҳ����������������*/
		if (Symbol[p - 1] == 1) return 0;
		if (Arith[i + 1] == '-') return 1;
		if (JudgeSymbol(Arith[i + 1], 2) || Arith[i + 1] == ')') return 0;
		break;
	}
	case ')': {	/*������������������ţ��Ҳ�������ֵ*/
		if (JudgeSymbol(Symbol[p - 1], 2) && Symbol[p - 1] == '(') return 0;
		if (JudgeSymbol(Arith[i + 1], 3)) return 0;
		break;
	}
	default: {
		/*���������*/
		if (JudgeSymbol(Arith[i + 1], 2)) return 0;

	}
	}
	return 1;
}
int ConvertValue(int Phy_index, int Log_index) { /*����1:�����±� ����2:�߼��±�*/
	int i = Phy_index;
	int p = Log_index;
	int n = 0, kp = 0;
	for (; i < 50; i++) {
		if (!JudgeSymbol(Arith[i], 3) && Arith[i] != '.') break;
		if (Arith[i] == '.') kp++;
		Stack[n++] = Arith[i];
	}

	if (kp > 1) return -1; /*С��������*/
	Value[p] = atof(Stack);

	clear(n);

	return i - 1;
}
void clear(int n) {
	int i;
	for (i = 0; i < n + 1; i++) {
		Stack[i] = 0;
	}
}
int JudgeSymbol(int data, int mode) {	/*1:�ж��ַ� 2:�ж������ 3:�ж���ֵ*/
	int i;
	switch (mode) {
	case 1: {
		for (i = 1; i < 8; i++) {
			if (data == str_symbol[i]) return i;
		}
		break;
	}
	case 2: {
		for (i = 1; i < 6; i++) {
			if (data == str_symbol[i]) return i;
		}
		break;
	}
	case 3: {
		for (i = 1; i < 11; i++) {
			if (data == str_value[i]) return i;
		}
		break;
	}
	default:
		printf(" error in mode \n");
	}
	return 0;
}

int Calculate() {
	int n = 0, level = 0;		/*n:�߼��������� level�����ȼ�*/

	int bracks[10] = { 0 };
	int brack = 0;
	int brack_level[10] = { 0 };

	int index = 0, ins = 0;		/*index:��¼�±� ins:��ջ������*/

	n = Initialize();

	if (n < 1) { printf("Initialize error\n"); return 0; }
	ShowList(1);

	for (; index < n; index++) {
		if (Symbol[index] == 1 || Symbol[index] == 0) continue;

		if (Symbol[index] == '(') {
			bracks[brack] = ins;
			brack++;
			continue;
		}
		if (Symbol[index] == ')') {
			/*�����Ų�����*/
			if (!brack) { printf(" brack error \n"); return 0; }

			brack--;
			pop(bracks[brack], ins - 1, n);	/*�ͷŵ�ǰ����ջ*/
			ins = bracks[brack];
			brack_level[brack] = 0;		/*���ȼ�����*/
			continue;
		}
		if (brack) {
			JJud(index, &brack_level[brack - 1], &ins, n, bracks[brack - 1]);
			continue;
		}
		JJud(index, &level, &ins, n, 0);
	}
	if (brack) return 0;
	if (ins > 0) pop(0, ins - 1, n);

	ShowCal();
	return 1;
}
int JudgePrio(int index, int* level, int* ins) {			/*�ж����ȼ�*/
	int i = 0;
	i = JudgeSymbol(Symbol[index], 2);
	if (*level == 0 || i > * level) {	/*�ж�¼��*/
		Stack[*ins] = index;
		(*ins)++;
		*level = i;
		return 1;
	}
	return 0;
}
int JJud(int index, int* level, int* ins, int n, int bound) {	/*������ջ*/

	while (!JudgePrio(index, level, ins) && *ins > bound) {
		Cal(Stack[*ins - 1], n);
		Stack[*ins - 1] = index;
		(*ins)--;

		if (*ins == bound) {
			*level = 0;
			if (JudgePrio(index, level, ins)) return 1;
			return 0;
		}
		*level = JudgeSymbol(Symbol[Stack[*ins - 1]], 2);
	}
	*level = JudgeSymbol(Symbol[index], 2);
	return 1;
}
void pop(int sta, int ins, int n) {	/*��ջ*/
	// printf("\n======pop=======\n"); //ȡ������ע��
	for (; sta <= ins; ins--) {
		Cal(Stack[ins], n);
	}
}
void Cal(int index, int n) {	/*����*/
	int k = 0;
	double ros = 0;
	int lef = index - 1, rig = index + 1;

	while (Symbol[lef] != 1 && lef > 0) lef--;
	while (Symbol[rig] != 1 && rig < n - 1) rig++;

	k = JudgeSymbol(Symbol[index], 2) - 1;
	//printf("k=%d lef=%d rig=%d\n", k, lef, rig); //ȡ������ע��
	ros = f[k](Value[lef], Value[rig]);

	Symbol[index] = 1;
	Value[index] = ros;
	Symbol[lef] = 0;
	Symbol[rig] = 0;

}
void ShowCal() {			/*������*/
	int i;
	for (i = 0; i < 50; i++) {
		if (Symbol[i] == 1) printf("=%f", Value[i]);
	}
}
void ShowList(int mode) { /*1�������ʽ 2�������ʱת�����ַ���*/
	int i;
	switch (mode) {
	case 1: {
		for (i = 0; i < 50; i++) {
			if (Symbol[i] == '\0') continue;
			if (Symbol[i] == 1) { printf("%f", Value[i]); }
			else printf("%c", Symbol[i]);
		}
		break;
	}
	case 3: {
		for (i = 0; i < 50; i++) {
			if (Stack[i] == '\0') break;
			else printf("%c", Stack[i]);
		}
		break;
	}
	default:
		break;
	}

}
double add(double a, double b) {
	//printf("\n 			a=%f add b=%f \n", a, b);//ȡ������ע��
	return a + b;
}
double sub(double a, double b) {
	//printf("\n 			a=%f sub b=%f \n", a, b)//ȡ������ע��
	return a - b;

}
double mul(double a, double b) {
	//printf("\n 			a=%f mul b=%f \n", a, b);//ȡ������ע��
	return a * b;
}
double _div(double a, double b) {
	if (b < 0) printf("div error");
	return a / b;
}
