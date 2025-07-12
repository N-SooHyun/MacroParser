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
	//���� "{"A" : "B"}" frstJson
	JSON::JNode frstJson(JNode::JType::OBJECT);
	JObj* obj = static_cast<JObj*>(frstJson.Get_Type());

	JsonCtrl NullCtrl;	//Nulltype
	JsonCtrl ObjCtrl(JNode::JType::OBJECT);	//Object type
	JsonCtrl ArrCtrl(JNode::JType::ARRAY);	//Array type
	JsonCtrl NumCtrl(JNode::JType::NUMBER);	//Number type
	JsonCtrl StrCtrl(JNode::JType::STRING);	//String type
	JsonCtrl BolCtrl(JNode::JType::BOOL);	//Boolean type
	JsonCtrl DblCtrl(JNode::JType::DOUBLE);	//Double type


	//Json�� ���� Ÿ���϶� ���� �־��ִ� �޼ҵ� ȣ���
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
	




	//��ü["Ű"] = ��;   Obj�϶� �ִ� ���1
	//��ü[var] = ��;   Obj�϶� �ִ� ���2 

	//�迭[�ε���] = ��;   Arr�϶� �ִ� ���1
	//�迭.Push(��);   Arr�϶� �ִ� ���2

	//����.Get_Value();   �� �������� ���
	//����.Get_Type();   Ÿ�� �������� ���
	//����.Get_Str();   ���ڿ� �������� ���
	//����.Get_Int();   ���� �������� ���
	//����.Get_Double();   �Ǽ� �������� ���
	//����.Get_Bool();   �Ҹ��� �������� ���
	//����.Set_Str("���ڿ�");   ���ڿ� �����ϴ� ���
	//����.Set_Int(����);   ���� �����ϴ� ���
	//����.Set_Double(�Ǽ�);   �Ǽ� �����ϴ� ���
	//����.Set_Bool(�Ҹ���);   �Ҹ��� �����ϴ� ���
	//����.Set_Type(Ÿ��);   Ÿ�� �����ϴ� ���
	//����.Set_Key("Ű");   Ű �����ϴ� ���	
	//����.Set_Value(Ÿ��);   �� Ÿ�� �����ϴ� ���
	//����.Get_Value();   �� �������� ���
	//����.Get_Key();   Ű �������� ���
	//����.Get_Parent();   �θ� �������� ���
	//����.Get_Child();   �ڽ� �������� ���
	//����.Get_Next();   ���� ��ü �������� ���
	//����.Get_Prev();   ���� ��ü �������� ���

	

	

}

void main() {
	//��ũ�� �Լ� �Ľ����ִ� ���α׷� �Դϴ�.
	//Excel�� Seq, Call, Result 3���� ���� ���ؼ� Seq�� Call�� �ְ� ���������� �����ϸ� Result�� ���ɴϴ�.
	//����
	//Excel -(VBA)-> .json( {"Excel" : [{"Seq(1)" : "Call"},{...},{...}] } ) -> fopen �б�
	//fopen -> Dataȭ

	testJsonModel();

	//Timer_Ctrl();

	
}