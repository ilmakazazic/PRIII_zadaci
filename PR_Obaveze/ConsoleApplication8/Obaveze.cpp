// ConsoleApplication8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include<vector>
#include<string>
#include<iterator>
#include<sstream>
#include<exception>
using namespace std;
const char* crt = "\n---------------------------------------------------------\n";


class Datum
{
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000)
	{
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum &dat)
	{
		_dan = new int(*dat._dan);
		_mjesec = new int(*dat._mjesec);
		_godina = new int(*dat._godina);
	}
	Datum& operator=(const Datum &dat)
	{
		if (this != &dat)
		{
			_dan = new int(*dat._dan);
			_mjesec = new int(*dat._mjesec);
			_godina = new int(*dat._godina);

		}
		return *this;
	}
	~Datum()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream &operator<<(ostream &COUT, const Datum &obj)
	{
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}

};
class Izuzetak : public exception
{
	string _funkcija;
public:
	Izuzetak(const char* greska, string f) : exception(greska)
	{
		_funkcija = f;
	}

};

template <class T1, class T2>
class Kolekcija
{
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutnoElemenata;
	bool _omoguciDupliranjeElemenata;
public:
	Kolekcija(bool omoguciDupliranjeElemenata = false)
	{
		_trenutnoElemenata = 0;
		_omoguciDupliranjeElemenata = omoguciDupliranjeElemenata;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Kolekcija(const Kolekcija& org)
	{

		_trenutnoElemenata = org._trenutnoElemenata;
		_omoguciDupliranjeElemenata = org._omoguciDupliranjeElemenata;
		_elementi1 = new T1[org._trenutnoElemenata];
		_elementi2 = new T2[org._trenutnoElemenata];

		for (int i = 0; i < org._trenutnoElemenata; i++)
		{
			_elementi1[i] = org._elementi1[i];
			_elementi2[i] = org._elementi2[i];
	}	}

	Kolekcija& operator=(const Kolekcija& org)
	{
		if (this != &org) {

			_trenutnoElemenata = org._trenutnoElemenata;
			_omoguciDupliranjeElemenata = org._omoguciDupliranjeElemenata;
			delete[] _elementi1; delete[] _elementi2;
			_elementi1 = nullptr; _elementi2 = nullptr;
			_elementi1 = new T1[org._trenutnoElemenata];
			_elementi2 = new T2[org._trenutnoElemenata];

			for (int i = 0; i < org._trenutnoElemenata; i++)
			{
				_elementi1[i] = org._elementi1[i];
				_elementi2[i] = org._elementi2[i];
			}
		}
		return *this;
	}

	bool AddElement(T1 e1, T2 e2)
	{
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			if (_elementi1[i]==e1 && _elementi2[i]==e2 && !_omoguciDupliranjeElemenata)
				return false;

		}
		T1* temp1 = new T1[_trenutnoElemenata+1];
		T2* temp2 = new T2[_trenutnoElemenata + 1];
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutnoElemenata] = e1;
		temp2[_trenutnoElemenata] = e2;
		delete[]_elementi1; delete[]_elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_trenutnoElemenata++;
		return true;
	}

	void RemoveElement(int index)
	{
		for (int i = index; i < _trenutnoElemenata-1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutnoElemenata--;
	}
	~Kolekcija()
	{
		delete[]_elementi2; _elementi2 = nullptr;
		delete[]_elementi1; _elementi1 = nullptr;
		_trenutnoElemenata = 0;
	}

	int GetTrenutno() const { return _trenutnoElemenata; }
	T1& GetElement1(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi1[lokacija];
	}

	T2& GetElement2(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi2[lokacija];
	}

	void SetElement2(int lok, T2 e) {
		_elementi2[lok] = e;
	}
	friend ostream &operator<<(ostream &COUT, const Kolekcija &obj)
	{
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
};

class Dogadjaj
{
	Datum _datumOdrzavanja;
	Kolekcija<string, bool> * _obaveze; //cuva informaciju o obavezama koje je potrebno ispuniti prije samog dogadjaja, string se odnosi na opis, 
					//a bool na izvrsenje te obaveze (da li je zavrsena ili ne)

	char *_naziv;
	int _notificirajPrije; //oznacava broj dana prije samog dogadjaja kada ce krenuti notifikacija/podsjetnik
	bool _rekurzivnaNotifikacija; //ako je vrijednost true onda se korisnik notificira svaki dan do _datumaOdrzavanja dogadjaja, 
					  //a pocevsi prije dogadjaja za _brojDanaZaNotifikaciju

public:
	Dogadjaj(Datum datumOdrzavanja, const char *naziv, int brojDana = 1,
		bool rekurzivnaNotifikacija = false) : _datumOdrzavanja(datumOdrzavanja)
	{
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);

		_notificirajPrije = brojDana;
		_rekurzivnaNotifikacija = rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string, bool>();
	}

	Dogadjaj(const Dogadjaj &obj) : _datumOdrzavanja(obj._datumOdrzavanja), _obaveze(new Kolekcija<string, bool>(obj._obaveze))
	{
		_naziv = new char[strlen(obj._naziv) + 1];
		strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);

		_notificirajPrije = obj._notificirajPrije;
		_rekurzivnaNotifikacija = obj._rekurzivnaNotifikacija;
		
	}
	bool provjeraZabranjeneRijeci(string opis)
	{
		string v = "video", s = "SQL";
		if (opis == v || opis == s) return false;
	}
	bool AddObavezu(string opis, bool zavrsena = false)
	{
		if (!provjeraZabranjeneRijeci(opis))
			throw Izuzetak("Ova rijec je zabranjena", __FUNCTION__);

		for (int i = 0; i < _obaveze->GetTrenutno(); i++)
		{
			if (_obaveze->GetElement1(i) == opis)
				return false;
		}
		_obaveze->AddElement(opis, zavrsena);

		return true;
	}

	~Dogadjaj()
	{
		delete[] _naziv;
		_naziv = nullptr;
		delete _obaveze;
		_obaveze = nullptr;
	}
	char *GetNaziv() { return _naziv; }
	Kolekcija<string, bool> *GetObaveze() { return _obaveze; }

};

class Student
{
	int _indeks;
	string _imePrezime;
	vector<Dogadjaj> _dogadjaji;
public:

	Student(int indeks, string imePrezime) : _indeks(indeks), _imePrezime(imePrezime) {}
	Student(const Student& o) : _indeks(o._indeks), _imePrezime(o._imePrezime) { _dogadjaji = o._dogadjaji; }

	int GetIndeks() const { return _indeks; }

	bool AddDogadjaj(Dogadjaj d)
	{
		for (int i = 0; i < _dogadjaji.size(); i++)
		{
			if (strcmp(_dogadjaji[i].GetNaziv(), d.GetNaziv())==0)
			{
				return false;
			}
		}
		_dogadjaji.push_back(d);
		return true;
	}
	vector<Dogadjaj>& GetDogadjaji() { return _dogadjaji; }

	friend ostream &operator<<(ostream &COUT, const Student &obj)
	{
		COUT << obj._imePrezime << " (" << obj._indeks << ")" << endl;
		return COUT;
	}
};

class DLWMSReminder
{
	vector<Student> _reminiderList;
public:
	DLWMSReminder() {};
	void AddStudent(Student s)
	{
		for (int i = 0; i < _reminiderList.size(); i++)
		{
			if(_reminiderList[i].GetIndeks()==s.GetIndeks())
				throw Izuzetak("postoji ovaj dogadjaj", __FUNCTION__);

		}
		_reminiderList.push_back(s);
	}
	bool OznaciObavezuKaoZavrsenu(int index, string naziv, string opis)
	{
		for (int i = 0; i < _reminiderList.size(); i++)
		{
			if (_reminiderList[i].GetIndeks() == index)
			{
				for (int j = 0; j < _reminiderList[i].GetDogadjaji().size(); j++)
				{
					if (strcmp(_reminiderList[i].GetDogadjaji()[j].GetNaziv(), naziv.c_str()) == 0)
					{
						for (int k = 0; k < _reminiderList[i].GetDogadjaji()[j].GetObaveze()->GetTrenutno(); k++)
						{
							if (_reminiderList[i].GetDogadjaji()[j].GetObaveze()[k].GetElement1(k) == opis) {
								_reminiderList[i].GetDogadjaji()[j].GetObaveze()[k].SetElement2(k, true);
								return true;

							}
						}
					}

			}
			}
		}
		return false;
	}
};

void main()
{
	/**************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA
	   KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_ParcijalniII.TXT
	5. NAZIVI FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U
	   TESTNOM CODE-U.OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI PO ŽELJI.
****************************************************************************/

#pragma region Datum

	Datum danas(28, 1, 2018), sutra(29, 1, 2018);
	Datum datumIspitaPRIII(30, 1, 2018), datumIspitBPII(31, 1, 2018);
	Datum prekosutra(danas);
	prekosutra = danas;
	cout << danas << endl
		<< sutra << endl
		<< prekosutra << crt;

#pragma endregion

#pragma region Kolekcija

	/*
	AddElement :: omogucava dodavanje novog elementa u kolekciju. Ukoliko je moguce, osigurati automatsko
	prosiranje kolekcije prilikom dodavanja svakog novog elementa, te onemoguciti ponavljanje elemenata
	RemoveElement :: na osnovu parametra tipa T1 uklanja elemente iz kolekcije i ukoliko je moguce smanjuje
	velicinu niza/kolekcije. Prilikom uklanjanja elemenata ocuvati redoslijed njihovog dodavanja
	*/
	const int brElemenata = 10;
	Kolekcija<int, float> kolekcija1;
	for (size_t i = 0; i < brElemenata; i++)
		if (!kolekcija1.AddElement(i, i + (0.6 * i)))
			cout << "Elementi " << i << " i " << i + (0.6 * i) << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) <<crt;
	cout << kolekcija1 << crt;

	kolekcija1.RemoveElement(1);
	cout << kolekcija1 << crt;

	Kolekcija<int, float> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;

	Kolekcija<int, float> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;

#pragma endregion

#pragma region Dogadjaj

	Dogadjaj ispitPRIII(datumIspitaPRIII, "Ispit iz PRIII", 5, true),
		ispitBPII(datumIspitBPII, "Ispit iz BPII", 7, true);
	/*po vlasitom izboru definisati listu zabranjenih rijeci koje ce onemoguciti dodavanje odredjene obaveze. Prilikom provjere koristiti regex*/
	if (ispitPRIII.AddObavezu("Preraditi pdf materijale")) cout << "Obaveza dodana!" << endl;
	//onemoguciti dupliranje obaveza
	if (!ispitPRIII.AddObavezu("Preraditi pdf materijale")) cout << "Obaveza nije dodana!" << endl;
	if (ispitPRIII.AddObavezu("Pregledati video materijale")) cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Preraditi ispitne zadatke")) cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Samostalno vjezbati")) cout << "Obaveza dodana!" << endl;

	if (ispitBPII.AddObavezu("Preraditi knjigu SQL za 24 h")) cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Pregledati video materijale")) cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Napraviti bazu za konkretnu aplikaciju")) cout << "Obaveza dodana!" << endl;

	Student jasmin(150051, "Jasmin Azemovic"), adel(160061, "Adel Handzic");

	if (jasmin.AddDogadjaj(ispitPRIII) && jasmin.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;

	if (adel.AddDogadjaj(ispitPRIII) && adel.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;
	//onemoguciti dupliranje dogadjaja
	if (!adel.AddDogadjaj(ispitPRIII))
		cout << "Dogadjaj nije uspjesno dodan!" << endl;

	DLWMSReminder reminder;

	try
	{
		reminder.AddStudent(jasmin);
		reminder.AddStudent(adel);
		//u slucaju dupliranja studenata funkcija baca izuzetak tipa Izuzetak
		reminder.AddStudent(jasmin);
	}
	catch (exception &err)
	{
		//ispisati sve informacije o nastalom izuzetku
		cout << err.what() << endl;
	}

	//da bi bila oznacena kao zavrsena, obaveza mora postojati i mora biti oznacena kao nezavrsena (false)
	if (reminder.OznaciObavezuKaoZavrsenu(150051, "Ispit iz PRIII", "Pregledati video materijale"))
		cout << "Obaveza oznacena kao zavrsena" << endl;

	///*metodi PosaljiNotifikacije se salje trenutni datum na osnovu cega ona pretrazuje sve studente koje treba
	//podsjetiti/notoficirati o dogadjajima koji se priblizavaju.
	//Koristeci multithread-ing, svim studentima se salju notifikacije sa sljedecim sadrzajem:
	//-------------------------------------------------------------------------
	//Postovani Jasmin Azemovic,
	//Dogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:
	//1.Preraditi ispitne zadatke
	//2.Samostalno vjezbati
	//Predlazemo Vam da ispunite i ostale planirane obaveze.
	//FIT Reminder
	//-------------------------------------------------------------------------
	//Dakle, notifikacije ce biti poslane svim studentima koji su dodali dogadjaj za 30.01.2018. godine i oznacili da
	//zele da budu podsjecani ponovo/rekurzivno najmanje 2 dana prije samog dogadjaja (podaci se odnose
	//na konkretan dogadjaj: Ispit iz PRIII)

	//*/
	//int poslato = 0;
	////funkcija vraca broj poslatih podsjetnika/notifikacija
	//poslato = reminder.PosaljiNotifikacije(danas);
	//cout << "Za " << danas << " poslato ukupno " << poslato << " podsjetnika!" << endl;
	//poslato = reminder.PosaljiNotifikacije(sutra);
	//cout << "Za " << sutra << " poslato ukupno " << poslato << " podsjetnika!" << endl;

#pragma endregion

	system("pause");
}