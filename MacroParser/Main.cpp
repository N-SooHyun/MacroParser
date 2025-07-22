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

	

	//Get 넣는 부분 인터페이스 거의 다 완성
	//Get부분 파싱만 하면 거의 다 완성



	//Pop 빼는 부분 만들어야 함
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

	if (strVarAA.StrCat("Test")) std::cout << "굿" << std::endl;
	if (!strVarAA.StrCat("Test\n")) std::cout << "굿" << std::endl;
	


	JNode Value = ctrl["a"];	//복사
	JNode* pvalue = ctrl["b"];	//참조

	//삽입 만들기
	JNode testNodeValue(JNode::JType::NUMBER);
	ctrl["a"] = testNodeValue;

	ctrl = JNode::JType::ARRAY;
	
	JNode arrValue = ctrl[1];
	JNode* parrValue = ctrl[1];

	

	int asdfasdf = 10;

	arrValue = ctrl[asdfasdf];

	int ssstest = 10;

	//뺄때 모든건 Node기준? Obj기준? JArr기준?

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