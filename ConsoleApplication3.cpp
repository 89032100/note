#include <iostream>  
#include <vector>  
#include <string>  
#include <iomanip>  
#include <sstream>  
#include <io.h>  
#include <fcntl.h>  
#include <windows.h>
using namespace std;

class Note
{
protected:
	int nlength;
	string tone;
	void set(int len)
	{
		nlength = len;
	}
	void set(string t)
	{
		tone = t;
	}
public:
	Note()
	{
		nlength = 0;

	}
	Note& operator=(Note& other)
	{
		nlength = other.nlength;
		tone = other.tone;
		return *this;
	}
	int length() {
		return nlength;
	}
	friend istream& operator>>(istream& is, Note& note)
	{
		static const string tones[] = { "La","Si","Do","Re","Mi","Fa","Sol" };
		char buf[2];
		is.get(buf, 2);
		note.tone = tones[buf[0] - 'A'];
		note.nlength = (buf[1] - '0');
		return is;
	}
	friend ostream& operator<<(ostream& os, const Note& note)
	{
		SetConsoleOutputCP(CP_UTF8);
		int error = _setmode(_fileno(stdout), _O_TEXT);
		os << setw(4) << note.tone;
		return os;
	}
	friend wostream& operator<<(wostream& os, const Note& note)
	{
		SetConsoleOutputCP(CP_UTF8);
		int error = _setmode(_fileno(stdout), _O_U8TEXT);
		wstring symbols[] = { L"♩" ,L"♪" };
		os << setw(3) << symbols[note.nlength / 4 - 1];
		return os;
	}
};

class Note4 : public Note
{
public:
	Note4()
	{
		set(4);
	}
};

class Note8 : public Note
{
public:
	Note8()
	{
		set(8);
	}
};

class SubSection {
private:
	vector <Note*> notes;
	int nBeat;
	double length;
public:
	SubSection()
	{
		length = 0.0;
		nBeat = 0;
	}
	SubSection(int n)
	{
		nBeat = n;
		length = 0.0;
	}
	~SubSection() {
		for (unsigned int i = 0; i < notes.size(); i++)
			delete notes[i];
		notes.clear();
	}
	bool add(Note* note)
	{
		if (length >= nBeat)
			return false;
		notes.push_back(note);
		length += (4.0 / note->length());
		return true;
	}
	friend istream& operator>>(istream& is, SubSection& section)
	{
		if (section.length >= section.nBeat) return is;
		Note temp;
		Note* note;
		while (section.length < section.nBeat)
		{
			is >> temp;
			switch (temp.length())
			{
			case 4:note = new Note4; break;
			case 8:note = new Note8; break;
			default:return is;
			}
			(*note) = temp;
			section.add(note);
		}
		return is;
	}
	friend ostream& operator<<(ostream& os, const SubSection& section)
	{
		for (unsigned int i = 0; i < section.notes.size(); i++)
			os << *(section.notes[i]) << " ";
		return os;
	}
	friend wostream& operator<<(wostream& os, const SubSection& section)
	{
		for (unsigned int i = 0; i < section.notes.size(); i++)
			os << *(section.notes[i]) << " ";
		return os;
	}
};

class Score {
private:
	vector <SubSection*> sections;
	int nBeat;
	int n;
public:
	Score() {
		nBeat = 0;
		n = 0;
	}
	~Score() {
		for (unsigned int i = 0; i < sections.size(); i++)
			delete sections[i];
		sections.clear();
	}
	void add(SubSection* section)
	{
		sections.push_back(section);
	}
	friend istream& operator>>(istream& is, Score& score)
	{
		is >> score.n;
		is >> score.nBeat;
		string buf;
		is >> buf;
		stringstream ss(buf);
		SubSection* section;
		for (int i = 0; i < score.n; i++)
		{
			section = new SubSection(score.nBeat);
			ss >> (*section);
			score.sections.push_back(section);
		}
		return is;
	}
	friend ostream& operator<<(ostream& os, const Score& score)
	{
		for (unsigned int i = 0; i < score.sections.size(); i++)
			os << *(score.sections[i]) << " | ";
		return os;
	}
	friend wostream& operator<<(wostream& os, const Score& score)
	{
		for (unsigned int i = 0; i < score.sections.size(); i++)
			os << *(score.sections[i]) << "   ";
		return os;
	}
};


void testScore()
{
	Score score;
	cin >> score;
	cout << score << endl;
	wcout << score << endl;
}

void testNote()
{
	Note note;
	cin >> note;
	wcout << note;
	cout << note;
}


void testSection2()
{
	SubSection section(4);
	cin >> section;
	cout << section << endl;
	wcout << section << endl;
}

void testSection1()
{
	SubSection* section = new SubSection(4);
	section->add(new Note4);
	section->add(new Note4);
	section->add(new Note8);
	section->add(new Note4);
	section->add(new Note4);
	Score score;
	score.add(section);
	wcout << score << endl;
}

int main()
{
	testScore();
}