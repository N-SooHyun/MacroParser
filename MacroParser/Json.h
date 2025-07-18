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

	class JNodeBase {
	public:
		virtual ~JNodeBase() {}
	};

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
		};
		JNode() : type(JType::NULL_TYPE), ptype(nullptr){}
		JNode(JType nodeType) : type(nodeType), ptype(nullptr) {
			Set_Type(nodeType); // Ÿ�� ����
		}

		void Set_Type(JType nodeType);
		void* Get_Ptype();
		template<typename T>
		JNode::JType GetJsonType() ;


		~JNode() {
			//delete ptype;
			if(type == JNode::JType::ARRAY || type == JNode::JType::OBJECT) {
				// JObj�� JArr Ÿ���� ���, ptype�� �ش� Ÿ���� ��ü�� ����Ű�Ƿ� ���� �Ҵ� ����
				delete static_cast<JNodeBase*>(ptype);
			}
			else if (type == JNode::JType::STRING) {
				// DynamicStr Ÿ���� ���, ptype�� ���ڿ��� ����Ű�Ƿ� ���� �Ҵ� ����
				delete static_cast<Dynamic::DynamicStr*>(ptype);
			}
			else if (type == JNode::JType::NUMBER || type == JNode::JType::BOOL || type == JNode::JType::DOUBLE) {
				delete ptype;
			}
			ptype = nullptr;
			type = JNode::JType::INVALID; // Ÿ���� INVALID�� �����Ͽ� �����ϰ� �ʱ�ȭ
		}

		JNode(const JNode& other) = delete; // ���� ������ ����
		JNode& operator=(const JNode& other) = delete; // ���� ���� ������ ����

	//private:
		JType type; //��� Ÿ��
		void* ptype; //��� Ÿ�Կ� ���� ������ ������

		friend class JObj;
		friend class JArr;
		friend class JsonCtrl;
		friend class JsonAPI;
	};


	class JObj : public JNodeBase {
	public: 
		JObj() : key(128) , value(JNode::JType::NULL_TYPE), next(nullptr){}
		~JObj() {
			if (next != nullptr) {
				delete next;
				next = nullptr;
			}
		}

		void Set_Key(const char* k);  // Ű �� ����
		void Set_Value(JNode::JType nodeType);// �� Ÿ�� ����
		JNode* Get_Value();

	//private:
		Dynamic::DynamicStr key; // Ű ��
		JNode value; // �� (Json_Model Ÿ������ ����)
		JObj* next; // ���� ��ü�� ����Ű�� ������ (���� ����Ʈ ���·� ���� ����)
		friend class JArr;
		friend class JsonAPI;
	};


	class JArr : public JNodeBase{
	public:
		JArr() : next(nullptr), value(JNode::JType::NULL_TYPE){}
		~JArr() {
			if (next != nullptr) {
				delete next;
				next = nullptr;
			}
		}

		void Set_Value(JNode::JType nodeType);
		void Set_Next(JArr* nextNode);

	private:
		JArr* next; // ���� �迭 ��Ҹ� ����Ű�� ������ (���� ����Ʈ ���·� ���� ����)
		JNode value; // �迭 ����� �� (Json_Model Ÿ������ ����)

		friend class JsonAPI;
	};


	//---------------------------------------------------------------------------------
	//-------------------------------Json����� ȣ��κ�-------------------------------
	//---------------------------------------------------------------------------------

	//JSON ��� ��Ģ
	// {} ��ü�϶�
	// JNode Js = "{"Key" : "Value"}"; 
	// 1. Insert �߰��ϱ�
	// Js.a = "b";  -> {"Key" : "Value", "a" : "b"}
	// Js["a"] = "b";  -> {"Key" : "Value", "a" : "b"}
	// char* var = "a";    Js[var] = "b";  -> {"Key" : "Value", "a" : "b"}

	// 2. Get �� �������� �� ã��?
	// Js.get("a");  -> "b"
	// Js["a"];  -> "b"
	// char* var = "a";    Js[var];  -> "b"

	// 3. Delete �����ϱ�
	// Js.del("a");  -> {"Key" : "Value"}
	// Js["a"].del();  -> {"Key" : "Value"}
	// char* var = "a";    Js[var].del();  -> {"Key" : "Value"}

	// 4. Clear ����
	// Js.clear();  -> {}


	// [] Arr�϶�
	// 1. Insert �߰��ϱ�
	// Js[0] = "a";  -> ["a"]
	// Js[1] = "b";  -> ["a", "b"]
	// int idx = 2;    Js[idx] = "c";  -> ["a", "b", "c"]

	// 2. Get �� �������� �� ã��?
	// Js[0];  -> "a"
	// Js[1];  -> "b"
	// int idx = 2;    Js[idx];  -> "c"

	// 3. Delete �����ϱ�
	// Js[0].del();  -> ["b", "c"]

	// 4. Clear ����
	// Js.clear();  -> []


	// ���� Ÿ���϶�
	// 1. Insert �߰��ϱ� Ȥ�� ��������?
	// Js = "Hello";  -> "Hello"
	// Js = 123;  -> 123

	// 2. Get �� ��������
	// Js.get();  -> "Hello" Ȥ�� 123
	// Js.get<int>();  -> 123

	// 3. Delete �����ϱ�
	// Js.del();  -> null Ȥ�� 0

	// 4. Clear ����
	// Js.clear();  -> null Ȥ�� 0

	//---------------------------------------------------------------------------------

	// ������ JNode�� �����̸� �̰� �������� �����ϵ���

	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr) {}
		JsonCtrl(JNode::JType rootType) {
			root = new JNode(rootType);
		}

		~JsonCtrl() {
			if (root != nullptr) {
				delete root;
				root = nullptr;
			}
		}


		//<���� ������ �����ε�>
		//enum���� ������
		void operator=(JNode::JType rootType) {
			root = new JNode(rootType);
		}

		//��ü �迭 Ÿ���϶�(JNode::JType::OBJECT, JNode::JType::ARRAY)
		//Object Ÿ���϶�
		/*void operator=(JObj& obj) {
			root->type = JNode::JType::OBJECT;
			root->ptype = static_cast<void*>(obj);
		}*/
		//�������� ������
		void operator=(JObj* obj) {
			root->type = JNode::JType::OBJECT;
			root->ptype = static_cast<void*>(obj);
		}

		//Array Ÿ���϶�
		void operator=(JArr* arr) {
			root->type = JNode::JType::ARRAY;
			root->ptype = static_cast<void*>(arr);
		}

		//���� Ÿ���϶�(int, double, string, bool ��)
		// 1. ����Ⱑ �����ϵ���
		//String Ÿ���϶�
		void operator=(const char* str) {
			root->Set_Type(JNode::JType::STRING);
			Dynamic::DynamicStr* strPtr = static_cast<Dynamic::DynamicStr*>(root->Get_Ptype());
			strPtr->Set_Str(str);
		}

		//Number Ÿ���϶�
		void operator=(int number) {
			root->Set_Type(JNode::JType::NUMBER);
			int* numPtr = static_cast<int*>(root->Get_Ptype());
			*numPtr = number;
		}

		//Boolean Ÿ���϶�
		void operator=(bool boolean) {
			root->Set_Type(JNode::JType::BOOL);
			bool* boolPtr = static_cast<bool*>(root->Get_Ptype());
			*boolPtr = boolean;
		}

		//Double Ÿ���϶�
		void operator=(double number) {
			root->Set_Type(JNode::JType::DOUBLE);
			double* doublePtr = static_cast<double*>(root->Get_Ptype());
			*doublePtr = number;
		}

		//Null Ÿ���϶�
		void operator=(std::nullptr_t) {
			root->Set_Type(JNode::JType::NULL_TYPE);
		}



	private:
		JNode* root;
	};

}

