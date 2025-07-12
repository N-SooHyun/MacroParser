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

void testJsonModel() {
	using namespace JSON;
	using namespace Dynamic;
	//예시 "{"A" : "B"}" frstJson
	JSON::JNode frstJson(JNode::JType::OBJECT);
	JObj* obj = static_cast<JObj*>(frstJson.Get_Type());

	JsonCtrl NullCtrl;	//Nulltype
	JsonCtrl ObjCtrl(JNode::JType::OBJECT);	//Object type
	JsonCtrl ArrCtrl(JNode::JType::ARRAY);	//Array type
	JsonCtrl NumCtrl(JNode::JType::NUMBER);	//Number type
	JsonCtrl StrCtrl(JNode::JType::STRING);	//String type
	JsonCtrl BolCtrl(JNode::JType::BOOL);	//Boolean type
	JsonCtrl DblCtrl(JNode::JType::DOUBLE);	//Double type


	//Json이 단일 타입일때 값을 넣어주는 메소드 호출법
	DblCtrl.Set_Value(12.1);
	NumCtrl.Set_Value(123);
	BolCtrl.Set_Value(true);

	StrCtrl.Set_Value("Hello");
	StrCtrl.Set_Value("World");
	StrCtrl.Force_Set_Value(1);

	int da = NullCtrl.Get_Value<int>();
	double d = DblCtrl.Get_Value<double>();
	std::cout << "DblCtrl Value: " << d << std::endl;
	int n = DblCtrl.Get_Value<int>();
	std::cout << "DblCtrl Value as int: " << n << std::endl;
	bool b = DblCtrl.Get_Value<bool>();
	std::cout << "DblCtrl Value as bool: " << b << std::endl;
	




	//객체["키"] = 값;   Obj일때 넣는 방식1
	//객체[var] = 값;   Obj일때 넣는 방식2 

	//배열[인덱스] = 값;   Arr일때 넣는 방식1
	//배열.Push(값);   Arr일때 넣는 방식2

	//단일.Get_Value();   값 가져오는 방식
	//단일.Get_Type();   타입 가져오는 방식
	//단일.Get_Str();   문자열 가져오는 방식
	//단일.Get_Int();   정수 가져오는 방식
	//단일.Get_Double();   실수 가져오는 방식
	//단일.Get_Bool();   불리언 가져오는 방식
	//단일.Set_Str("문자열");   문자열 설정하는 방식
	//단일.Set_Int(정수);   정수 설정하는 방식
	//단일.Set_Double(실수);   실수 설정하는 방식
	//단일.Set_Bool(불리언);   불리언 설정하는 방식
	//단일.Set_Type(타입);   타입 설정하는 방식
	//단일.Set_Key("키");   키 설정하는 방식	
	//단일.Set_Value(타입);   값 타입 설정하는 방식
	//단일.Get_Value();   값 가져오는 방식
	//단일.Get_Key();   키 가져오는 방식
	//단일.Get_Parent();   부모 가져오는 방식
	//단일.Get_Child();   자식 가져오는 방식
	//단일.Get_Next();   다음 객체 가져오는 방식
	//단일.Get_Prev();   이전 객체 가져오는 방식

	

	

}

void main() {
	//매크로 함수 파싱해주는 프로그램 입니다.
	//Excel에 Seq, Call, Result 3개의 열을 통해서 Seq와 Call을 넣고 실행파일을 실행하면 Result가 나옵니다.
	//구조
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen 읽기
	//fopen -> Data화

	testJsonModel();

	//Timer_Ctrl();

	
}