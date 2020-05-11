#include"pch.h"
#include<iostream>
#include<string>
using namespace std;

int id = 0;
template<class T1, class T2>
class Kolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija()
	{
		_trenutno = 0;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}

	~Kolekcija()
	{
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;
	}

	void DodajElemente(const T1& el1, const T2& el2)
	{
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		for (int i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}

	int GetTrenutno() const { return _trenutno; }
	T1* GetE1() const { return _elementi1; }
	T2* GetE2() const { return _elementi2; }

	friend ostream& operator<< <>(ostream&, const Kolekcija&);
};

template<class T1, class T2>
ostream& operator<< (ostream& cout, const Kolekcija<T1, T2>& k)
{
	for (int i = 0; i < k._trenutno; i++)
	{
		cout << k._elementi1[i] << "\t"<<k._elementi2[i]<< endl;
	}

	return cout;
}


//U programu implementirati nasljeðivanje izmeðu odgovarajuæih klasa
//Klasu Osoba deklarisati kao apstraktnu.
class Osoba
{
	char* _ime;
	char* _prezime;
	char* _adresa;
	char _telefon[15];
public:

	Osoba(const char* ime, const char* prezime, const char* adresa, const char* telefon)
	{
		int vel = strlen(ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, ime);

		vel = strlen(prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, prezime);

		vel = strlen(adresa) + 1;
		_adresa = new char[vel];
		strcpy_s(_adresa, vel, adresa);

		strcpy_s(_telefon, 15, telefon);
	}

	virtual ~Osoba()
	{
		delete[] _ime;
		_ime = nullptr;
		delete[] _prezime;
		_prezime = nullptr;
		delete[] _adresa;
		_adresa = nullptr;
	}

	virtual bool IsPotrosnjaVeca(int mjesec, double potrosnja) = 0;
	virtual char* GetIme() { return _ime; }
	virtual char* GetEmail() { return _adresa; }

	void Info()
	{
		cout << "Ime i prezime: " << _ime << " " << _prezime << endl;
		cout << "Adresa: " << _adresa << endl;
		cout << "Telefon: " << _telefon << endl;
	}
};

class Proizvod
{
	char* _naziv;
	char* _opis;
	double _cijena;
public:
	Proizvod(const char* naziv="NoN", const char* opis="non", double cijena=0.0)
	{
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);

		vel = strlen(opis) + 1;
		_opis = new char[vel];
		strcpy_s(_opis, vel, opis);

		_cijena = cijena;
	}

	Proizvod(const Proizvod& org)
	{
		int vel = strlen(org._naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, org._naziv);

		vel = strlen(org._opis) + 1;
		_opis = new char[vel];
		strcpy_s(_opis, vel, org._opis);

		_cijena = org._cijena;

	}


	Proizvod & operator=(const Proizvod & right) {
		if (this != &right) {
			this->_naziv = new char[strlen(right._naziv) + 1];
			strcpy_s(this->_naziv, strlen(right._naziv) + 1, right._naziv);
			this->_opis = new char[strlen(right._opis) + 1];
			strcpy_s(this->_opis, strlen(right._opis) + 1, right._opis);
			this->_cijena = right._cijena;
		}
		return *this;
	}


	~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;

		delete[] _opis;
		_opis = nullptr;
	}


	double GetCijenu() { return _cijena; }
	void SetCijenu(int cijena) { _cijena = cijena; }
	friend ostream& operator<<(ostream&, const Proizvod&);

};

ostream& operator<<(ostream& cout, const Proizvod& p)
{
	cout << "Naziv: " << p._naziv << endl;
	cout << "Opis: " << p._opis << endl;
	cout << "Cijena: " << p._cijena << endl;

	return cout;
}


class Narudzba {
	char _datum[12]; //Format: dd.MM.yyyy.
	Kolekcija<Proizvod*, int> _proizvodi; //Podaci o naruèenim proizvodima i kolièinama
public:
	//Potrebne osnovne funkcije za rad sa klasom
	Narudzba(const char* datum = "1.1.2000.")
	{
		strcpy_s(_datum, 12, datum);

	}

	Narudzba(const Narudzba& org)
	{
		strcpy_s(_datum, 12, org._datum);
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			_proizvodi.DodajElemente(org._proizvodi.GetE1()[i], org._proizvodi.GetE2()[i]);
		}

	}

	Narudzba& operator=(const Narudzba& org)
	{
		if (this != &org)
		{
				strcpy_s(this->_datum, 12, org._datum);
				for (int i = 0; i < _proizvodi.GetTrenutno(); i++)
				{
					this->_proizvodi.DodajElemente(org._proizvodi.GetE1()[i], org._proizvodi.GetE2()[i]);
				}

		}
		return *this;
	}
	void DodajProizvod(const Proizvod& proizvod, int kolicina)
	{
		_proizvodi.DodajElemente(new Proizvod(proizvod), kolicina);
	}

	int GetMjesec() {
		string temp = "";
		const char* tacka = ".";
		int brojacT = 0;
		for (int i = 0; i < strlen(_datum); i++)
		{
			if (_datum[i] == *tacka)
			{
				brojacT++;
			}
			else if(brojacT>0 && brojacT<2)
				temp += _datum[i];
		}
		return stoi(temp);
	}
	//Funkciju GetIznosNarudzbe koja vraæa ukupan iznos narudžbe na osnovu
	//podataka o cijenama proizvoda i naruèenim kolièinama. Funkcija prima neobavezni parametar popust
	//koji inicijalno ima vrijednost 0, što znaèi da ne postoji popust na postojeæe cijene proizvoda.
	double GetIznosNarudzbe(float popust=0) {
		double sum = 0;
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			sum += _proizvodi.GetE1()[i]->GetCijenu()*_proizvodi.GetE2()[i];
		}
		return sum-popust;

	}
	//Funkciju za ispis svih vrijednosti atributa klase.


	int GetTrenutnoProizvoda() const { return _proizvodi.GetTrenutno(); }

	friend ostream & operator<<(ostream & cout, const Narudzba & n) {
		cout << "\tDatum narudzbe: " << n._datum << endl;
		cout << "\nProizvodi:\n" << n._proizvodi << endl;
		return cout;
	}
};



class Klijent :public Osoba
{
	int _id;
	char* _email;
	Kolekcija<Narudzba, double> _narudzbe; //Podaci o narudžbama sa ukupnim utrošenim iznosom u KM
public:
	//Potrebne konstruktor i destruktor funkcije.
	Klijent(const char* email,
		const char* ime,
		const char* prezime,
		const char* tel,
		const char* adresa) :Osoba(ime, prezime, adresa, tel), _id(++id) {
		_email = new char[strlen(email) + 1];
		strcpy_s(_email, strlen(email) + 1, email);
		
	}


	~Klijent()
	{
		delete _email; _email = nullptr;
	}
	//Funkciju za evidenciju nove narudžbe.
	void DodajNarudzbu(const Narudzba& narudzba, double iznos)
	{
		_narudzbe.DodajElemente(narudzba, iznos);
	}
	//Funkciju koja vraæa logièku vrijednost true ukoliko je klijent za odabrani mjesec u godini imao
	//potrošnju veæu od zadate. Prototip funkcije: bool IsPotrosnjaVeca(int mjesec, double potrosnja);
	//Za pronalazak potrosnje po mjesecima koristiti podatak o datumu narudžbe. 

	bool IsPotrosnjaVeca(int mjesec, double potrosnja)
	{
		for (int i = 0; i < _narudzbe.GetTrenutno(); i++)
		{
			if (_narudzbe.GetE1()[i].GetMjesec() == mjesec)
			{
				if (_narudzbe.GetE1()[i].GetIznosNarudzbe() > potrosnja)
					return true;
			}
		}
		return false;
	}

	//Funkciju za izmjenu e-mail adrese.

	//Funkciju za ispis svih vrijednosti atributa klase.
	void Info()
	{
		Osoba::Info();

		cout << "--------Klijent ispis narudjbe-----" << endl;
		cout << "id: " << _id << "\t email: " << _email << endl;
		cout << _narudzbe;
	}

	void IzmjenaEmailAdrese(const char* email)
	{
		delete _email;
		_email = new char[strlen(email) + 1];
		strcpy_s(_email, strlen(email) + 1, email);

	}

	void GenerisiEmail(const char * nastavak = "edu.fit.ba")
	{
		char tempmail[40];
		for (size_t i = 0; i < strlen(_email); i++)
		{
			if (_email[i] == '@') {
				tempmail[i] = '@';
				int brojac = i + 1;
				int brojacNastavka = 0;
				while (brojacNastavka < strlen(nastavak)) {
					tempmail[brojac++] = nastavak[brojacNastavka++];
				}
				tempmail[brojacNastavka + i + 1] = '\0';
				break;
			}
			else {
				tempmail[i] = _email[i];
			}
		}
		if (_email != nullptr) {
			delete[] _email;
			_email = new char[strlen(tempmail) + 1];
			strcpy_s(_email, strlen(tempmail) + 1, tempmail);
		}
		else {
			_email = new char[strlen(tempmail) + 1];
			strcpy_s(_email, strlen(tempmail) + 1, tempmail);
		}
	}

	Kolekcija<Narudzba, double>& GetNarudzbe() { return _narudzbe; }
	Narudzba& GetPosljednjuNarudzbu() { return _narudzbe.GetE1()[_narudzbe.GetTrenutno() - 1]; }
	int TrenutnoNarudzbi() { return _narudzbe.GetTrenutno(); }

};

class VIPKlijent : public Klijent
{
	char* _parkingOznaka;
	double _popust;
public:
	//Potrebne konstruktor i destruktor funkcije
	VIPKlijent(const char* mail,
		const char* ime, const char* prezime,
		const char* tel, const char* adresa, const char* parking, int popust) : Klijent(mail, ime, prezime, tel, adresa), _popust(popust)
	{
		int vel = strlen(parking) + 1;
		_parkingOznaka= new char[vel];
		strcpy_s(_parkingOznaka, vel, parking);

	}

	void DodajNarudzbuVIP(const Narudzba& narudzba, int iznos)
	{
		Klijent::DodajNarudzbu(narudzba, iznos*_popust / 100);
	}


	//Funkciju za evidenciju nove narudžbe sa zaraèunatim popustom na sve proizvode.

	//Funkciju koja posljednjoj narudžbi klijenta dodaje gratis proizvod proslijeðen kao parametar.
	void DodajProizvodGratis(Proizvod gratis)
	{
		gratis.SetCijenu(0);

		if (Klijent::TrenutnoNarudzbi() == 0)
		{
			Klijent::GetNarudzbe().GetE1()->DodajProizvod(gratis, 1);
			return;
		}
		else
		{
			Klijent::GetPosljednjuNarudzbu().DodajProizvod(gratis, 1);
			return;
		}

	}
	//Funkciju za ispis svih vrijednosti atributa klase
	void Info() {
		Klijent::Info();
		cout << endl;
		cout << "Oznaka parking " << _parkingOznaka << endl;
	}
	double GetPopust() { return _popust; };
	double GetPotrosnju() {
		int x = Klijent::GetPosljednjuNarudzbu().GetIznosNarudzbe()*_popust / 100;
		return Klijent::GetPosljednjuNarudzbu().GetIznosNarudzbe() - x;
	}
};

//Funkcija treba da pronaðe sve one klijente koji su za odabrani mjesec ostvarili potrošnju veæu
//od zadate. Ispisati njihove podatke i vratiti ukupan broj pronaðenih klijenata.
int PronadjiKlijentePoPotrosnji(Osoba* niz[], int max, double zadanaPotrosnja, int mjesec)
{
	int brojac = 0;
	for (int i = 0; i < max; i++)
	{
		if (niz[i]->IsPotrosnjaVeca(mjesec, zadanaPotrosnja))
			brojac++;
	}
	return brojac;
}

//Pronaæi sve VIP klijente, te im dodati gratis proizvod proslijeðen kao parametar.
void UkljuciGratisProizvodVIPKlijentima(Osoba* niz[], int max, const Proizvod& p)
{
	for (int i = 0; i < max; i++)
	{
		VIPKlijent* o = dynamic_cast<VIPKlijent*>(niz[i]);
		if (o != nullptr)
		{
			o->DodajProizvodGratis(p);
			cout << "VIP klijent sa gratis proizvodom je " << niz[i]->GetIme() << endl;
		}
		
	}
}

//Svim klijentima postaviti mail adresu u formatu: ime.prezime@firma.ba.
//void GenerisiMailAdreseKlijentima(Osoba* niz[], int max);

//Omoguæiti pretragu klijenata po imenu ili prezimenu (ispisati informacije
//o svim klijentima koji sadrže uneseni tekst u dijelu imena ili prezimena.
//void PronadjiKlijentePoImenuPrezimenu(Osoba* niz[], int max);

int main()
{
	//Testirati sve dostupne funkcionalnosti uz obaveznu provjeru globalnih funkcija.
	//VIP klijentima sve narudžbe kreirati uz zaraèunati popust pozivom odgovarajuæe funkcije.

	//Testirati sve dostupne funkcionalnosti uz obaveznu provjeru globalnih funkcija.
	//VIP klijentima sve narudžbe kreirati uz zaracunati popust pozivom odgovarajuce funkcije.

#pragma region Testiranje Kolekcije
	Kolekcija<int, double> k1;
	k1.DodajElemente(74, 22);
	k1.DodajElemente(75, 22.7);
	k1.DodajElemente(76, 233.31);
	k1.DodajElemente(747, 2.414);
	cout << k1 << endl;

	Proizvod majoneza("Majoneza", "Stetno", 2.70);
	Proizvod salama("Salama", "Stetno", 3.85);
	Proizvod kuhalo("Kuhalo", "Za kafu", 69.99);
	Proizvod telefon("Samsung", "Galaxy S9 Plus", 914.99);
	Proizvod casa("Casa", "Plasticna", 1.85);
	Proizvod patike("Nike", "Air Max Turbo Fast 2.0 TDI", 150.00);
	Proizvod brzeCvike("Brzzze", "Samo brzina", 13.90);
	Proizvod zvucnici("KRK", "Rokit 10 (par)", 1099.99);
	Proizvod flasa("Flasa", "Staklena", 0.75);

	Kolekcija<int, Proizvod> k2;
	k2.DodajElemente(1, majoneza);
	k2.DodajElemente(2, salama);
	k2.DodajElemente(3, kuhalo);
	cout << k2 << endl;
#pragma endregion


#pragma region Testiranje Narudzbe
	Narudzba narudzba("1.10.2018");
	cout << "-> Trenutno proizvoda: " << narudzba.GetTrenutnoProizvoda() << endl;
	cout << "-> Iznos narudzbe: " << narudzba.GetIznosNarudzbe() << endl;
	narudzba.DodajProizvod(majoneza, 5);
	narudzba.DodajProizvod(salama, 7);
	narudzba.DodajProizvod(kuhalo, 2);
	cout << "-> Trenutno proizvoda: " << narudzba.GetTrenutnoProizvoda() << endl;
	cout << "-> Iznos narudzbe: " << narudzba.GetIznosNarudzbe() << endl;
	cout << "Mjesec: "; narudzba.GetMjesec();
	cout << endl << endl;
#pragma endregion


#pragma region Testiranje Klijenta
	Klijent klijent1("ajdin.hukara@gmail.com", "Ajdin", "Hukara", "061/550-135", "Adresa 5");
	klijent1.DodajNarudzbu(narudzba, 10);
	klijent1.Info();
	if (klijent1.IsPotrosnjaVeca(10, 181)) cout << "Potrosnja je veca!\n";
	else cout << "Potrosnja je manja!\n";
	klijent1.IzmjenaEmailAdrese("neurouzmedia@gmail.com");
	klijent1.Info();
#pragma endregion

#pragma region Testiranje VIP Klijenta
	VIPKlijent klijentVIP("nerma.curic@gmail.com", "Nerma", "Curic", "062/611-193", "Adresa 7", "C8", 17);
	klijentVIP.DodajNarudzbuVIP(narudzba, 70);
	Proizvod _gratis("Bicikl", "Sportski", 199.90);
	klijentVIP.DodajProizvodGratis(_gratis);
	klijentVIP.Info();
	cout << "Posljednja narudzba (" << klijentVIP.GetPopust() << "% popust) ------> " << klijentVIP.GetPotrosnju() << " KM\n\n";
#pragma endregion

#pragma region Testiranje Funkcija
	Narudzba n1("14.10.2018.");
	Narudzba n2("12.10.2018.");

	n1.DodajProizvod(zvucnici, 7);		n2.DodajProizvod(patike, 3);
	n1.DodajProizvod(salama, 15);		n2.DodajProizvod(flasa, 500);
	n1.DodajProizvod(brzeCvike, 80);	n2.DodajProizvod(telefon, 20);

	Klijent prvi("denis.m@gmail.com", "Denis", "Music", "061/107-994", "Adresa br. 5");
	prvi.DodajNarudzbu(narudzba, 1);
	Klijent drugi("indira@hotmail.com", "Indira", "Hamulic", "033/711-211", "Adresa br. 4");
	drugi.DodajNarudzbu(n1, 3);
	VIPKlijent treci("jasmin.azemovic@edu.fit.ba", "Jasmin", "Azemovic", "061/201-747", "Adresa br. 3", "D11", 15);
	treci.DodajNarudzbu(n2, 2);
	Klijent cetvrti("adel_cybersecurity@w.cn", "Adel", "Handzic", "063/515-001", "Adresa br. 2");
	VIPKlijent peti("amila_h@gmail.com", "Amila", "Handzic", "033/991-171", "Adresa br. 1", "B01", 4.5);
	peti.DodajNarudzbu(n1, 87);

	Osoba * _nizOsoba[5];

	_nizOsoba[0] = &prvi;
	_nizOsoba[1] = &drugi;
	_nizOsoba[2] = &treci;
	_nizOsoba[3] = &cetvrti;
	_nizOsoba[4] = &peti;

	int brojKlijenata = PronadjiKlijentePoPotrosnji(_nizOsoba, 5, 20, 10);
	cout << "-- Broj klijenata koji su ostvarili potrosnju vecu od 20KM -> " << brojKlijenata << endl;
	UkljuciGratisProizvodVIPKlijentima(_nizOsoba, 5, _gratis);
	_nizOsoba[4]->Info();
	_nizOsoba[3]->Info();
	_nizOsoba[2]->Info();
	_nizOsoba[1]->Info();
	_nizOsoba[0]->Info();
	//GenerisiMailAdreseKlijentima(_nizOsoba, 5);
	//PronadjiKlijentePoImenuPrezimenu(_nizOsoba, 5, "Ajda");
	//PronadjiKlijentePoImenuPrezimenu(_nizOsoba, 5, "Denis");

#pragma endregion


	return 0;
}
