#pragma once

namespace JSON {
	//.Json ������ ������ ���� 2��
	// ������Ÿ�� - {}, []
	// ������Ÿ�� - "str", true, 1, 123.2, null
	//1. Key�� ������ Stirng Ÿ��
	//2. Value : Number (int, float), String, Boolean (true, false), Array ([]), Object ({}), null
	
	//---------------------------------------------------------------------------------
	//-------------------------------Json�ڷᱸ�� �κ�---------------------------------
	//---------------------------------------------------------------------------------

	class JNode {
	public: 
		enum class JType {
			//������ Ÿ��
			OBJECT,		//{}
			ARRAY,		//[]
			//������ Ÿ��
			STRING,		//���ڿ�
			NUMBER,		//����
			BOOL,		//true false
			NULL_TYPE,	//null
			DOUBLE,		//�Ҽ�
			INVALID,	//�߸��� Ÿ��
			DELETE		//������ Ÿ��(�߰����� �뵵�� ��� ����)
		};
		JNode() : type(JType::NULL_TYPE), ptype(nullptr){}
		JNode(JType nodeType) : type(nodeType), ptype(nullptr) {
			Set_Type(nodeType); // Ÿ�� ����
		}

		void Set_Type(JType nodeType);

		~JNode() {
			delete ptype; // ���� �Ҵ�� �޸� ����
			ptype = nullptr; // ������ �ʱ�ȭ
			type = JType::DELETE; // Ÿ���� DELETE�� ����
		}

	private:
		JType type; //��� Ÿ��
		void* ptype; //��� Ÿ�Կ� ���� ������ ������

		friend class JObj;
		friend class JArr;
		friend class JsonCtrl;
		friend class Obj_Ctrl;
	};


	class JObj {
	public: 
		JObj() : key(128) , value(JNode::JType::NULL_TYPE), next(nullptr){}
		~JObj() {
			delete next; // ���� ��ü�� ����Ű�� ������ ���� 
		}

		void Set_Key(const char* k);  // Ű �� ����
		void Set_Value(JNode::JType nodeType);// �� Ÿ�� ����

	private:
		Dynamic::DynamicStr key; // Ű ��
		JNode value; // �� (Json_Model Ÿ������ ����)
		JObj* next; // ���� ��ü�� ����Ű�� ������ (���� ����Ʈ ���·� ���� ����)
	};


	class JArr {
	public:
		JArr() : next(nullptr), value(JNode::JType::NULL_TYPE){}
		~JArr() {
			delete next; // ���� �迭 ��Ҹ� ����Ű�� ������ ����
		}

		void Set_Value(JNode::JType nodeType);
		void Set_Next(JArr* nextNode);

	private:
		JArr* next; // ���� �迭 ��Ҹ� ����Ű�� ������ (���� ����Ʈ ���·� ���� ����)
		JNode value; // �迭 ����� �� (Json_Model Ÿ������ ����)
	};

	//---------------------------------------------------------------------------------
	//-------------------------------Json����� ȣ��κ�-------------------------------
	//---------------------------------------------------------------------------------


	using namespace std;
	using namespace Dynamic;
	using namespace JSON;

#define JSON_OBJ JSON::JObj()
#define JSON_ARR JSON::JArr()
#define OPER_ASSIGN(para_type, Jenum_type)\
void operator=(const para_type& value){\
		if(key == nullptr) {\
			cout << "JsonCtrl is Not Obj." << endl;\
			return;\
		}\
		JObj* obj = static_cast<JObj*>(parent.root->ptype);\
		obj->Set_Key(key->Get_Str()); \
		obj->Set_Value(Jenum_type); \
	}

	//Json ó�� �κ� �ڷᱸ�� ��� API��� �����ϸ� �ɰ� �����ϴ�.
	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr), Child(nullptr), Obj(nullptr) {}
		JsonCtrl(JNode::JType rootType) : root(new JNode(rootType)), Child(nullptr), Obj(nullptr) {}
		~JsonCtrl() {
			if (root != nullptr) {
				delete root;
				root = nullptr; // ������ �ʱ�ȭ
			}
			if (Child != nullptr) {
				delete Child;
				Child = nullptr; // ������ �ʱ�ȭ
			}
			if (Obj != nullptr) {
				delete Obj;
				Obj = nullptr; // ������ �ʱ�ȭ
			}
		}

		class Obj_Ctrl {		//������ ��ü Ÿ���϶��� ���Ǵ� Push�� Ŭ������� ���� �˴ϴ�.
			Dynamic::DynamicStr* key; // Ű ��
			JsonCtrl& parent; // �θ� JsonCtrl ��ü ����

		public:
			Obj_Ctrl(JsonCtrl& parentCtrl, const char* k) : parent(parentCtrl) {
				if (k == "" || k == "arr") {
					cout << "This Node Not Obj Type" << endl;
					key = nullptr;
				}
				else {
					key = new Dynamic::DynamicStr(128); // Ű �� ���� �Ҵ�
					key->Set_Str(k); // Ű �� ����
				}
			}
			~Obj_Ctrl() {
				if (key != nullptr) {
					delete key; // ���� �Ҵ�� Ű �� ����
					key = nullptr; // ������ �ʱ�ȭ
					parent.Child->key = nullptr;
				}
			}

			//�ڵ� Ÿ������ �ϸ� �����ڰ� � Ÿ������ �˱Ⱑ ������ ���� ������ �����ε����� �밡�� �ؾ��ҵ�
			//1. Obj Ÿ��
			OPER_ASSIGN(const JSON::JObj, JNode::JType::OBJECT);
			//2. Arr Ÿ��
			OPER_ASSIGN(const JSON::JArr, JNode::JType::ARRAY);
			//3. Str Ÿ��
			OPER_ASSIGN(const char*, JNode::JType::STRING); 
			//4. Num Ÿ��
			OPER_ASSIGN(int, JNode::JType::NUMBER);
			//5. Bol Ÿ��
			OPER_ASSIGN(bool, JNode::JType::BOOL);
			//6. Double Ÿ��
			OPER_ASSIGN(double, JNode::JType::DOUBLE);
			//7. NULL Ÿ��
			OPER_ASSIGN(void*, JNode::JType::NULL_TYPE);

			operator int() {
				printf("Test");
				return 0;
			}

			operator double() {
				printf("Test2");
				return 0.0;
			}
			
		};

		//����� API �Լ���
		//Push �ֱ�
		//{} ��ü ���� var["key"] = value;
		Obj_Ctrl& operator[](const char* key) {
			// JsonCtrl ��ü�� �ʱ�ȭ���� ���� ��� Ȥ�� ��ü�� �ƴ� ���
			if (root == nullptr) {
				//nullptr �� ��� ���� ó��
				static Obj_Ctrl dummy(*this, "");
				return dummy;
			}
			else if (root->type == JNode::JType::ARRAY) {
				//�迭 Ÿ���� ��� �Ѱ��ֱ�
				static Obj_Ctrl dummy(*this, "arr");
				return dummy;
			}
			Child = new Obj_Ctrl(*this, key);
			return *Child; // Obj_Ctrl ��ü ��ȯ
		}

		//Pop ����
		

	private:
		JObj* Obj;		// Json ��ü
		Obj_Ctrl* Child;	// ���ҽ� ���� ��ü
		JNode* root;	// Json�� ��Ʈ ���
	};
}

