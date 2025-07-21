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




	//Get 넣는 부분 인터페이스 거의 다 완성
	//Get부분 파싱만 하면 거의 다 완성



	//Pop 빼는 부분 만들어야 함
	//int num = ctrl;


}

void main() {
	//매크로 함수 파싱해주는 프로그램 입니다.
	//Excel에 Seq, Call, Result 3개의 열을 통해서 Seq와 Call을 넣고 실행파일을 실행하면 Result가 나옵니다.
	//구조
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen 읽기
	//fopen -> Data화

	//testJsonModel();

	//Timer_Ctrl();
	
	JsonAPI_Test();

	//No_Timer_Dac();

}