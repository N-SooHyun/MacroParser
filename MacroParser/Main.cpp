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
	JSON::JsonCtrl example_json(JSON::JNode::JType::OBJECT);
	
	//Test
	example_json["name"] = "John Doe";
	const char* name = "name2";

	example_json[name] = 1;

	example_json["obj"] = JSON_OBJ;

	
}

void main() {
	//��ũ�� �Լ� �Ľ����ִ� ���α׷� �Դϴ�.
	//Excel�� Seq, Call, Result 3���� ���� ���ؼ� Seq�� Call�� �ְ� ���������� �����ϸ� Result�� ���ɴϴ�.
	//����
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen �б�
	//fopen -> Dataȭ

	testJsonModel();

	
}