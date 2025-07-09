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

	//NullJson["Test"] = 1;
	ObjJson["Test"] = 1;
	ObjJson["Test"] = 2;
	ObjJson["Test2"] = 2;
	//ArrJson["Test"] = 1;

	int num = ObjJson["Test"];

}

void main() {
	//매크로 함수 파싱해주는 프로그램 입니다.
	//Excel에 Seq, Call, Result 3개의 열을 통해서 Seq와 Call을 넣고 실행파일을 실행하면 Result가 나옵니다.
	//구조
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen 읽기
	//fopen -> Data화

	testJsonModel();

	
}