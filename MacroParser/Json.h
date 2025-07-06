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
			FLOAT,		//�Ҽ�
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
		friend class Proxy;
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


	//Json ó�� �κ� �ڷᱸ�� ��� API��� �����ϸ� �ɰ� �����ϴ�.
	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr), Child(nullptr) {}
		JsonCtrl(JNode::JType rootType) : root(new JNode(rootType)), Child(nullptr) {}
		~JsonCtrl() {
			if (root != nullptr) {
				delete root;
				root = nullptr; // ������ �ʱ�ȭ
			}
			if (Child != nullptr) {
				delete Child;
				Child = nullptr; // ������ �ʱ�ȭ
			}
		}

		class Proxy {
			Dynamic::DynamicStr* key; // Ű ��
			JsonCtrl& parent; // �θ� JsonCtrl ��ü ����

		public:
			Proxy(JsonCtrl& parentCtrl, const char* k) : key(new Dynamic::DynamicStr(128)), parent(parentCtrl) {
				key->Set_Str(k); // Ű �� ����
			}
			~Proxy() {
				if (key != nullptr) {
					delete key; // ���� �Ҵ�� Ű �� ����
					key = nullptr; // ������ �ʱ�ȭ
					parent.Child->key = nullptr;
				}
			}

#define JSON_OBJ JSON::JObj{}
#define JSON_ARR JSON::JArr{}
			//�ڵ� Ÿ������ �ϸ� �����ڰ� � Ÿ������ �˱Ⱑ ������ ���� ������ �����ε����� �밡�� �ؾ��ҵ�
			template<typename T>
			void operator=(const T& value) {
				JObj* obj = static_cast<JObj*>(parent.root->ptype);

				obj->Set_Key(key->Get_Str()); // Ű �� ����




			}

		};

		//����� API �Լ���
		//Push �ֱ�
		//{} ��ü ���� var["key"] = value;
		Proxy operator[](const char* key) {
			Child = new Proxy(*this, key);
			return *Child; // Proxy ��ü ��ȯ
		}

		//Pop ����


	private:

		Proxy* Child;	// ���ҽ� ���� ��ü
		JNode* root;	// Json�� ��Ʈ ���
	};
}

