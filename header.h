#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class Dict;

class Word_string
{
protected:
	string *word;
public:
	Word_string() 
	{ 
		word = new string; 
	}

	Word_string(char _string[]) 
	{ 
		word = new string(_string); 
	}

	Word_string(const Word_string &object) 
	{ 
		word = new string(*(object.word)); 
	}

	virtual ~Word_string() 
	{ 
		delete word; 
	}

	virtual string *get_ptr() = 0;
};

class Unique_word : public Word_string
{
protected:
	friend class Dict;
	static int count_unique_words; //не забыть инициализировать
	bool founded; // показывает, найдено ли слово в тексте
public:
	Unique_word();
	Unique_word(char _string[]);
	Unique_word(const Unique_word &obj);
	~Unique_word();

	bool operator == (const Unique_word &obj) const;
	Unique_word & operator = (const Unique_word &obj);
	bool get_flag() const;
	virtual string *get_ptr();
	static int get_count_unique_words();// вернули количество слов в словаре
};

Unique_word::Unique_word() : Word_string()
{
	founded = false;
	count_unique_words++;
}

Unique_word::Unique_word(char _string[]) : Word_string(_string)// конструктор  слова
{
	founded = true; // слово найдено в тексте
	count_unique_words++;
}

Unique_word::Unique_word(const Unique_word &obj) : Word_string(obj)
{
	founded = obj.founded;
	count_unique_words++;
}

Unique_word::~Unique_word()
{
	count_unique_words--;
}

bool Unique_word::operator == (const Unique_word &obj) const
{
	return *word == *(obj.word);
}

Unique_word & Unique_word::operator = (const Unique_word &obj)
{
	*word = *(obj.word);
	founded = obj.founded;
	return *this;
}

bool Unique_word::get_flag() const
{
	return founded;
}

string * Unique_word::get_ptr() // вернули СЛОВО
{
	return word;
}

int Unique_word::get_count_unique_words()
{
	return count_unique_words;
} // вернули количество слов в словаре

class Dict:public vector <Unique_word> // словарь  с указателями на слова, наследник шаблона класса вектор
{
protected:
	void new_text();
public:
	Dict();
	~Dict();

	iterator find_word(const Unique_word &_word);
	void add_word(const Unique_word &_word);
	void add_all_words(char text[]); //добавить слова из нового текста и удалить те, которых нет теперь
	friend ostream & operator << (ostream &out, const Dict & dictionary);// вывод словаря
};

Dict::Dict():vector()// cоздали в словаре вектор (конструктор по умолчанию)
{
}

Dict::~Dict() //деструктор для вектора вызывается сам
{
}

void Dict::new_text()
{
	iterator it = begin();
	while (it != end()) // пока итератор не равен итератору на посл. посл. элем.
	{
		it->founded = false;
		++it;
	}
}

vector<Unique_word>::iterator Dict::find_word(const Unique_word &_word)
{
	iterator it = begin();
	while (it != end())
	{
		if (*it == _word)
			break;
		++it;
	}
	return it;
}

void Dict::add_word(const Unique_word &_word) // добавить слово
{

	iterator it = find_word(_word); //равен результату поиска слова
	if (empty()) //если словарь пуст 
	{
		resize(1);
		operator[](0) = _word;
	}
	else if (it == end()) // если искомый объект ненайден - равен итератору, вслед за концом веткора
	{
		resize(size() + 1);
		operator[](size() - 1) = _word;//вставили элемент в конец словаря
	}
	else if (!it->founded) // если слово есть, но текст новый, тогда тру
	{
		it->founded = true;
	}
}

void Dict::add_all_words(char text[])
{
	if (!empty())
		new_text(); //при создании нового текста, всё false
					//идем по строке и формируем словарь
	int i = 0;

	while (text[i] != '\0')
	{
		if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
		{
			char temp_str[30];
			int j = 0;
			int len_word = 0;
			while ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
			{
				temp_str[j] = text[i];
				i++, j++;
				len_word++;
			}
			temp_str[len_word] = '\0';
			Unique_word temp1(temp_str);	
			add_word(temp1);
		}
		if (text[i] == '\0')
			break;
		i++;
	}
	// удаление слов, которые не встретились, из словаря
	iterator it = begin();
	while (it != end())
	{
		if (!it->get_flag())
		{
			erase(it);
			it = begin();
		}
		else
			it += 1;
	}
}

ostream & operator << (ostream &out, Dict & dictionary)
{
	vector<Unique_word>::iterator it;
	it = dictionary.begin();
	while (it != dictionary.end())
	{
		out << it->get_ptr() << "\t" << (*it->get_ptr()) << "\t" << endl;
		it += 1;
	}
	out << "Dictionary size: " << dictionary.size() << "\n" << "Total words in dictionary: " << Unique_word::get_count_unique_words() << endl;
	return out;
}
