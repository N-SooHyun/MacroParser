#include "Main.h"

//JsonModel class �κ�

using namespace JSON;

void JNode::Set_Type(JType nodeType) {
	if (ptype) {
		delete ptype; // ������ �Ҵ�� �޸� ����
	}
	type = nodeType;
	switch (type) {
	case JType::OBJECT:
		ptype = new JObj();
		break;
	case JType::ARRAY:
		ptype = new JArr();
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
	case JType::INVALID:
		ptype = nullptr; // null Ÿ���� �����͸� ������� ����
		break;
	default:
		ptype = nullptr; // �⺻������ nullptr�� �ʱ�ȭ
		break;
	}
}



//JObj class �κ�
void JObj::Set_Key(const char* k) {
	key.Set_Str(k); // Ű �� ����
}

void JObj::Set_Value(JNode::JType nodeType) {
	value.Set_Type(nodeType); // �� Ÿ�� ����
}

//JArr class �κ�
void JArr::Set_Value(JNode::JType nodeType) {
	value.Set_Type(nodeType); // �迭 ����� Ÿ�� ����
}
void JArr::Set_Next(JArr* nextNode) {
	next = nextNode; // ���� �迭 ��� ����
}






