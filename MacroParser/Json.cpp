#include "Main.h"

//JsonModel class �κ�

using namespace JSON;

void JNode::Set_Type(JType nodeType) {
	type = nodeType;
	switch (type) {
		case JType::OBJECT:
		{
			ptype = new JObj();
			ObjCnt = -1;
			break;
		}
		case JType::ARRAY:
		{
			ptype = new JArr();
			ArrCnt = -1;
			break;
		}
		case JType::CHAR:
		{
			ptype = new char();
			char *init_char = static_cast<char*>(ptype);
			init_char[0] = '0';
			break;
		}
		case JType::STRING:
		{
			ptype = new Dynamic::DynamicStr(128);
			Dynamic::DynamicStr* init_str = static_cast<Dynamic::DynamicStr*>(ptype);
			init_str->Set_Str("0\0");
			break;
		}
		case JType::NUMBER:
		{
			ptype = new int(); // �⺻������ int�� �ʱ�ȭ
			int* init_num = static_cast<int*>(ptype);
			*init_num = 0;
			break;
		}
		case JType::BOOL:
		{
			ptype = new bool();
			bool* init_bool = static_cast<bool*>(ptype);
			*init_bool = false;
			break;
		}
		case JType::DOUBLE:
		{
			ptype = new double();
			double* init_dnum = static_cast<double*>(ptype);
			*init_dnum = 0.0;
			break;
		}
		case JType::NULL_TYPE:
		{
			ptype = nullptr; // null Ÿ���� �����͸� ������� ����
			ObjCnt = -2;
			ArrCnt = -2;
			break;
		}
		default:
		{
			ptype = nullptr; // �⺻������ nullptr�� �ʱ�ȭ
			break;
		}
	}
}

void* JNode::Get_Ptype() {
	return ptype;
}


//JObj class �κ�
void JObj::Set_Key(const char* k) {
	key.Set_Str(k); // Ű �� ����
}

void JObj::Set_Value(JNode::JType nodeType) {
	value.Set_Type(nodeType); // �� Ÿ�� ����
}


JNode* JObj::Get_Value() {
	return &value;
}

//JArr class �κ�
void JArr::Set_Value(JNode::JType nodeType) {
	value.Set_Type(nodeType); // �迭 ����� Ÿ�� ����
}
void JArr::Set_Next(JArr* nextNode) {
	next = nextNode; // ���� �迭 ��� ����
}








