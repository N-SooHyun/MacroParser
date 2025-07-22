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



void JsonAPI_Test() {
	using namespace JSON;
	using namespace Dynamic;

	JsonCtrl ctrl(JNode::JType::OBJECT);

	const char* test = "Test";



	JsonCtrl* pCtrl = new JsonCtrl();
	*pCtrl = "Test";
	*pCtrl = JNode::JType::OBJECT;

	delete pCtrl;

	ctrl = R"({"Test" : "a"})";

	ctrl = "{\"a\" : \"b\"}";

	ctrl = R"(["Test", 1, 2])";

	ctrl = "[\"Test\", 1, 2]";

	

	//Get �ִ� �κ� �������̽� ���� �� �ϼ�
	//Get�κ� �Ľ̸� �ϸ� ���� �� �ϼ�



	//Pop ���� �κ� ������ ��
	//int num = ctrl;
	ctrl = 1;

	int num = ctrl;
	double dnum = ctrl;

	ctrl = "Test";

	ctrl = 1;

	char* str = ctrl;

	ctrl = JNode::JType::OBJECT;

	JObj* pobj = ctrl;

	JObj obj = ctrl;

	ctrl = 10;

	JObj *pobj2 = ctrl;

	JObj obj2 = ctrl;

	ctrl = JNode::JType::OBJECT;

	Dynamic::DynamicStr strVarAA(128);
	strVarAA.Set_Str("Test");

	if (strVarAA.StrCat("Test")) std::cout << "��" << std::endl;
	if (!strVarAA.StrCat("Test\n")) std::cout << "��" << std::endl;
	


	JNode Value = ctrl["a"];	//����
	JNode* pvalue = ctrl["b"];	//����

	//���� �����
	JNode testNodeValue(JNode::JType::NUMBER);
	ctrl["a"] = testNodeValue;

	ctrl = JNode::JType::ARRAY;
	
	JNode arrValue = ctrl[1];
	JNode* parrValue = ctrl[1];

	

	int asdfasdf = 10;

	arrValue = ctrl[asdfasdf];

	int ssstest = 10;

	//���� ���� Node����? Obj����? JArr����?

}

void main() {
	//��ũ�� �Լ� �Ľ����ִ� ���α׷� �Դϴ�.
	//Excel�� Seq, Call, Result 3���� ���� ���ؼ� Seq�� Call�� �ְ� ���������� �����ϸ� Result�� ���ɴϴ�.
	//����
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen �б�
	//fopen -> Dataȭ

	//testJsonModel();

	//Timer_Ctrl();

	JsonAPI_Test();

	//No_Timer_Dac();

}