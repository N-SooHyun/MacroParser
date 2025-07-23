#include "Main.h"

void testDynamicStrMain() {
	const char* test = "Hello";

	char s = 'a';

	Dynamic::DynamicStr testStr(5);

	testStr.Set_Str("HelloMy");

	char* GetStr = testStr.Get_Str();

	printf("%s\n", GetStr);

	testStr.Append_Char("a");

	GetStr = testStr.Get_Str();

	printf("%s\n", GetStr);
}




void JsonAPI_Test2(){
	using namespace JSON;
	using namespace Dynamic;

//enum���� ���� �� ����
	JsonCtrl SnglTyp;							//NULLTYPE
	JsonCtrl NumTyp(JNode::JType::NUMBER);		//0
	JsonCtrl DblTyp(JNode::JType::DOUBLE);		//0.0
	JsonCtrl BlTyp = JNode::JType::BOOL;		//true
	JsonCtrl StrTyp = JNode::JType::STRING;		//
	JsonCtrl charTyp = JNode::JType::CHAR;		

	int num = 0;
	int* pnum = &num;
	double dnum = 0;
	double* pdnum = &dnum;
	bool bl = false;
	bool* pbl = &bl;
	char c = 'c';
	char* str = "Test";

//���� Ÿ�� �׽�Ʈ------------------------------------------------------------------------------------------
//<Operator ()= ����>	//������ �������
	SnglTyp = num;		//operator=(int number)
	SnglTyp = pnum;		//operator=(int* number)
	SnglTyp = dnum;		//operator=(double number)
	SnglTyp = pdnum;	//operator=(double* number)
	SnglTyp = bl;		//operator=(bool boolean)
	SnglTyp = pbl;		//operator=(bool* boolean)
	SnglTyp = str;		//operator=(const char* str)
	SnglTyp = c;		//operator=(const char c)

//<Operator() ��ȯ>		//��ȯ�����ϸ� -1�̶���� null���� ������
	num = SnglTyp;		//operator int()		���н� -1 ��ȯ
	pnum = SnglTyp;		//operator int*()		���н� nullptr��ȯ
	dnum = SnglTyp;		//operator double()		���н� -1 ��ȯ
	pdnum = SnglTyp;	//operator double*()	���н� nullptr��ȯ
	bl = SnglTyp;		//operator bool()		���н� false ��ȯ
	pbl = SnglTyp;		//operator bool*()		���н� nullptr ��ȯ
	c = SnglTyp;		//operator char()		���н� '\0' ��ȯ
	str = SnglTyp;		//operator char*()		���н� nullptr ��ȯ


//Obj Ÿ�� �׽�Ʈ-------------------------------------------------------------------------------------------
	char* key = "Key";
	SnglTyp = JNode::JType::OBJECT;
	JsonCtrl* test = new JsonCtrl();


	
//<operator() = ����>
	//Node["key"] -> Node->Obj->Node�� ���� ���� ������ ����
	//SnglTyp["Key"] = num;
	//SnglTyp[key] = num;
	SnglTyp["Key"];

//<operator() ��ȯ>
	//num = SnglTyp["Key"];
	//num = SnglTyp[key];


	return;



}

void main() {
	//��ũ�� �Լ� �Ľ����ִ� ���α׷� �Դϴ�.
	//Excel�� Seq, Call, Result 3���� ���� ���ؼ� Seq�� Call�� �ְ� ���������� �����ϸ� Result�� ���ɴϴ�.
	//����
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen �б�
	//fopen -> Dataȭ

	//testJsonModel();

	//Timer_Ctrl();

	//JsonAPI_Test();

	JsonAPI_Test2();

	//No_Timer_Dac();

}