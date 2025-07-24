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
	//JsonCtrl NumTyp(JNode::JType::NUMBER);		//0
	//JsonCtrl DblTyp(JNode::JType::DOUBLE);		//0.0
	//JsonCtrl BlTyp = JNode::JType::BOOL;		//true
	//JsonCtrl StrTyp = JNode::JType::STRING;		//
	//JsonCtrl charTyp = JNode::JType::CHAR;		

	int num = 0;
	int* pnum = &num;
	double dnum = 0;
	double* pdnum = &dnum;
	bool bl = false;
	bool* pbl = &bl;
	char c = 'c';
	const char* str = "Test";

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

	pnum = &num;
	pdnum = &dnum;
	pbl = &bl;
	str = "Test";
	
//<operator() = ����>
	//Node["key"] -> Node->Obj->Node�� ���� ���� ������ ����
	//SnglTyp["Key"] = num;
	//SnglTyp[key] = num;
	SnglTyp["Key0"] = 1;
	SnglTyp["Key1"] = pnum;
	SnglTyp["Key2"] = dnum;
	SnglTyp["Key3"] = pdnum;
	SnglTyp["Key4"] = bl;
	SnglTyp["Key5"] = pbl;
	SnglTyp["Key6"] = c;
	SnglTyp["Key7"] = str;
	SnglTyp["Key0"] = num;
	SnglTyp["Key1"] = 1;


	SnglTyp["Key0"] = pnum;
	SnglTyp["Key1"] = 1;
	SnglTyp["Key2"] = pdnum;
	SnglTyp["Key3"] = dnum;
	SnglTyp["Key4"] = pbl;
	SnglTyp["Key5"] = bl;
	SnglTyp["Key6"] = str;
	SnglTyp["Key7"] = c;
	//SnglTyp["Key0"] = num;
	SnglTyp["Key1"] = 9;
	SnglTyp["Key8"] = "Test";
	SnglTyp["Key8"] = "Ts";
	SnglTyp["Key8"] = 1;
	



//<operator() ��ȯ>
	num = SnglTyp["Key1"];
	pnum = SnglTyp["Key0"];
	dnum = SnglTyp["Key3"];
	pdnum = SnglTyp["Key2"];
	bl = SnglTyp["Key5"];
	pbl = SnglTyp["Key4"];
	const char* str2 = SnglTyp["Key6"];
	c = SnglTyp["Key7"];



	std::cout << num << std::endl;
	std::cout << pnum << std::endl;
	std::cout << dnum << std::endl;
	std::cout << pdnum << std::endl;
	std::cout << bl << std::endl;
	std::cout << pbl << std::endl;
	std::cout << str2 << std::endl;
	std::cout << c << std::endl;

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