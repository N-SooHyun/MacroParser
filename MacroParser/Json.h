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
		};
		JNode() : type(JType::NULL_TYPE), ptype(nullptr){}
		JNode(JType nodeType) : type(nodeType), ptype(nullptr) {
			Set_Type(nodeType); // Ÿ�� ����
		}

		void Set_Type(JType nodeType);
		void* Get_Type();
		template<typename T>
		JNode::JType GetJsonType() ;


		~JNode() {
			delete ptype; // ���� �Ҵ�� �޸� ����
			ptype = nullptr; // ������ �ʱ�ȭ
			type = JType::NULL_TYPE; // Ÿ���� DELETE�� ����
		}

	private:
		JType type; //��� Ÿ��
		void* ptype; //��� Ÿ�Կ� ���� ������ ������

		friend class JObj;
		friend class JArr;
		friend class JsonCtrl;
	};


	class JObj {
	public: 
		JObj() : key(128) , value(JNode::JType::NULL_TYPE), next(nullptr){}
		~JObj() {
			delete next; // ���� ��ü�� ����Ű�� ������ ���� 
		}

		void Set_Key(const char* k);  // Ű �� ����
		void Set_Value(JNode::JType nodeType);// �� Ÿ�� ����
		JNode Get_Value();

	private:
		Dynamic::DynamicStr key; // Ű ��
		JNode value; // �� (Json_Model Ÿ������ ����)
		JObj* next; // ���� ��ü�� ����Ű�� ������ (���� ����Ʈ ���·� ���� ����)
		friend class JArr;
		friend class JsonCtrl;
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

	//���������� ó���� JsonCtrlŬ����

#define OBJ JSON::JObj()		//{}�� �ǹ�
#define ARR	JSON::JArr()		//[]�� �ǹ�

	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr), Obj(nullptr), Arr(nullptr){}
		JsonCtrl(JNode::JType JsonType) : root(nullptr), Obj(nullptr), Arr(nullptr){
			root = new JNode(JsonType); // Json�� ��Ʈ ��� ���� Type, ptype �ʱ�ȭ
		}
		
		//3�� ���� �Ҹ�
		~JsonCtrl() {
			if (root != nullptr) {
				delete root;
				root = nullptr;
			}
			if(Obj != nullptr) {
				delete Obj;
				Obj = nullptr;
			}
			if(Arr != nullptr) {
				delete Arr;
				Arr = nullptr;
			}
		}

		//root ����ʱ�ȭ
		void operator = (const JNode::JType& JsonType) {
			if (root != nullptr) {
				std::cout << "JsonCtrl is Already Initialized." << std::endl;
				return;
			}

			root = new JNode(JsonType); // Json�� ��Ʈ ��� ���� Type, ptype �ʱ�ȭ
		}

		//����ó�� �κ�
		bool Exception_Root_Null() {
			if (root == nullptr) {
				std::cout << "JsonCtrl is Not Initialized." << std::endl;
				return true;
			}
			return false;
		}
		bool Exception_Root_Type_Null() {
			if (root->ptype == nullptr) {
				std::cout << "JsonCtrl is Not Initialized." << std::endl;
				return true;
			}
			return false;
		}

		//Node�� Object Ÿ���� ���



		//Node Arr Ÿ���� ���



		//Node ���� Ÿ���� ���	(int, double, bool, string)
		//Ÿ���� �ٸ��� ����� ������ Set_Value
		void Set_Value(const char* value) {
			//���ڿ� ���ͷ��� ���� ���
			Set_Value<const char*>(value);
		}
		template<typename T>
		void Set_Value(const T& value) {
			if (Exception_Root_Null() || Exception_Root_Type_Null()) return;

			if (root->type != root->GetJsonType<T>()) {
				std::cout << "�ش� Ÿ������ ���� ������ �� �����ϴ�." << std::endl;
				return;
			}

			if (root->type == JNode::JType::OBJECT || root->type == JNode::JType::ARRAY) {
				std::cout << "����Ÿ�Ը� ���� ������ �� �ֽ��ϴ�." << std::endl;
				return;
			}

			if (root->ptype) {
				try {
					switch (root->type) {
					case JNode::JType::NUMBER:
						if constexpr (std::is_convertible_v<T, int>) {
							int* num = static_cast<int*>(root->ptype);
							*num = static_cast<int>(value); // T�� int�� ��ȯ
							std::cout << "Set_Value: " << *num << std::endl;
						}
						else throw std::runtime_error("Ÿ�� ��ȯ ����: T�� int�� ��ȯ�� �� �����ϴ�.");
						break;

					case JNode::JType::STRING:
						if constexpr (std::is_convertible_v<T, const char*>) {
							Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(root->ptype);
							str->Set_Str(static_cast<const char*>(value)); // T�� const char*�� ��ȯ
							std::cout << "Set_Value: " << str->Get_Str() << std::endl;
						}
						else throw std::runtime_error("Ÿ�� ��ȯ ����: T�� const char*�� ��ȯ�� �� �����ϴ�.");
						break;

					case JNode::JType::BOOL:
						if constexpr (std::is_convertible_v<T, bool>) {
							bool* bol = static_cast<bool*>(root->ptype);
							*bol = static_cast<bool>(value); // T�� bool�� ��ȯ
							std::cout << "Set_Value: " << *bol << std::endl;
						}
						else throw std::runtime_error("Ÿ�� ��ȯ ����: T�� bool�� ��ȯ�� �� �����ϴ�.");
						break;

					case JNode::JType::DOUBLE:
						if constexpr (std::is_convertible_v<T, double>) {
							double* dbl = static_cast<double*>(root->ptype);
							*dbl = static_cast<double>(value); // T�� double�� ��ȯ
							std::cout << "Set_Value: " << *dbl << std::endl;
						}
						else throw std::runtime_error("Ÿ�� ��ȯ ����: T�� double�� ��ȯ�� �� �����ϴ�.");
						break;

					default:

						break;
					}
				}catch(const std::exception& e) {
					std::cout << "���� �߻�: " << e.what() << std::endl;
					return;
				}
				

				


				
			}

		}

		//���� Ÿ�� ���� 
		template<typename T>
		void Force_Set_Value(const T& value) {
			if (Exception_Root_Null() || Exception_Root_Type_Null()) return;

			if (root->type == JNode::JType::OBJECT || root->type == JNode::JType::ARRAY) {
				std::cout << "����Ÿ�Ը� ���� ������ �� �ֽ��ϴ�." << std::endl;
				return;
			}

			if(root->type != root->GetJsonType<T>()) {
				if (root->GetJsonType<T>() == JNode::JType::OBJECT || root->GetJsonType<T>() == JNode::JType::ARRAY) {
					std::cout << "�ش� Ÿ������ ���� ������ �� �����ϴ�. ��ü�� �迭 Ÿ���� ���� ������ �� �����ϴ�." << std::endl;
					return;
				}

				//Ÿ���� �ٸ��� ����� �����ϵ��� �������մϴ�.
				std::cout << "�ش� Ÿ������ ���� ������ �� �����ϴ�. ���� �����մϴ�." << std::endl;
				root->Set_Type(root->GetJsonType<T>()); // ���ο� Ÿ������ ����
			}

			if (root->ptype) {
				try {
					switch (root->type) {
					case JNode::JType::NUMBER:
						if constexpr (std::is_convertible_v<T, int>) {
							int* num = static_cast<int*>(root->ptype);
							*num = static_cast<int>(value); // T�� int�� ��ȯ
							std::cout << "Set_Value: " << *num << std::endl;
						}
						else throw std::runtime_error("Ÿ�� ��ȯ ����: T�� int�� ��ȯ�� �� �����ϴ�.");
						break;

					case JNode::JType::STRING:
						if constexpr (std::is_convertible_v<T, const char*>) {
							Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(root->ptype);
							str->Set_Str(static_cast<const char*>(value)); // T�� const char*�� ��ȯ
							std::cout << "Set_Value: " << str->Get_Str() << std::endl;
						}
						else throw std::runtime_error("Ÿ�� ��ȯ ����: T�� const char*�� ��ȯ�� �� �����ϴ�.");
						break;

					case JNode::JType::BOOL:
						if constexpr (std::is_convertible_v<T, bool>) {
							bool* bol = static_cast<bool*>(root->ptype);
							*bol = static_cast<bool>(value); // T�� bool�� ��ȯ
							std::cout << "Set_Value: " << *bol << std::endl;
						}
						else throw std::runtime_error("Ÿ�� ��ȯ ����: T�� bool�� ��ȯ�� �� �����ϴ�.");
						break;

					case JNode::JType::DOUBLE:
						if constexpr (std::is_convertible_v<T, double>) {
							double* dbl = static_cast<double*>(root->ptype);
							*dbl = static_cast<double>(value); // T�� double�� ��ȯ
							std::cout << "Set_Value: " << *dbl << std::endl;
						}
						else throw std::runtime_error("Ÿ�� ��ȯ ����: T�� double�� ��ȯ�� �� �����ϴ�.");
						break;

					default:

						break;
					}
				}
				catch (const std::exception& e) {
					std::cout << "���� �߻�: " << e.what() << std::endl;
					return;
				}






			}

		}

		
		//���� Ÿ���� ���� �������� �޼ҵ�
		template<typename T>
		T Get_Value() {
			if (Exception_Root_Null() || Exception_Root_Type_Null()) {
				throw std::runtime_error("��� �ִ� ���");
			}

			auto expected = root->GetJsonType<T>();
			if (root->type != expected) {
				std::cerr << "[ERROR] Ÿ�� ����ġ. root->type = " << (int)root->type
					<< ", ��û�� Ÿ�� = " << (int)expected << std::endl;
				throw std::runtime_error("Ÿ�� ����ġ");
			}
			
			return *static_cast<T*>(root->ptype);
		}

	private:
		JNode* root; // Json�� ��Ʈ ���
		JObj* Obj; // Json ��ü
		JArr* Arr; // Json �迭

	};

/*
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
		*static_cast<para_type*>(obj->value.ptype) = value; \
		parent.Obj = parent.Obj->next;\
	}

#define OPER_DEFAULT

	//Json ó�� �κ� �ڷᱸ�� ��� API��� �����ϸ� �ɰ� �����ϴ�.
	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr), Child(nullptr), Obj(nullptr) {}
		JsonCtrl(JNode::JType rootType) : root(new JNode(rootType)), Child(nullptr), Obj(nullptr) {
			if (rootType == JNode::JType::OBJECT)
				Obj = static_cast<JObj*>(root->ptype);
		}
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

				JObj* Excep_Key = static_cast<JObj*>(parent.root->ptype);

				for (; ;) {
					if (Excep_Key == nullptr) break; //�̹� �����ϴ� Ű�� ������ ����

					if (Excep_Key->key.Get_Str() == k) {
						// Key������ Value�� �̹� �����ϴ� ��� �� �����
						key = &(Excep_Key->key);
						return;
					}
					Excep_Key = Excep_Key->next; // ���� ��ü�� �̵�
				}

				key = new Dynamic::DynamicStr(128); // Ű �� ���� �Ҵ�
				key->Set_Str(k); // Ű �� ����
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
			OPER_ASSIGN(JSON::JObj, JNode::JType::OBJECT)
			//2. Arr Ÿ��
			OPER_ASSIGN(JSON::JArr, JNode::JType::ARRAY)
			//3. Str Ÿ��
			void operator=(const char* value) {
				if (key == nullptr) {
					cout << "JsonCtrl is Not Obj." << endl; return;
				} 
				JObj* obj = static_cast<JObj*>(parent.root->ptype); 
				obj->Set_Key(key->Get_Str()); 
				obj->Set_Value(JNode::JType::STRING);
				Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(obj->value.ptype);
				str->Set_Str(value);
				parent.Obj = parent.Obj->next; // ���� ��ü�� �̵�
			}
			//4. Num Ÿ��
			OPER_ASSIGN(int, JNode::JType::NUMBER)
			//5. Bol Ÿ��
			OPER_ASSIGN(bool, JNode::JType::BOOL)
			//6. Double Ÿ��
			OPER_ASSIGN(double, JNode::JType::DOUBLE)
			//7. NULL Ÿ��
			void operator=(const void* value) {
				JObj* Excep_Key = static_cast<JObj*>(parent.root->ptype);

				for (; ;) {
					if (Excep_Key == nullptr) break; //�̹� �����ϴ� Ű�� ������ ����

					if (Excep_Key->key.Get_Str() == value) {
						cout << "The Key Already Exists" << endl;
						return; // �̹� �����ϴ� Ű�� ��� ���� ó��
					}
					Excep_Key = Excep_Key->next; // ���� ��ü�� �̵�
				}

				if (key == nullptr) {
					cout << "JsonCtrl is Not Obj." << endl; return;
				} JObj* obj = static_cast<JObj*>(parent.root->ptype); 
				obj->Set_Key(key->Get_Str()); 
				obj->Set_Value(JNode::JType::NULL_TYPE);
				parent.Obj = parent.Obj->next; // ���� ��ü�� �̵�
			}

			operator int() {
				if (key == nullptr) {
					cout << "JsonCtrl is Not Obj." << endl;
					return 0;
				}
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


			if (root->type == JNode::JType::OBJECT) {
				Child = new Obj_Ctrl(*this, key);
				return *Child; // Obj_Ctrl ��ü ��ȯ
			}
			else {
				//�߸��� Ÿ���� ��� ���� ó��
				static Obj_Ctrl dummy(*this, "");
				return dummy;
			}


		}

		//Pop ����
		

	private:
		JObj* Obj;			// Json ��ü
		Obj_Ctrl* Child;	// ���ҽ� ���� ��ü
		JNode* root;		// Json�� ��Ʈ ���
	};

	*/
}

