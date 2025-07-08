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

void testJsonModel() {
	JSON::JsonCtrl NullJson;
	JSON::JsonCtrl ObjJson(JSON::JNode::JType::OBJECT);
	JSON::JsonCtrl ArrJson(JSON::JNode::JType::ARRAY);

	NullJson["Test"] = 1;
	ObjJson["Test"] = 1;
	ArrJson["Test"] = 1;

	int num = ObjJson["Test"];
	printf("%d\n", ObjJson["Test"]);

	ObjJson["Test2"] = 0.0;
	double dum = ObjJson["Test2"];
	printf("%f\n", dum);

}

void main() {
	//��ũ�� �Լ� �Ľ����ִ� ���α׷� �Դϴ�.
	//Excel�� Seq, Call, Result 3���� ���� ���ؼ� Seq�� Call�� �ְ� ���������� �����ϸ� Result�� ���ɴϴ�.
	//����
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen �б�
	//fopen -> Dataȭ

	testJsonModel();

	
}