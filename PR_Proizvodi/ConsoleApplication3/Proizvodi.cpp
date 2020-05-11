// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include<iostream>
#include<iomanip>
using namespace std;

char* AlocirajNizKaraktera(const char* t) {
	if (t == nullptr) return nullptr;
	char* n = new char[strlen(t) + 1];
	strcpy_s(n, strlen(t) + 1, t);
	return n;
}

class NedozvoljenaOperacija : public exception {
	int linijaKoda;
public:
	NedozvoljenaOperacija(const char * poruka, int lin) :exception(poruka) {
		linijaKoda = lin;
	}
	void ErrorInfo() { cout << "Greska: " << exception::what() << " Linija koda: " << linijaKoda << endl; }
};

//Kataloška prodaja
class Proizvod
{
protected:
	char _sifra[10];
	char* _naziv;
	double _cijena;
	double _popust; //Izrazen procentualno
	int _kolicina;	//Dostupno proizvoda

public:
	//Konstruktor i destruktor funkcije

	Proizvod(const char* sifra, const char* naziv, double cijena, int kolicina, double popust = 0)
	{
		//U sluèaju nedozvoljenih vrijednosti za cijenu, popust i kolièinu baciti izuzetak
		if (cijena < 0.0 || popust < 0 || popust>100 || kolicina < 0)
			throw NedozvoljenaOperacija("gresska ne moze ovako", __LINE__);

		strncpy_s(_sifra, sifra, _TRUNCATE);

		int size = strlen(naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, naziv);

		_cijena = cijena;
		_kolicina = kolicina;
		_popust = popust;
	}

	Proizvod(const Proizvod& org)
	{
		strcpy_s(_sifra, org._sifra);

		int size = strlen(org._naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, org._naziv);

		_cijena = org._cijena;
		_kolicina = org._kolicina;
		_popust = org._popust;

	}

	Proizvod& operator=(const Proizvod& org)
	{
		if (this != &org)
		{

			strncpy_s(_sifra, org._sifra, _TRUNCATE);

			if (_naziv != nullptr) delete[] _naziv;

			int size = strlen(org._naziv) + 1;
			_naziv = new char[size];
			strcpy_s(_naziv, size, org._naziv);

			_cijena = org._cijena;
			_kolicina = org._kolicina;
			_popust = org._popust;

		}
		return *this;
				
	}
	friend ostream& operator<<(ostream& cout, const Proizvod& p) {
		return cout << "Proizvod - Sifra: " << p._sifra << "\tNaziv: " << p._naziv << "\tCijena: " << p._cijena << "\tKolicina: " << p._kolicina << endl;
	}
	virtual ~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;
	}

	char* GetSifru() { return _sifra; }
	int getKolicinu() { return _kolicina; }
	double getCijenu() { return _cijena; }
	double getPopust() { return _popust; }
	char* GetNaziv() { return _naziv; }
	//Implementirati potrebne funkcije za rad sa klasom
};

//Implementirati nasljeðivanje
class Odjeca : public Proizvod
{
	char*_velicine;
	char* _boje;
	char _kategorija[50];
public:
	//Potrebne konstruktor i destruktor funkcije
	Odjeca(const char* sifra, const char* naziv, double cijena, int kolicina, const char* velicina, const char* boja, const char* kategorija, double popust = 0)
		: Proizvod(sifra, naziv, cijena, kolicina, popust) {
		_velicine = AlocirajNizKaraktera(velicina);
		_boje = AlocirajNizKaraktera(boja);

		strcpy_s(_kategorija, kategorija);

	}

	Odjeca(const Odjeca& org):Proizvod(org._sifra, org._naziv, org._cijena, org._kolicina,org._popust)
	{
		_velicine = AlocirajNizKaraktera(org._velicine);
		_boje = AlocirajNizKaraktera(org._boje);
		strcpy_s(_kategorija, org._kategorija);
	}

	Odjeca& operator=(const Odjeca& right)
	{
		if (this != &right) {
			strncpy_s(_sifra, right._sifra, _TRUNCATE);
			if (_naziv != nullptr) delete[] _naziv;
			_naziv = AlocirajNizKaraktera(right._naziv);
			_cijena = right._cijena;
			_popust = right._popust;
			_kolicina = right._kolicina;
			if (_velicine != nullptr) delete[] _velicine;
			_velicine = AlocirajNizKaraktera(right._velicine);
			if (_boje != nullptr) delete[] _boje;
			_boje = AlocirajNizKaraktera(right._boje);
			strncpy_s(_kategorija, right._kategorija, _TRUNCATE);
		}
		return *this;
	}
	friend ostream& operator<<(ostream& cout, const Odjeca& p) {
		return cout << "ODJECA: " << p << p._boje << endl;
	}
	virtual ~Odjeca()
	{
		delete[] _boje; _boje = nullptr;
		delete[] _velicine; _velicine = nullptr;
	}
	//Implementirati potrebne funkcije za rad sa klasom
};

class Katalog
{
	char* _naslov;
	char* _vrijediDo; //Datum u formatu dd.MM.yyyy
	Proizvod* _proizvodi[100];
	int _trenutnoProizvoda;
public:

	bool vecPostoji(const char* sifra)
	{
		for (int i = 0; i < _trenutnoProizvoda; i++)
		{
			if (strcmp(sifra, _proizvodi[i]->GetSifru()) == 0)
				return true;
		}
		return false;
	}

	int nadjiPoziciju(const char* sifra)
	{
		for (int i = 0; i < _trenutnoProizvoda; i++)
		{
			if (strcmp(sifra, _proizvodi[i]->GetSifru()) == 0)
				return i;
		}
	}
	//Potrebne konstruktor i destruktor funkcije
	Katalog(const char* naslov, const char* vrijediDo)
	{
		//U sluèaju da datum ne ispunjava zadati format baciti izuzetak
		int size = strlen(naslov) + 1;
		_naslov = new char[size];
		strcpy_s(_naslov, size, naslov);

		size = strlen(vrijediDo) + 1;
		_vrijediDo = new char[size];
		strcpy_s(_vrijediDo, size, vrijediDo);

		for (size_t i = 0; i < 100; i++)
		{
			_proizvodi[i] = nullptr;
		}

		_trenutnoProizvoda = 0;
	}

	virtual ~Katalog()
	{
		delete[] _naslov;
		_naslov = nullptr;

		delete[] _vrijediDo;
		_vrijediDo = nullptr;

		for (size_t i = 0; i < _trenutnoProizvoda; i++)
		{
			delete _proizvodi[i];
			_proizvodi[i] = nullptr;
		}
	}

	//Kreirati funkciju za dodavanje proizvoda u katalog i pri tome provjeriti da li se radi
	//o objektu izvedene ili osnovne klase. Funkcija treba da ima povratnu vrijednost tipa bool.
	//Onemoguæiti dodavanje identiènih proizvoda u jedan katalog (proizvode razlikovati samo na osnovu šifre).
	bool DodajProizvod(Proizvod & p)
	{
		if (vecPostoji(p.GetSifru()))
			return false;
		Odjeca* pok = dynamic_cast<Odjeca*>(&p);

		if (pok != nullptr)
		{
			cout << "proizvod je tipa odjeca!" << endl;
			_proizvodi[_trenutnoProizvoda++] = new Odjeca(*pok);
			return true;
		}
		else
		{
			cout << "proizvod je klasican tip!" << endl;
			_proizvodi[_trenutnoProizvoda++] = new Proizvod(p);
			return true;
		}
	}
	//Funkciju za uklanjanje proizvoda iz kataloga na osnovu šifre.
	//Ukoliko se ne pronaðe traženi proizvod baciti izuzetak.
	void UkloniProizvod(const char* sifra)
	{
		if (!vecPostoji(sifra))
		{
			throw NedozvoljenaOperacija("Sifra ne postoji u sistemu", __LINE__);
			return;
		}

		int lok = nadjiPoziciju(sifra);
		for (int i = lok; i < _trenutnoProizvoda-1; i++)
		{
			_proizvodi[i] = _proizvodi[i + 1];
		}
		_proizvodi[_trenutnoProizvoda] = nullptr;
		_trenutnoProizvoda--;
	}
	//Preklopiti operator za ispis kataloške ponude

	friend ostream& operator<<(ostream& cout, const Katalog& p) {
		cout << p._naslov << "\t vrijedi do: " << p._vrijediDo<<endl;
		for (int i = 0; i < p._trenutnoProizvoda; i++)
		{
			cout << *p._proizvodi[i] << endl;
		}
		return cout;
	}

};


class Narudzba
{
	int _brojNarudzbe;
	char* _datumIsporuke; //Datum u formatu dd.MM.yyyy
	Proizvod* _naruceniProizvodi[100];
	int _brojProizvoda;
	bool otkazana;
public:
	bool ValidanDatum(const char* testDatum) {
		int count = 0;
		for (size_t i = 0; i < strlen(testDatum); i++)
		{
			if (testDatum[i] == '.') count++;
		}
		if (count != 3) throw NedozvoljenaOperacija("\tERROR :: Datum nije validan", __LINE__);
	}
	//Potrebne konstruktor i destruktor funkcije
//U sluèaju da datum ne ispunjava zadati format baciti izuzetak
	int nadjiPoziciju(Proizvod& p)
	{
		for (int i = 0; i < _brojProizvoda; i++)
		{
			if (strcmp(p.GetSifru(), _naruceniProizvodi[i]->GetSifru()) == 0)
				return i;
		}
	}

	Narudzba(int brojNarudzbe=0, const char* datum="nepoznat") :_brojNarudzbe(brojNarudzbe), _brojProizvoda(0), otkazana(false)
	{
		if (ValidanDatum(datum)) _datumIsporuke = AlocirajNizKaraktera(datum);
		for (int i = 0; i < 100; i++)
		{
			_naruceniProizvodi[i] = nullptr;
		}
	}

	Narudzba(const Narudzba& original) {
		_brojNarudzbe = original._brojNarudzbe;
		_datumIsporuke = AlocirajNizKaraktera(original._datumIsporuke);
		_brojProizvoda = original._brojProizvoda;

		for (int i = 0; i < 100; i++) {

			if (i < _brojProizvoda) {

				Odjeca* p = dynamic_cast<Odjeca*>(original._naruceniProizvodi[i]);
				if (p != nullptr)	_naruceniProizvodi[i] = new Odjeca(*p);
				else				_naruceniProizvodi[i] = new Proizvod(*original._naruceniProizvodi[i]);

			}
			else _naruceniProizvodi[i] = nullptr;
		}
		otkazana = original.otkazana;
	}
	Narudzba operator=(const Narudzba& org)
	{
		if (this != &org)
		{
			this->_brojNarudzbe = org._brojNarudzbe;
			this->_brojProizvoda = org._brojProizvoda;
			this->otkazana = org.otkazana;
			if (_datumIsporuke != nullptr) delete[] _datumIsporuke; _datumIsporuke = nullptr;
			_datumIsporuke = AlocirajNizKaraktera(org._datumIsporuke);
			for ( int i = 0; i < org._brojProizvoda; i++)
			{
				delete[] _naruceniProizvodi[i];
				_naruceniProizvodi[i] = nullptr;
			}
			for (int i = 0; i < 100; i++)
			{
				if (i < _brojProizvoda) {
					Odjeca* p = dynamic_cast<Odjeca*>(org._naruceniProizvodi[i]);
					if (p != nullptr)	
						_naruceniProizvodi[i] = new Odjeca(*p);
					else				
						_naruceniProizvodi[i] = new Proizvod(*org._naruceniProizvodi[i]);
				}
				else _naruceniProizvodi[i] = nullptr;
			}
		}
		return *this;
	}

	virtual ~Narudzba()
	{
		delete[] _datumIsporuke; _datumIsporuke = nullptr;
		for (int i = 0; i < _brojProizvoda; i++)
		{
			_naruceniProizvodi[i] = nullptr;
		}
	}

	//Kreirati funkciju za dodavanje proizvoda narudžbe sa povratnom vrijednosti tipa bool.
	//Ukoliko proizvod više nije dostupan baciti izuzetak.
	//Dozvoliti dodavanje istog proizvoda više puta.
	bool DodajProizvod(Proizvod& p)
	{
		if (p.getKolicinu() == 0)
		{
			throw NedozvoljenaOperacija("greska nema vise proizvoda", __LINE__);
			return false;
		}
		Odjeca* pok = dynamic_cast<Odjeca*>(&p);
		if (pok != nullptr)
		{
			_naruceniProizvodi[_brojProizvoda++] = new Odjeca(*pok);
			return true;
		}
		else {
			_naruceniProizvodi[_brojProizvoda++] = new Proizvod(p);
			return true;
		}
	}
	//Funkciju za uklanjanje odreðenog proizvoda iz narudžbe. Voditi raèuna da narudžba može 
	//imati više puta isti proizvod.
	void UkloniProizvod(Proizvod& p)
	{
		int k = nadjiPoziciju(p);
		for (int i = k; i < _brojProizvoda-1; i++)
		{
			_naruceniProizvodi[i] = _naruceniProizvodi[i + 1];
		}
		_naruceniProizvodi[_brojProizvoda] = nullptr;
		_brojProizvoda--;
	}
	//Dodati funkciju za proraèun ukupne cijene narudžbe.
	//Voditi raèuna o moguænosti dodavanja više istih proizvoda (kolièina).
	//Uraèunati popust u sluèaju da je definisan.
	double GetUkupnuCijenu() {
		double sum = 0;
		
		for (int i = 0; i < _brojProizvoda; i++)
		{
			sum +=(_naruceniProizvodi[i]->getKolicinu()*_naruceniProizvodi[i]->getCijenu());
			if (_naruceniProizvodi[i]->getPopust() != 0)
				sum = _naruceniProizvodi[i]->getPopust() / 100 * sum;
		}
		return sum;
	}
	//Preklopiti operator za ispis podataka o narudžbi zajedno sa cijenom.
	friend ostream& operator<<(ostream& cout, Narudzba& n)
	{
		cout << n._brojNarudzbe << "\t isporuka-> " << n._datumIsporuke << endl;
		for (int i = 0; i < n._brojProizvoda; i++)
		{
			cout << "Proizvod - Sifra: " << n._naruceniProizvodi[i]->GetSifru() << "\tNaziv: " << n._naruceniProizvodi[i]->GetNaziv() << "\tCijena: " << n._naruceniProizvodi[i]->getCijenu() << "\tKolicina: " << n._naruceniProizvodi[i]->getKolicinu() << endl;

		}
		cout << endl << "----------------------------------------------------------------------" << endl;
		return cout;

	}
};

//U posljednjoj klasi samostalno prepoznati izuzetne situacije
class Prodavac
{
	char* _imePrezime;
	Narudzba* _listaNarudzbi;
	int _brojNarudzbi;
public:
	//Potrebne konstruktor i destruktor funkcije
	Prodavac(const char* ime) :_brojNarudzbi(0)
	{
		_listaNarudzbi = nullptr;
		_imePrezime = AlocirajNizKaraktera(ime);
	}
	//Omoguæiti dodavanje narudžbi za prodavaèa{
	void DodajNarudzbu(const Narudzba& n)
	{
		Narudzba* temp = new Narudzba[_brojNarudzbi + 1];
		for (int i = 0; i < _brojNarudzbi; i++)
		{
			temp[i] = _listaNarudzbi[i];
		}
		temp[_brojNarudzbi] = n;
		delete[] _listaNarudzbi;
		_listaNarudzbi = temp;
		_brojNarudzbi++;
	}

	//Omoguæiti otkazivanje narudžbe na osnovu njenog broja.

	//Ispis svih podataka o prodavaèu i kratke informacije o
	//svim narudžbama (datum, broj proizvoda i cijena). Posebno izdvojiti otkazane narudžbe.

	//Implementirati metodu koja u svim aktivnim narudžbama pronalazi proizvod koji se najviše prodavao
	//sa informacijom o tome da li se radi o odjeæi ili osnovnom proizvodu.

};


int main()
{
#pragma region Proizvodi

	Odjeca airmax("4417", "Nike AirMax", 179.99, 5, "43, 44, 45", "crna, bijela, plava", "Sportska obuca", 20);
	Proizvod jakna("9110", "Adidas jakna", 229.99, 7, 25);
	Odjeca majica("7714", "Tom Tailor", 14.99, 50, "43, 44, 45", "crna, bijela, plava", "Casual", 40);
	Proizvod sprej("4001", "Sprej za kosu", 4.90, 30, 75);
	Odjeca pantole("3121", "Texas Jeans", 74.90, 10, "36, 37, 38, 44", "plava", "Slim-fit", 10);
	Proizvod rukavice("9171", "Zimske rukavice", 11.95, 100, 80);

#pragma endregion

#pragma region Katalog

	Katalog katalog("Zimska akcija", "1.1.2019.");

	katalog.DodajProizvod(airmax);
	katalog.DodajProizvod(jakna);

	katalog.DodajProizvod(airmax);	// Ponovo se dodaje
	katalog.DodajProizvod(jakna);	// Ponovo se dodaje

	katalog.DodajProizvod(majica);
	katalog.DodajProizvod(sprej);
	katalog.DodajProizvod(pantole);
	katalog.DodajProizvod(rukavice);

	katalog.UkloniProizvod("4001");

	cout << katalog;

#pragma endregion

#pragma region Testiranje narudzbe

	Narudzba narudzba1(1, "3.12.2018.");

	narudzba1.DodajProizvod(majica);
	narudzba1.DodajProizvod(sprej);
	narudzba1.DodajProizvod(sprej);
	narudzba1.DodajProizvod(sprej);	// Moze se dodavati vise puta

	Proizvod test("7001", "Testni proizvod", 4.90, 0, 75);	// Kolicina je 0 pa bi trebalo baciti izuzetak

	try {
		narudzba1.DodajProizvod(test);
	}
	catch (NedozvoljenaOperacija& n) { n.ErrorInfo(); }

	narudzba1.UkloniProizvod(sprej);		// Brise sva tri spreja iz narudzbe

	narudzba1.DodajProizvod(rukavice);
	narudzba1.DodajProizvod(sprej);
	narudzba1.DodajProizvod(sprej);
	narudzba1.DodajProizvod(pantole);
	narudzba1.DodajProizvod(rukavice);
	narudzba1.DodajProizvod(jakna);
	narudzba1.DodajProizvod(rukavice);

	cout << "\nUkupna cijena narudzbe [Uracunat popust] :: " << narudzba1.GetUkupnuCijenu() << "KM" << endl;
	cout << narudzba1;

	// Testiranje operatora=
	Narudzba t(2, "22.7.2018.");
	t = narudzba1;
	cout << t;

	Narudzba narudzba2(3, "4.12.2018.");
	narudzba2.DodajProizvod(airmax);
	narudzba2.DodajProizvod(pantole);
	narudzba2.DodajProizvod(jakna);

	Narudzba narudzba3(4, "11.3.2018.");
	narudzba3.DodajProizvod(majica);
	narudzba3.DodajProizvod(majica);
	narudzba3.DodajProizvod(jakna);
	narudzba3.DodajProizvod(pantole);

#pragma endregion

#pragma region Prodavac

	Prodavac _prodavac("Ajdin Hukara");
	_prodavac.DodajNarudzbu(narudzba1);
	_prodavac.DodajNarudzbu(narudzba2);
	_prodavac.DodajNarudzbu(narudzba3);
	try {
		_prodavac.DodajNarudzbu(t);		// Dodavanje iste narudzbe
	}
	catch (NedozvoljenaOperacija& n) { n.ErrorInfo(); }

	//try {
	//	_prodavac.OtkaziNarudzbu(47);		// Otkazivanje narudzbe koja ne postoji
	//}
	//catch (NedozvoljenaOperacija& n) { n.ErrorInfo(); }

	//_prodavac.OtkaziNarudzbu(3);

	//_prodavac.InfoNarudzba();

	//_prodavac.NajprodavanijiProizvodi();

#pragma endregion

	system("pause>0");
	return 0;
}