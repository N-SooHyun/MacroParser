#include "Main.h"

//JsonModel class �κ�

using namespace JSON;

void JNode::Set_Type(JType nodeType) {
	type = nodeType;
	switch (type) {
	case JType::OBJECT:
		ptype = new JObj();
		ObjCnt = -1;
		break;
	case JType::ARRAY:
		ptype = new JArr();
		ArrCnt = -1;
		break;
	case JType::STRING:
		ptype = new Dynamic::DynamicStr(128);
		break;
	case JType::NUMBER:
		ptype = new int(); // �⺻������ int�� �ʱ�ȭ
		break;
	case JType::BOOL:
		ptype = new bool();
		break;
	case JType::DOUBLE:
		ptype = new double();
		break;
	case JType::NULL_TYPE:
		ptype = nullptr; // null Ÿ���� �����͸� ������� ����
		ObjCnt = -2;
		ArrCnt = -2;
		break;
	default:
		ptype = nullptr; // �⺻������ nullptr�� �ʱ�ȭ
		break;
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








