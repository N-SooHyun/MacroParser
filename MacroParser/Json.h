#pragma once

namespace JSON {
	//.Json 파일이 가능한 조건 2개
	// 구조적타입 - {}, []
	// 원시적타입 - "str", true, 1, 123.2, null
	//1. Key값 무조건 Stirng 타입
	//2. Value : Number (int, float), String, Boolean (true, false), Array ([]), Object ({}), null
	
	//---------------------------------------------------------------------------------
	//-------------------------------Json자료구조 부분---------------------------------
	//---------------------------------------------------------------------------------

	class JNode {
	public: 
		enum class JType {
			//구조적 타입
			OBJECT,		//{}
			ARRAY,		//[]
			//원시적 타입
			STRING,		//문자열
			NUMBER,		//숫자
			BOOL,		//true false
			NULL_TYPE,	//null
			DOUBLE,		//소수
			INVALID,	//잘못된 타입
		};
		JNode() : type(JType::NULL_TYPE), ptype(nullptr){}
		JNode(JType nodeType) : type(nodeType), ptype(nullptr) {
			Set_Type(nodeType); // 타입 설정
		}

		void Set_Type(JType nodeType);
		void* Get_Type();
		template<typename T>
		JNode::JType GetJsonType() ;


		~JNode() {
			delete ptype; // 동적 할당된 메모리 해제
			ptype = nullptr; // 포인터 초기화
			type = JType::NULL_TYPE; // 타입을 DELETE로 설정
		}

	private:
		JType type; //노드 타입
		void* ptype; //노드 타입에 따른 데이터 포인터

		friend class JObj;
		friend class JArr;
		friend class JsonCtrl;
	};


	class JObj {
	public: 
		JObj() : key(128) , value(JNode::JType::NULL_TYPE), next(nullptr){}
		~JObj() {
			delete next; // 다음 객체를 가리키는 포인터 해제 
		}

		void Set_Key(const char* k);  // 키 값 설정
		void Set_Value(JNode::JType nodeType);// 값 타입 설정
		JNode Get_Value();

	private:
		Dynamic::DynamicStr key; // 키 값
		JNode value; // 값 (Json_Model 타입으로 정의)
		JObj* next; // 다음 객체를 가리키는 포인터 (연결 리스트 형태로 구현 가능)
		friend class JArr;
		friend class JsonCtrl;
	};


	class JArr {
	public:
		JArr() : next(nullptr), value(JNode::JType::NULL_TYPE){}
		~JArr() {
			delete next; // 다음 배열 요소를 가리키는 포인터 해제
		}

		void Set_Value(JNode::JType nodeType);
		void Set_Next(JArr* nextNode);

	private:
		JArr* next; // 다음 배열 요소를 가리키는 포인터 (연결 리스트 형태로 구현 가능)
		JNode value; // 배열 요소의 값 (Json_Model 타입으로 정의)
	};


	//---------------------------------------------------------------------------------
	//-------------------------------Json사용자 호출부분-------------------------------
	//---------------------------------------------------------------------------------

	//관리자이자 처리자 JsonCtrl클래스

#define OBJ JSON::JObj()		//{}를 의미
#define ARR	JSON::JArr()		//[]를 의미

	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr), Obj(nullptr), Arr(nullptr){}
		JsonCtrl(JNode::JType JsonType) : root(nullptr), Obj(nullptr), Arr(nullptr){
			root = new JNode(JsonType); // Json의 루트 노드 생성 Type, ptype 초기화
		}
		
		//3개 변수 소멸
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

		//root 노드초기화
		void operator = (const JNode::JType& JsonType) {
			if (root != nullptr) {
				std::cout << "JsonCtrl is Already Initialized." << std::endl;
				return;
			}

			root = new JNode(JsonType); // Json의 루트 노드 생성 Type, ptype 초기화
		}

		//예외처리 부분
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

		//Node가 Object 타입인 경우



		//Node Arr 타입인 경우



		//Node 단일 타입인 경우	(int, double, bool, string)
		//타입이 다르면 덮어쓰기 금지인 Set_Value
		void Set_Value(const char* value) {
			//문자열 리터럴로 받을 경우
			Set_Value<const char*>(value);
		}
		template<typename T>
		void Set_Value(const T& value) {
			if (Exception_Root_Null() || Exception_Root_Type_Null()) return;

			if (root->type != root->GetJsonType<T>()) {
				std::cout << "해당 타입으로 값을 설정할 수 없습니다." << std::endl;
				return;
			}

			if (root->type == JNode::JType::OBJECT || root->type == JNode::JType::ARRAY) {
				std::cout << "단일타입만 값을 설정할 수 있습니다." << std::endl;
				return;
			}

			if (root->ptype) {
				try {
					switch (root->type) {
					case JNode::JType::NUMBER:
						if constexpr (std::is_convertible_v<T, int>) {
							int* num = static_cast<int*>(root->ptype);
							*num = static_cast<int>(value); // T를 int로 변환
							std::cout << "Set_Value: " << *num << std::endl;
						}
						else throw std::runtime_error("타입 변환 오류: T는 int로 변환할 수 없습니다.");
						break;

					case JNode::JType::STRING:
						if constexpr (std::is_convertible_v<T, const char*>) {
							Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(root->ptype);
							str->Set_Str(static_cast<const char*>(value)); // T를 const char*로 변환
							std::cout << "Set_Value: " << str->Get_Str() << std::endl;
						}
						else throw std::runtime_error("타입 변환 오류: T는 const char*로 변환할 수 없습니다.");
						break;

					case JNode::JType::BOOL:
						if constexpr (std::is_convertible_v<T, bool>) {
							bool* bol = static_cast<bool*>(root->ptype);
							*bol = static_cast<bool>(value); // T를 bool로 변환
							std::cout << "Set_Value: " << *bol << std::endl;
						}
						else throw std::runtime_error("타입 변환 오류: T는 bool로 변환할 수 없습니다.");
						break;

					case JNode::JType::DOUBLE:
						if constexpr (std::is_convertible_v<T, double>) {
							double* dbl = static_cast<double*>(root->ptype);
							*dbl = static_cast<double>(value); // T를 double로 변환
							std::cout << "Set_Value: " << *dbl << std::endl;
						}
						else throw std::runtime_error("타입 변환 오류: T는 double로 변환할 수 없습니다.");
						break;

					default:

						break;
					}
				}catch(const std::exception& e) {
					std::cout << "예외 발생: " << e.what() << std::endl;
					return;
				}
				

				


				
			}

		}

		//강제 타입 변경 
		template<typename T>
		void Force_Set_Value(const T& value) {
			if (Exception_Root_Null() || Exception_Root_Type_Null()) return;

			if (root->type == JNode::JType::OBJECT || root->type == JNode::JType::ARRAY) {
				std::cout << "단일타입만 값을 설정할 수 있습니다." << std::endl;
				return;
			}

			if(root->type != root->GetJsonType<T>()) {
				if (root->GetJsonType<T>() == JNode::JType::OBJECT || root->GetJsonType<T>() == JNode::JType::ARRAY) {
					std::cout << "해당 타입으로 값을 설정할 수 없습니다. 객체나 배열 타입은 강제 설정할 수 없습니다." << std::endl;
					return;
				}

				//타입이 다르면 덮어쓰기 가능하도록 만들어야합니다.
				std::cout << "해당 타입으로 값을 설정할 수 없습니다. 강제 설정합니다." << std::endl;
				root->Set_Type(root->GetJsonType<T>()); // 새로운 타입으로 설정
			}

			if (root->ptype) {
				try {
					switch (root->type) {
					case JNode::JType::NUMBER:
						if constexpr (std::is_convertible_v<T, int>) {
							int* num = static_cast<int*>(root->ptype);
							*num = static_cast<int>(value); // T를 int로 변환
							std::cout << "Set_Value: " << *num << std::endl;
						}
						else throw std::runtime_error("타입 변환 오류: T는 int로 변환할 수 없습니다.");
						break;

					case JNode::JType::STRING:
						if constexpr (std::is_convertible_v<T, const char*>) {
							Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(root->ptype);
							str->Set_Str(static_cast<const char*>(value)); // T를 const char*로 변환
							std::cout << "Set_Value: " << str->Get_Str() << std::endl;
						}
						else throw std::runtime_error("타입 변환 오류: T는 const char*로 변환할 수 없습니다.");
						break;

					case JNode::JType::BOOL:
						if constexpr (std::is_convertible_v<T, bool>) {
							bool* bol = static_cast<bool*>(root->ptype);
							*bol = static_cast<bool>(value); // T를 bool로 변환
							std::cout << "Set_Value: " << *bol << std::endl;
						}
						else throw std::runtime_error("타입 변환 오류: T는 bool로 변환할 수 없습니다.");
						break;

					case JNode::JType::DOUBLE:
						if constexpr (std::is_convertible_v<T, double>) {
							double* dbl = static_cast<double*>(root->ptype);
							*dbl = static_cast<double>(value); // T를 double로 변환
							std::cout << "Set_Value: " << *dbl << std::endl;
						}
						else throw std::runtime_error("타입 변환 오류: T는 double로 변환할 수 없습니다.");
						break;

					default:

						break;
					}
				}
				catch (const std::exception& e) {
					std::cout << "예외 발생: " << e.what() << std::endl;
					return;
				}






			}

		}

		
		//단일 타입의 값을 가져오는 메소드
		template<typename T>
		T Get_Value() {
			if (Exception_Root_Null() || Exception_Root_Type_Null()) {
				throw std::runtime_error("비어 있는 노드");
			}

			auto expected = root->GetJsonType<T>();
			if (root->type != expected) {
				std::cerr << "[ERROR] 타입 불일치. root->type = " << (int)root->type
					<< ", 요청된 타입 = " << (int)expected << std::endl;
				throw std::runtime_error("타입 불일치");
			}
			
			return *static_cast<T*>(root->ptype);
		}

	private:
		JNode* root; // Json의 루트 노드
		JObj* Obj; // Json 객체
		JArr* Arr; // Json 배열

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

	//Json 처리 부분 자료구조 사용 API라고 생각하면 될거 같습니다.
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
				root = nullptr; // 포인터 초기화
			}
			if (Child != nullptr) {
				delete Child;
				Child = nullptr; // 포인터 초기화
			}
			if (Obj != nullptr) {
				delete Obj;
				Obj = nullptr; // 포인터 초기화
			}
		}

		class Obj_Ctrl {		//무조건 객체 타입일때만 사용되는 Push용 클래스라고 보면 됩니다.
			Dynamic::DynamicStr* key; // 키 값
			JsonCtrl& parent; // 부모 JsonCtrl 객체 참조

		public:
			Obj_Ctrl(JsonCtrl& parentCtrl, const char* k) : parent(parentCtrl) {
				if (k == "" || k == "arr") {
					cout << "This Node Not Obj Type" << endl;
					key = nullptr;
				}

				JObj* Excep_Key = static_cast<JObj*>(parent.root->ptype);

				for (; ;) {
					if (Excep_Key == nullptr) break; //이미 존재하는 키가 없으면 종료

					if (Excep_Key->key.Get_Str() == k) {
						// Key에대한 Value가 이미 존재하는 경우 값 덮어쓰기
						key = &(Excep_Key->key);
						return;
					}
					Excep_Key = Excep_Key->next; // 다음 객체로 이동
				}

				key = new Dynamic::DynamicStr(128); // 키 값 동적 할당
				key->Set_Str(k); // 키 값 설정
			}
			~Obj_Ctrl() {
				if (key != nullptr) {
					delete key; // 동적 할당된 키 값 해제
					key = nullptr; // 포인터 초기화
					parent.Child->key = nullptr;
				}
			}

			//자동 타입으로 하면 개발자가 어떤 타입인지 알기가 어려우니 여러 연산자 오버로딩으로 노가다 해야할듯
			
			//1. Obj 타입
			OPER_ASSIGN(JSON::JObj, JNode::JType::OBJECT)
			//2. Arr 타입
			OPER_ASSIGN(JSON::JArr, JNode::JType::ARRAY)
			//3. Str 타입
			void operator=(const char* value) {
				if (key == nullptr) {
					cout << "JsonCtrl is Not Obj." << endl; return;
				} 
				JObj* obj = static_cast<JObj*>(parent.root->ptype); 
				obj->Set_Key(key->Get_Str()); 
				obj->Set_Value(JNode::JType::STRING);
				Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(obj->value.ptype);
				str->Set_Str(value);
				parent.Obj = parent.Obj->next; // 다음 객체로 이동
			}
			//4. Num 타입
			OPER_ASSIGN(int, JNode::JType::NUMBER)
			//5. Bol 타입
			OPER_ASSIGN(bool, JNode::JType::BOOL)
			//6. Double 타입
			OPER_ASSIGN(double, JNode::JType::DOUBLE)
			//7. NULL 타입
			void operator=(const void* value) {
				JObj* Excep_Key = static_cast<JObj*>(parent.root->ptype);

				for (; ;) {
					if (Excep_Key == nullptr) break; //이미 존재하는 키가 없으면 종료

					if (Excep_Key->key.Get_Str() == value) {
						cout << "The Key Already Exists" << endl;
						return; // 이미 존재하는 키인 경우 예외 처리
					}
					Excep_Key = Excep_Key->next; // 다음 객체로 이동
				}

				if (key == nullptr) {
					cout << "JsonCtrl is Not Obj." << endl; return;
				} JObj* obj = static_cast<JObj*>(parent.root->ptype); 
				obj->Set_Key(key->Get_Str()); 
				obj->Set_Value(JNode::JType::NULL_TYPE);
				parent.Obj = parent.Obj->next; // 다음 객체로 이동
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

		//사용자 API 함수들
		//Push 넣기
		//{} 객체 기준 var["key"] = value;
		Obj_Ctrl& operator[](const char* key) {
			// JsonCtrl 객체가 초기화되지 않은 경우 혹은 객체가 아닌 경우
			if (root == nullptr) {
				//nullptr 인 경우 예외 처리
				static Obj_Ctrl dummy(*this, "");
				return dummy;
			}
			else if (root->type == JNode::JType::ARRAY) {
				//배열 타입인 경우 넘겨주기
				static Obj_Ctrl dummy(*this, "arr");
				return dummy;
			}


			if (root->type == JNode::JType::OBJECT) {
				Child = new Obj_Ctrl(*this, key);
				return *Child; // Obj_Ctrl 객체 반환
			}
			else {
				//잘못된 타입인 경우 예외 처리
				static Obj_Ctrl dummy(*this, "");
				return dummy;
			}


		}

		//Pop 빼기
		

	private:
		JObj* Obj;			// Json 객체
		Obj_Ctrl* Child;	// 리소스 하위 객체
		JNode* root;		// Json의 루트 노드
	};

	*/
}

