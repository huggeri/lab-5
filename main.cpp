#include "Header.h"

int Unique_word::count_unique_words = 0;

void main()
{
	char str[1000];
	cin.getline(str, 1000);

	Dict dictionary;
	
	dictionary.add_all_words(str);

	cout << dictionary;

	cin.getline(str, 1000);

	dictionary.add_all_words(str);
	cout << dictionary;

	system("PAUSE");
}
