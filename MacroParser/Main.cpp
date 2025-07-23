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

//enum으로 대입 및 생성
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

//단일 타입 테스트------------------------------------------------------------------------------------------
//<Operator ()= 대입>	//무조건 덮어씌워짐
	SnglTyp = num;		//operator=(int number)
	SnglTyp = pnum;		//operator=(int* number)
	SnglTyp = dnum;		//operator=(double number)
	SnglTyp = pdnum;	//operator=(double* number)
	SnglTyp = bl;		//operator=(bool boolean)
	SnglTyp = pbl;		//operator=(bool* boolean)
	SnglTyp = str;		//operator=(const char* str)
	SnglTyp = c;		//operator=(const char c)

//<Operator() 반환>		//반환실패하면 -1이라던가 null값이 던져짐
	num = SnglTyp;		//operator int()		실패시 -1 반환
	pnum = SnglTyp;		//operator int*()		실패시 nullptr반환
	dnum = SnglTyp;		//operator double()		실패시 -1 반환
	pdnum = SnglTyp;	//operator double*()	실패시 nullptr반환
	bl = SnglTyp;		//operator bool()		실패시 false 반환
	pbl = SnglTyp;		//operator bool*()		실패시 nullptr 반환
	c = SnglTyp;		//operator char()		실패시 '\0' 반환
	str = SnglTyp;		//operator char*()		실패시 nullptr 반환


//Obj 타입 테스트-------------------------------------------------------------------------------------------
	char* key = "Key";
	SnglTyp = JNode::JType::OBJECT;
	JsonCtrl* test = new JsonCtrl();


	
//<operator() = 대입>
	//Node["key"] -> Node->Obj->Node의 값을 보고 대입을 해줌
	//SnglTyp["Key"] = num;
	//SnglTyp[key] = num;
	SnglTyp["Key"];

//<operator() 반환>
	//num = SnglTyp["Key"];
	//num = SnglTyp[key];


	return;



}

void main() {
	//매크로 함수 파싱해주는 프로그램 입니다.
	//Excel에 Seq, Call, Result 3개의 열을 통해서 Seq와 Call을 넣고 실행파일을 실행하면 Result가 나옵니다.
	//구조
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen 읽기
	//fopen -> Data화

	//testJsonModel();

	//Timer_Ctrl();

	//JsonAPI_Test();

	JsonAPI_Test2();

	//No_Timer_Dac();

}