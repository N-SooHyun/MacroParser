#pragma once

#define DEEP_COPY_STR(for_init, for_if, for_In_De, lvalue, rvalue) \
	for(for_init; for_if; for_In_De){\
		lvalue[i] = rvalue[i]; \
			}

namespace Dynamic {
	class DynamicStr {
	private:
		char* Str;
		int capacity_size;
		int current_size;	//������ '\0'������ �� ex) New Word\0 => 8
		int str_last_focus;	//������ ���ڱ����� �� ex) New Word]0 => 7
	public:
		DynamicStr(int _size) : capacity_size(_size), current_size(-1), str_last_focus(-1) {
			Str = new char[capacity_size];
		}
		~DynamicStr() {
			if (Str != nullptr) {
				delete[] Str;
			}
		}

		char* Get_Str() {
			return Str;
		}

		//���ڿ� ������ �����ϱ�
		void Set_Str(const char* new_str) {
			int i = 0;
			for (; new_str[i] != '\0'; i++) {
				if (i >= capacity_size) {
					SizeUpStr();
				}
				Str[i] = new_str[i];
			}
			Str[i] = new_str[i];	// \0����
			current_size = i;
			str_last_focus = i - 1;

			//FitSizeStr();
		}

		//���� 1���� �߰�
		void Append_Char(const char* new_char) {
			if (current_size <= capacity_size) {
				SizeUpStr();
			}
			Str[current_size++] = new_char[0];
			Str[current_size] = '\0';
		}

	private:
		//<���ڿ� ó�� ������ ���� API>

		//���ڿ� ũ�� ���� ����
		void SizeUpStr() {
			int old_capacity = capacity_size;
			capacity_size = (capacity_size / 2) > 2 ? (capacity_size / 2) + capacity_size : capacity_size + 2;
			char* new_size_str = new char[capacity_size + 1];		//Null�������� +1

			//Deep Copy (new_size_str <- Str)
			DEEP_COPY_STR(int i = 0, i < old_capacity, i++, new_size_str, Str);
			delete Str;

			Str = new_size_str;
		}

		//���ڿ� ����� ���� ���� ������� ���� ������� �߻�
		void FitSizeStr() {
			int null_current_size = current_size + 1;			//Null �����ؼ� �־������ +1
			char* new_size_str = new char[null_current_size];
			DEEP_COPY_STR(int i = 0, i < null_current_size, i++, new_size_str, Str);
			capacity_size = null_current_size;
			delete Str;
			Str = new_size_str;
		}
	};

}
