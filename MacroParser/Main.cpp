#include "Main.h"

#include "Timer_Thread.h"

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